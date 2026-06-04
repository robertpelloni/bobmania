#include "RageFileManager.h"

#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>

#if defined(WIN32)
#include <windows.h>
#elif defined(UNIX) || defined(MACOSX)
#include <paths.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <miniz.h>

RageFileManager* FILEMAN = nullptr;

/* Lock this before touching any of these globals (except FILEMAN itself). */
static RageEvent* g_Mutex;

std::string RageFileManagerUtil::sDirOfExecutable;

struct LoadedDriver {
  /* A loaded driver may have a base path, which modifies the path we
   * pass to the driver.  For example, if the base is "Songs/", and we
   * want to send the path "Songs/Foo/Bar" to it, then we actually
   * only send "Foo/Bar".  The path "Themes/Foo" is out of the scope
   * of the driver, and GetPath returns false. */
  RageFileDriver* m_pDriver;
  std::string m_sType, m_sRoot, m_sMountPoint;

  int m_iRefs;

  LoadedDriver() {
    m_pDriver = nullptr;
    m_iRefs = 0;
  }
  std::string GetPath(const std::string& sPath) const;
};

static std::vector<LoadedDriver*> g_pDrivers;
static std::map<const RageFileBasic*, LoadedDriver*> g_mFileDriverMap;

static void ReferenceAllDrivers(std::vector<LoadedDriver*>& apDriverList) {
  g_Mutex->Lock();
  apDriverList = g_pDrivers;
  for (unsigned i = 0; i < apDriverList.size(); ++i) {
    ++apDriverList[i]->m_iRefs;
  }
  g_Mutex->Unlock();
}

static void UnreferenceAllDrivers(std::vector<LoadedDriver*>& apDriverList) {
  g_Mutex->Lock();
  for (unsigned i = 0; i < apDriverList.size(); ++i) {
    --apDriverList[i]->m_iRefs;
  }
  g_Mutex->Broadcast();
  g_Mutex->Unlock();

  /* Clear the temporary list, to make it clear that the drivers may no longer
   * be accessed. */
  apDriverList.clear();
}

RageFileDriver *RageFileManager::GetFileDriver( RString sMountpoint )
{
	FixSlashesInPlace( sMountpoint );
	if( sMountpoint.size() && !StrUtil::EndsWith(sMountpoint, "/") )
		sMountpoint += '/';

	g_Mutex->Lock();
	RageFileDriver *pRet = nullptr;
	for( unsigned i = 0; i < g_pDrivers.size(); ++i )
	{
		if( g_pDrivers[i]->m_sType == "mountpoints" )
			continue;
		if( g_pDrivers[i]->m_sMountPoint.CompareNoCase( sMountpoint ) )
			continue;

    pRet = g_pDrivers[i]->m_pDriver;
    ++g_pDrivers[i]->m_iRefs;
    break;
  }
  g_Mutex->Unlock();

  return pRet;
}

void RageFileManager::ReleaseFileDriver(RageFileDriver* pDriver) {
  ASSERT(pDriver != nullptr);

  g_Mutex->Lock();
  unsigned i;
  for (i = 0; i < g_pDrivers.size(); ++i) {
    if (g_pDrivers[i]->m_pDriver == pDriver) {
      break;
    }
  }
  ASSERT(i != g_pDrivers.size());

  --g_pDrivers[i]->m_iRefs;

  g_Mutex->Broadcast();
  g_Mutex->Unlock();
}

std::size_t zipRead(void *pOpaque, mz_uint64 file_ofs, void *pBuf, std::size_t n)
{
	RageFile *f = static_cast<RageFile*>(pOpaque);

	const int pos = f->Seek(file_ofs);
	if (pos >= 0 && static_cast<std::uint64_t>(pos) != file_ofs)
	{
		return 0;
	}

  return f->Read(pBuf, n);
}

std::size_t zipWriteFile(void *pOpaque, mz_uint64 file_ofs, const void *pBuf, std::size_t n)
{
	RageFile *f = static_cast<RageFile*>(pOpaque);

  /*
   * XXX: RageFile doesn't allow to Seek() in a file open for writing. We
   * rely on the fact that miniz writes the file in order. Tell() is not
   * allowed either, so we can't even check that the current offset is
   * correct.
   */
  /*
  int pos = f->Seek(file_ofs);
  if (pos != file_ofs)
  {
          return 0;
  }
  */

  return f->Write(pBuf, n);
}

bool RageFileManager::Unzip(
    const std::string& zipPath, std::string targetPath, int strip) {
  if (targetPath.empty() || targetPath.back() != '/') {
    targetPath.push_back('/');
  }

  RageFile zipFile;
  if (!zipFile.Open(zipPath, RageFile::READ)) {
    std::string error = zipFile.GetError();
    LOG->Warn("Could not unzip %s: %s", zipPath.c_str(), error.c_str());
    return false;
  }

  mz_zip_archive zip = {};
  zip.m_pRead = zipRead;
  zip.m_pIO_opaque = &zipFile;

  if (!mz_zip_reader_init(&zip, zipFile.GetFileSize(), 0)) {
    LOG->Warn(
        "Could not unzip %s: %s", zipPath.c_str(),
        mz_zip_get_error_string(zip.m_last_error));
    mz_zip_reader_end(&zip);
    return false;
  }

  bool success = true;
  mz_uint file_count = mz_zip_reader_get_num_files(&zip);

  for (mz_uint fileIndex = 0; fileIndex < file_count; fileIndex++) {
    mz_zip_archive_file_stat info;
    if (!mz_zip_reader_file_stat(&zip, fileIndex, &info)) {
      LOG->Warn(
          "Could not unzip %s: %s", zipPath.c_str(),
          mz_zip_get_error_string(zip.m_last_error));
      success = false;
      break;
    }

    std::string filename(info.m_filename);
    if (filename.back() == '/') {
      filename.pop_back();
    }

		for (int i = 0; i < strip; i++)
		{
			std::size_t pos = filename.find('/');
			if (pos != std::string::npos)
				pos++;
			filename.erase(0, pos);
		}
		if (filename.empty())
			continue;

    std::string filepath = targetPath + filename;

    if (FILEMAN->IsPathProtected(filepath)) {
      LOG->Warn("Overwriting %s is not allowed", filepath.c_str());
      continue;
    }

    if (info.m_is_directory) {
      CreateDir(filepath);
    } else {
      RageFile f;
      if (!f.Open(filepath, RageFile::WRITE | RageFile::STREAMED)) {
        std::string error = zipFile.GetError();
        LOG->Warn("Could not write to %s: %s", filepath.c_str(), error.c_str());
        success = false;
        break;
      }

      success = mz_zip_reader_extract_to_callback(
          &zip, fileIndex, zipWriteFile, &f, 0);
      if (!success) {
        std::string error = f.GetError();
        LOG->Warn("Could not write to %s: %s", filepath.c_str(), error.c_str());
        FILEMAN->Remove(filepath);
        break;
      }
    }
  }

  mz_zip_reader_end(&zip);
  return success;
}

static void NormalizePath(std::string& sPath) {
  FixSlashesInPlace(sPath);
  CollapsePath(sPath, true);
  if (sPath.size() == 0) {
    sPath = '/';
  } else if (sPath[0] != '/') {
    sPath = '/' + sPath;
  }
}

void RageFileManager::ProtectPath(const std::string& path)
{
	RString normalizedPath(path);
	NormalizePath(normalizedPath);
	normalizedPath.MakeLower();

  m_protectedPaths.insert(normalizedPath);
}

bool RageFileManager::IsPathProtected(const std::string& path)
{
	RString normalizedPath(path);
	NormalizePath(normalizedPath);
	normalizedPath.MakeLower();

  return m_protectedPaths.count(normalizedPath) > 0;
}

// Mountpoints as directories cause a problem.  If "Themes/default" is a
// mountpoint, and doesn't exist anywhere else, then GetDirListing("Themes/*")
// must return "default".  The driver containing "Themes/default" won't do this;
// its world view begins at "BGAnimations" (inside "Themes/default").  We need a
// dummy driver that handles mountpoints. */
class RageFileDriverMountpoints : public RageFileDriver {
 public:
  RageFileDriverMountpoints() : RageFileDriver(new FilenameDB) {}
  RageFileBasic* Open(const std::string& sPath, int iMode, int& iError) {
    iError = (iMode == RageFile::WRITE) ? ERROR_WRITING_NOT_SUPPORTED : ENOENT;
    return nullptr;
  }
  /* Never flush FDB, except in LoadFromDrivers. */
  void FlushDirCache(const std::string& sPath) {}

  void LoadFromDrivers(const std::vector<LoadedDriver*>& apDrivers) {
    /* XXX: Even though these two operations lock on their own, lock around
     * them, too.  That way, nothing can sneak in and get incorrect
     * results between the flush and the re-population. */
    FDB->FlushDirCache();
    for (unsigned i = 0; i < apDrivers.size(); ++i) {
      if (apDrivers[i]->m_sMountPoint != "/") {
        FDB->AddFile(apDrivers[i]->m_sMountPoint, 0, 0);
      }
    }
  }
};
static RageFileDriverMountpoints* g_Mountpoints = nullptr;

static RString ExtractDirectory( RString sPath )
{
	// return the directory containing sPath
	std::size_t n = sPath.find_last_of("/");
	if( n != sPath.npos )
		sPath.erase(n);
	else
		sPath.erase();
	return sPath;
}

#if defined(UNIX) || defined(MACOSX)
static RString ReadlinkRecursive( RString sPath )
{
	// unices support symbolic links; dereference them
	RString dereferenced = sPath;
	do
	{
		sPath = dereferenced;
		char derefPath[512];
		ssize_t linkSize = readlink(sPath, derefPath, sizeof(derefPath));
		if ( linkSize != -1 && linkSize != sizeof(derefPath) )
		{
			dereferenced = RString( derefPath, linkSize );
			if (derefPath[0] != '/')
			{
				// relative link
				dereferenced = RString( ExtractDirectory(sPath) + "/" + dereferenced);
			}
		}
	} while (sPath != dereferenced);

  return sPath;
}
#endif

static std::string GetDirOfExecutable(std::string argv0) {
  // argv[0] can be wrong in most OS's; try to avoid using it.

	RString sPath;
#if defined(WIN32)
	char szBuf[MAX_PATH];
	GetModuleFileName( nullptr, szBuf, sizeof(szBuf) );
	sPath = szBuf;
#else
  sPath = argv0;
#endif

	sPath.Replace( "\\", "/" );

	bool bIsAbsolutePath = false;
	if( sPath.size() == 0 || sPath[0] == '/' )
		bIsAbsolutePath = true;
#if defined(WIN32)
	if( sPath.size() > 2 && sPath[1] == ':' && sPath[2] == '/' )
		bIsAbsolutePath = true;
#endif

  // strip off executable name
  sPath = ExtractDirectory(sPath);

  if (!bIsAbsolutePath) {
#if defined(UNIX) || defined(MACOSX)
    if (sPath.empty()) {
      // This is in our path so look for it.
      const char* path = getenv("PATH");

      if (!path) {
        path = _PATH_DEFPATH;
      }

			std::vector<RString> vPath;
			split( path, ":", vPath );
			for (RString &i : vPath)
			{
				if( access(i + "/" + argv0, X_OK|R_OK) )
					continue;
				sPath = ExtractDirectory(ReadlinkRecursive(i + "/" + argv0));
				break;
			}
			if( sPath.empty() )
				sPath = GetCwd(); // What?
			else if( sPath[0] != '/' ) // For example, if . is in $PATH.
				sPath = GetCwd() + "/" + sPath;

    } else {
      sPath = ExtractDirectory(ReadlinkRecursive(GetCwd() + "/" + argv0));
    }
#else
		sPath = GetCwd() + "/" + sPath;
		sPath.Replace( "\\", "/" );
#endif
  }
  return sPath;
}

static void ChangeToDirOfExecutable(const std::string& argv0) {
  RageFileManagerUtil::sDirOfExecutable = GetDirOfExecutable(argv0);

  /* Set the CWD.  Any effects of this is platform-specific; most files are read
   * and written through RageFile.  See also RageFileManager::RageFileManager.
   */
#if defined(_WINDOWS)
	if( _chdir( RageFileManagerUtil::sDirOfExecutable + "/.." ) )
#elif defined(UNIX)
	if( chdir( RageFileManagerUtil::sDirOfExecutable + "/" ) )
#elif defined(MACOSX)
	/* If the basename is not MacOS, then we've likely been launched via the command line
	 * through a symlink. Assume this is the case and change to the dir of the symlink. */
	if( Basename(RageFileManagerUtil::sDirOfExecutable) == "MacOS" )
		CollapsePath( RageFileManagerUtil::sDirOfExecutable += "/../../../" );
	if( chdir( RageFileManagerUtil::sDirOfExecutable ) )
#endif
  {
    LOG->Warn(
        "Can't set current working directory to %s",
        RageFileManagerUtil::sDirOfExecutable.c_str());
    return;
  }
}

RageFileManager::RageFileManager(const std::string& argv0) {
  CHECKPOINT_M(argv0);
  ChangeToDirOfExecutable(argv0);

  g_Mutex = new RageEvent("RageFileManager");

  g_Mountpoints = new RageFileDriverMountpoints;
  LoadedDriver* pLoadedDriver = new LoadedDriver;
  pLoadedDriver->m_pDriver = g_Mountpoints;
  pLoadedDriver->m_sMountPoint = "/";
  pLoadedDriver->m_sType = "mountpoints";
  g_pDrivers.push_back(pLoadedDriver);

  /* The mount path is unused, but must be nonempty. */
  RageFileManager::Mount("mem", "(cache)", "/@mem");

  // Register with Lua.
  {
    Lua* L = LUA->Get();
    lua_pushstring(L, "FILEMAN");
    this->PushSelf(L);
    lua_settable(L, LUA_GLOBALSINDEX);
    LUA->Release(L);
  }
}

void RageFileManager::MountInitialFilesystems() {
  HOOKS->MountInitialFilesystems(RageFileManagerUtil::sDirOfExecutable);
}

void RageFileManager::MountUserFilesystems() {
  HOOKS->MountUserFilesystems(RageFileManagerUtil::sDirOfExecutable);
}

RageFileManager::~RageFileManager() {
  // Unregister with Lua.
  LUA->UnsetGlobal("FILEMAN");

  /* Note that drivers can use previously-loaded drivers, eg. to load a ZIP
   * from the FS.  Unload drivers in reverse order. */
  for (int i = g_pDrivers.size() - 1; i >= 0; --i) {
    delete g_pDrivers[i]->m_pDriver;
    delete g_pDrivers[i];
  }
  g_pDrivers.clear();

  //	delete g_Mountpoints; // g_Mountpoints was in g_pDrivers
  g_Mountpoints = nullptr;

  delete g_Mutex;
  g_Mutex = nullptr;
}

/* path must be normalized (FixSlashesInPlace, CollapsePath). */
std::string LoadedDriver::GetPath(const std::string& sPath) const {
  /* If the path begins with /@, only match mountpoints that begin with /@. */
  if (sPath.size() >= 2 && sPath[1] == '@') {
    if (m_sMountPoint.size() < 2 || m_sMountPoint[1] != '@') {
      return std::string();
    }
  }


  std::vector<std::string> vsFiles;
  prfd->GetDirListing(sDir + "*", vsFiles, false, true);
  for (const std::string& s : vsFiles) {
    if (IsADirectory(s)) {
      DeleteRecursive(s + "/");
    } else {
      FILEMAN->Remove(s);
    }
  }

  return FILEMAN->Remove(sDir);
}

bool DeleteRecursive( const RString &sDir )
{
	ASSERT( StrUtil::EndsWith(sDir, "/") );
=======
>>>>>>> Stashed changes

  std::vector<std::string> vsFiles;
  GetDirListing(sDir + "*", vsFiles, false, true);
  for (const std::string& s : vsFiles) {
    if (IsADirectory(s)) {
      DeleteRecursive(s + "/");
    } else {
      FILEMAN->Remove(s);
    }
  }

  return FILEMAN->Remove(sDir);
}

unsigned int GetHashForFile(const std::string& sPath) {
  return FILEMAN->GetFileHash(sPath);
}

unsigned int GetHashForDirectory(const std::string& sDir) {
  unsigned int hash = 0;

  hash += GetHashForString(sDir);

  std::vector<std::string> arrayFiles;
  GetDirListing(sDir + "*", arrayFiles, false);
  for (unsigned i = 0; i < arrayFiles.size(); i++) {
    const std::string sFilePath = sDir + arrayFiles[i];
    hash += GetHashForFile(sFilePath);
  }

  return hash;
}

// lua start
#include "LuaBinding.h"

/** @brief Allow Lua to have access to the RageFileManager. */
class LunaRageFileManager : public Luna<RageFileManager> {
 public:
  static int Copy(T* p, lua_State* L) {
    const std::string fromPath = SArg(1);
    const std::string toPath = SArg(2);

    if (p->IsPathProtected(toPath)) {
      LOG->Warn("Overwriting %s is not allowed", toPath.c_str());
      lua_pushboolean(L, false);
      return 1;
    }

    lua_pushboolean(L, p->Copy(fromPath, toPath));
    return 1;
  }
  static int DoesFileExist(T* p, lua_State* L) {
    lua_pushboolean(L, p->DoesFileExist(SArg(1)));
    return 1;
  }
  static int GetFileSizeBytes(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetFileSizeInBytes(SArg(1)));
    return 1;
  }
  static int GetHashForFile(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetFileHash(SArg(1)));
    return 1;
  }
  static int GetDirListing(T* p, lua_State* L) {
    std::vector<std::string> vDirs;
    bool bOnlyDirs = false;
    bool bReturnPathToo = false;

    // the last two arguments of GetDirListing are optional;
    // let's reflect that in the Lua too. -aj
    if (lua_gettop(L) >= 2 && !lua_isnil(L, 2)) {
      bOnlyDirs = BArg(2);
      if (!lua_isnil(L, 3)) {
        bReturnPathToo = BArg(3);
      }
    }
    //( Path, addTo, OnlyDirs=false, ReturnPathToo=false );
    p->GetDirListing(SArg(1), vDirs, bOnlyDirs, bReturnPathToo);
    LuaHelpers::CreateTableFromArray(vDirs, L);
    return 1;
  }
  static int Unzip(T* p, lua_State* L) {
    std::string zipPath = SArg(1);
    std::string targetPath = SArg(2);

    int strip = 0;
    if (lua_gettop(L) >= 3) {
      strip = IArg(3);
    }

    bool success = p->Unzip(zipPath, targetPath, strip);

    lua_pushboolean(L, success);
    return 1;
  }

  LunaRageFileManager() {
    ADD_METHOD(Copy);
    ADD_METHOD(DoesFileExist);
    ADD_METHOD(GetFileSizeBytes);
    ADD_METHOD(GetHashForFile);
    ADD_METHOD(GetDirListing);
    ADD_METHOD(Unzip);
  }
};

LUA_REGISTER_CLASS(RageFileManager)
// lua end

/*
 * Copyright (c) 2001-2004 Glenn Maynard, Chris Danford
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
