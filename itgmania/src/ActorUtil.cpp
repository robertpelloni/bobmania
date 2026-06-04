#include "global.h"

#include "ActorUtil.h"

#include <vector>

#include "Course.h"
#include "EnumHelper.h"
#include "GameState.h"
#include "IniFile.h"
#include "LuaManager.h"
#include "PrefsManager.h"
#include "RageException.h"
#include "RageFileManager.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "Song.h"
#include "ThemeManager.h"
#include "XmlFile.h"
#include "XmlFileUtil.h"
#include "arch/Dialog/Dialog.h"


// Actor registration
static std::map<RString, CreateActorFn> *g_pmapRegistrees = nullptr;

static bool IsRegistered(const RString& sClassName) {
  return g_pmapRegistrees->find(sClassName) != g_pmapRegistrees->end();
}

void ActorUtil::Register(const RString& sClassName, CreateActorFn pfn) {
  if (g_pmapRegistrees == nullptr) {
    g_pmapRegistrees = new std::map<RString, CreateActorFn>;
  }

  std::map<RString, CreateActorFn>::iterator iter =
      g_pmapRegistrees->find(sClassName);
  ASSERT_M(
      iter == g_pmapRegistrees->end(),
      ssprintf("Actor class '%s' already registered.", sClassName.c_str()));

  (*g_pmapRegistrees)[sClassName] = pfn;
}

// Resolves actor paths a la LoadActor("..."), with autowildcarding and .redir
// files. Returns a path within* the Rage filesystem, unlike the FILEMAN
// function of the same name.
bool ActorUtil::ResolvePath(RString& sPath, const RString& sName,
                            bool optional) {
  CollapsePath(sPath);

  // If we know this is an exact match, don't bother with the GetDirListing,
  // so "foo" doesn't partial match "foobar" if "foo" exists.
  RageFileManager::FileType ft = FILEMAN->GetFileType(sPath);
  if (ft != RageFileManager::TYPE_FILE && ft != RageFileManager::TYPE_DIR) {
    std::vector<RString> asPaths;
    GetDirListing(sPath + "*", asPaths, false, true);  // return path too

    if (asPaths.empty()) {
      if (optional) {
        return false;
      }
      RString sError =
          ssprintf("%s: references a file \"%s\" which doesn't exist",
                   sName.c_str(), sPath.c_str());
      Dialog::Result result =
          LuaHelpers::ReportScriptError(sError, "BROKEN_FILE_REFERENCE", true);
      switch (result) {
        case Dialog::abort:
          RageException::Throw("%s", sError.c_str());
          break;
        case Dialog::retry:
          FILEMAN->FlushDirCache();
          return ResolvePath(sPath, sName);
        case Dialog::ignore:
          return false;
        default:
          FAIL_M("Invalid response to Abort/Retry/Ignore dialog");
      }
    }

    THEME->FilterFileLanguages(asPaths);

    if (asPaths.size() > 1) {
      RString sError =
          ssprintf("%s: references a file \"%s\" which has multiple matches",
                   sName.c_str(), sPath.c_str());
      sError += "\n" + join("\n", asPaths);
      Dialog::Result result =
          LuaHelpers::ReportScriptError(sError, "BROKEN_FILE_REFERENCE", true);
      switch (result) {
        case Dialog::abort:
          RageException::Throw("%s", sError.c_str());
          break;
        case Dialog::retry:
          FILEMAN->FlushDirCache();
          return ResolvePath(sPath, sName);
        case Dialog::ignore:
          asPaths.erase(asPaths.begin() + 1, asPaths.end());
          break;
        default:
          FAIL_M("Invalid response to Abort/Retry/Ignore dialog");
      }
    }

    sPath = asPaths[0];
  }

  if (ft == RageFileManager::TYPE_DIR) {
    RString sLuaPath = sPath + "/default.lua";
    if (DoesFileExist(sLuaPath)) {
      sPath = sLuaPath;
      return true;
    }
  }

  sPath = DerefRedir(sPath);
  return true;
}

namespace {

RString GetLegacyActorClass(XNode* pActor) {
  DEBUG_ASSERT(PREFSMAN->m_bQuirksMode);
  ASSERT(pActor);

  // The non-legacy LoadFromNode has already checked the Class and
  // Type attributes.

  if (pActor->GetAttr("Text") != nullptr) {
    return "BitmapText";
  }

  RString sFile;
  if (pActor->GetAttrValue("File", sFile) && sFile != "") {
    // Backward compatibility hacks for "special" filenames
    if (sFile.EqualsNoCase("songbackground")) {
      XNodeStringValue *pVal = new XNodeStringValue;
      Song* pSong = GAMESTATE->cur_song_;
      if (pSong && pSong->HasBackground()) {
        pVal->SetValue(pSong->GetBackgroundPath());
      } else {
        pVal->SetValue(THEME->GetPathG("Common", "fallback background"));
      }
      pActor->AppendAttrFrom("Texture", pVal, false);
      return "Sprite";
    } else if (sFile.EqualsNoCase("songbanner")) {
      XNodeStringValue *pVal = new XNodeStringValue;
      Song* pSong = GAMESTATE->cur_song_;
      if (pSong && pSong->HasBanner()) {
        pVal->SetValue(pSong->GetBannerPath());
      } else {
        pVal->SetValue(THEME->GetPathG("Common", "fallback banner"));
      }
      pActor->AppendAttrFrom("Texture", pVal, false);
      return "Sprite";
    } else if (sFile.EqualsNoCase("coursebanner")) {
      XNodeStringValue *pVal = new XNodeStringValue;
      Course* pCourse = GAMESTATE->cur_course_;
      if (pCourse && pCourse->HasBanner()) {
        pVal->SetValue(pCourse->GetBannerPath());
      } else {
        pVal->SetValue(THEME->GetPathG("Common", "fallback banner"));
      }
      pActor->AppendAttrFrom("Texture", pVal, false);
      return "Sprite";
    }
  }

  // Fallback: use XML tag name for actor class
  return pActor->m_sName;
}

}  // namespace

Actor* ActorUtil::LoadFromNode(const XNode* _pNode, Actor* pParentActor) {
  ASSERT(_pNode != nullptr);

  XNode node = *_pNode;

  // Remove this in favor of using conditionals in Lua. -Chris
  // There are a number of themes out there that depend on this (including
  // sm-ssc default). Probably for the best to leave this in. -aj
  {
    bool bCond;
    if (node.GetAttrValue("Condition", bCond) && !bCond) return nullptr;
  }

  RString sClass;
  bool bHasClass = node.GetAttrValue("Class", sClass);
  if (!bHasClass) bHasClass = node.GetAttrValue("Type", sClass);

  bool bLegacy = (node.GetAttr("_LegacyXml") != nullptr);
  if (!bHasClass && bLegacy) {
    sClass = GetLegacyActorClass(&node);
  }

  auto iter = g_pmapRegistrees->find(sClass);
  if (iter == g_pmapRegistrees->end()) {
    RString sFile;
    if (bLegacy && node.GetAttrValue("File", sFile) && sFile != "") {
      RString sPath;
      // Handle absolute paths correctly
      if (sFile.Left(1) == "/") {
        sPath = sFile;
      } else {
        sPath = Dirname(GetSourcePath(&node)) + sFile;
      }
      if (ResolvePath(sPath, GetWhere(&node))) {
        Actor *pNewActor = MakeActor(sPath, pParentActor);
        if (pNewActor == nullptr) {
          return nullptr;
        }
        if (pParentActor) {
          pNewActor->SetParent(pParentActor);
        }
        pNewActor->LoadFromNode(&node);
        return pNewActor;
      }
    }

    // sClass is invalid
    RString sError =
        ssprintf("%s: invalid Class \"%s\"", ActorUtil::GetWhere(&node).c_str(),
                 sClass.c_str());
    LuaHelpers::ReportScriptError(sError);
    // Return a dummy object so that we don't crash in AutoActor later.
    return new Actor;  
  }

  const CreateActorFn& pfn = iter->second;
  Actor* pRet = pfn();

  if (pParentActor) {
    pRet->SetParent(pParentActor);
  }

  pRet->LoadFromNode(&node);
  return pRet;
}

namespace {

XNode* LoadXNodeFromLuaShowErrors(const RString& sFile) {
  RString sScript;
  if (!GetFileContents(sFile, sScript)) {
    return nullptr;
  }

  Lua* L = LUA->Get();

  RString sError;
  if (!LuaHelpers::LoadScript(L, sScript, "@" + sFile, sError)) {
    LUA->Release(L);
    sError = ssprintf("Lua runtime error: %s", sError.c_str());
    LuaHelpers::ReportScriptError(sError);
    return nullptr;
  }

  XNode* pRet = nullptr;
  if (ActorUtil::LoadTableFromStackShowErrors(L)) {
    pRet = XmlFileUtil::XNodeFromTable(L);
  }

  LUA->Release(L);
  return pRet;
}
}  // namespace

}  // namespace

// Run the function at the top of the stack, which returns an actor description
// table.  If the table was returned, return true and leave it on the stack.
// If not, display an error, return false, and leave nothing on the stack.
bool ActorUtil::LoadTableFromStackShowErrors(Lua *L) {
  LuaReference func;
  lua_pushvalue(L, -1);
  func.SetFromStack(L);

  RString Error = "Lua runtime error: ";
  if (!LuaHelpers::RunScriptOnStack(L, Error, 0, 1, true)) {
    lua_pop(L, 1);
    return false;
  }

  if (lua_type(L, -1) != LUA_TTABLE) {
    lua_pop(L, 1);

    func.PushSelf(L);
    lua_Debug debug;
    lua_getinfo(L, ">nS",& debug);

    Error = ssprintf("%s: must return a table", debug.short_src);

    LuaHelpers::ReportScriptError(Error, "LUA_ERROR");
    return false;
  }
  return true;
}

// NOTE: This function can return nullptr if the actor should not be displayed.
// Callers should be aware of this and handle it appropriately.
Actor* ActorUtil::MakeActor(const RString& sPath_, Actor *pParentActor) {
  RString sPath(sPath_);

  FileType ft = GetFileType(sPath);
  switch (ft) {
    case FT_Lua: {
      std::unique_ptr<XNode> pNode(LoadXNodeFromLuaShowErrors(sPath));
      if (pNode.get() == nullptr) {
        // XNode will warn about the error
        return new Actor;
      }

      Actor *pRet = ActorUtil::LoadFromNode(pNode.get(), pParentActor);
      return pRet;
    }
    case FT_Xml: {
      // Legacy actors; only supported in quirks mode
      if (!PREFSMAN->m_bQuirksMode) {
        return new Actor;
      }

    if (!pNode->GetAttrValue("_Dir", sDir)) {
      if (!optional) {
        LOG->Warn("Relative path \"%s\", but path is unknown", sOut.c_str());
      }
      return false;
    }
    sOut = sDir + sOut;
  }
=======
>>>>>>> upstream/release
=======
FileType ActorUtil::GetFileType(const RString& sPath) {
  RString sExt = GetExtension(sPath);
  sExt.MakeLower();
>>>>>>> Stashed changes

  etft_cont_t::iterator conversion_entry = ExtensionToFileType.find(sExt);
  if (conversion_entry != ExtensionToFileType.end()) {
    return conversion_entry->second;
  } else if (sPath.size() > 0 && sPath[sPath.size() - 1] == '/') {
    return FT_Directory;
  } else if (IsADirectory(sPath)) {
    // Do this last, to avoid the IsADirectory call above in most cases.
    return FT_Directory;
  }
  return FileType_Invalid;
}

// lua start
#include "LuaBinding.h"


  LUA->YieldLua();
  bool bRet = ActorUtil::ResolvePath(sPath, sWhere, optional);
  LUA->UnyieldLua();

  if (!bRet) {
    return 0;
  }
  LuaHelpers::Push(L, sPath);
  return 1;
}
int IsRegisteredClass(lua_State* L) {
  lua_pushboolean(L, IsRegistered(SArg(1)));
  return 1;
}
static void name_error(Actor* p, lua_State* L) {
  if (p->GetName() == "") {
    luaL_error(L, "LoadAllCommands requires the actor to have a name.");
  }
}
static int LoadAllCommands(lua_State* L) {
  Actor* p = Luna<Actor>::check(L, 1);
  name_error(p, L);
  ActorUtil::LoadAllCommands(p, SArg(2));
  return 0;
}
static int LoadAllCommandsFromName(lua_State* L) {
  Actor* p = Luna<Actor>::check(L, 1);
  name_error(p, L);
  ActorUtil::LoadAllCommandsFromName(*p, SArg(2), SArg(3));
  return 0;
}
static int LoadAllCommandsAndSetXY(lua_State* L) {
  Actor* p = Luna<Actor>::check(L, 1);
  name_error(p, L);
  ActorUtil::LoadAllCommandsAndSetXY(p, SArg(2));
  return 0;
}

const luaL_Reg ActorUtilTable[] = {
    LIST_METHOD(GetFileType),
    LIST_METHOD(ResolvePath),
    LIST_METHOD(IsRegisteredClass),
    LIST_METHOD(LoadAllCommands),
    LIST_METHOD(LoadAllCommandsFromName),
    LIST_METHOD(LoadAllCommandsAndSetXY),
    {nullptr, nullptr}
};

}  // namespace

LUA_REGISTER_NAMESPACE(ActorUtil)

/*
 * (c) 2003-2004 Chris Danford
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
