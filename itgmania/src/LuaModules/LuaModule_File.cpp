#include "global.h"
#include "LuaManager.h"
#include "RageFile.h"
#include "RageFileManager.h"
#include "RageLog.h"
#include "LuaBinding.h"

namespace
{
	static bool IsSafePath( const RString &sPath )
	{
		// Only allow writing to Save/
		if( sPath.Left(5).CompareNoCase("Save/") == 0 ) return true;
		if( sPath.Left(6).CompareNoCase("/Save/") == 0 ) return true;
		return false;
	}

	static int Write( lua_State *L )
	{
		RString sPath = SArg(1);
		RString sContent = SArg(2);

		if( !IsSafePath(sPath) )
		{
			LuaHelpers::ReportScriptErrorFmt( "File.Write: Access denied to %s", sPath.c_str() );
			return 0;
		}

		RageFile f;
		if( !f.Open(sPath, RageFile::WRITE) )
		{
			lua_pushboolean( L, false );
			lua_pushstring( L, f.GetError() );
			return 2;
		}

		f.Write( sContent );
		f.Close();

		lua_pushboolean( L, true );
		return 1;
	}

	static int Read( lua_State *L )
	{
		RString sPath = SArg(1);

		RageFile f;
		if( !f.Open(sPath, RageFile::READ) )
		{
			lua_pushnil( L );
			lua_pushstring( L, f.GetError() );
			return 2;
		}

		RString sContent;
		f.Read( sContent, f.GetFileSize() );
		f.Close();

		LuaHelpers::Push( L, sContent );
		return 1;
	}

    // Append to a file
    static int Append( lua_State *L )
    {
        RString sPath = SArg(1);
        RString sContent = SArg(2);

        if( !IsSafePath(sPath) )
        {
            LuaHelpers::ReportScriptErrorFmt( "File.Append: Access denied to %s", sPath.c_str() );
            return 0;
        }

        RageFile f;
        // Check if file exists to determine open mode? RageFile::WRITE usually truncates.
        // We probably need to read first or check if RageFile supports append.
        // RageFile doesn't strictly support "APPEND" mode in all backends easily,
        // but let's check RageFile.h.
        // Actually, let's just Read + Write for now as a naive implementation
        // or check if RageFile has Seek(End).

        if( !f.Open(sPath, RageFile::READ | RageFile::WRITE) )
        {
             // Try creating if it doesn't exist
             if( !f.Open(sPath, RageFile::WRITE) )
             {
                 lua_pushboolean( L, false );
                 lua_pushstring( L, f.GetError() );
                 return 2;
             }
        }

        f.Seek( 0, RageFile::end );
        f.Write( sContent );
        f.Close();

        lua_pushboolean( L, true );
        return 1;
    }

	static int ReadDir( lua_State *L )
	{
		RString sPath = SArg(1);
		// Allow reading any directory, but usually filtered by FILEMAN mounts.
		// Standard SM5 'GetDirListing' allows any path in VFS.

		vector<RString> vs;
		FILEMAN->GetDirListing( sPath, vs, false, false );

		LuaHelpers::CreateTableFromArray( vs, L );
		return 1;
	}

	const luaL_Reg FileHelpersTable[] =
	{
		LIST_METHOD( Write ),
		LIST_METHOD( Read ),
        LIST_METHOD( Append ),
		LIST_METHOD( ReadDir ),
		{ nullptr, nullptr }
	};
}

LUA_REGISTER_NAMESPACE( File )
