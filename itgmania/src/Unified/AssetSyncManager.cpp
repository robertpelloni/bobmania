#include "global.h"
#include "AssetSyncManager.h"
<<<<<<< HEAD:itgmania/src/Unified/AssetSyncManager.cpp
#include "Economy/EconomyManager.h" // Relative include from src/
#include "RageLog.h"
#include "RageUtil.h"
#include "LuaBinding.h"
#include "LuaManager.h"

AssetSyncManager* AssetSyncManager::s_pInstance = NULL;

AssetSyncManager* AssetSyncManager::Instance()
{
	if( !s_pInstance )
	{
		s_pInstance = new AssetSyncManager;

		// Register with Lua
		Lua *L = LUA->Get();
		lua_pushstring( L, "ASSETSYNCMAN" );
		s_pInstance->PushSelf( L );
		lua_settable( L, LUA_GLOBALSINDEX );
		LUA->Release( L );
	}
	return s_pInstance;
}

void AssetSyncManager::Destroy()
{
	delete s_pInstance;
	s_pInstance = NULL;
}

AssetSyncManager::AssetSyncManager()
{
=======
#include "RageLog.h"
#include "LuaBinding.h"

AssetSyncManager* ASSETSYNCMAN = nullptr;

AssetSyncManager::AssetSyncManager()
{
    m_pDownloader = new FileDownload();
    m_bSyncing = false;
    m_fProgress = 0.0f;
    m_sStatus = "Idle";
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Unified/AssetSyncManager.cpp
}

AssetSyncManager::~AssetSyncManager()
{
<<<<<<< HEAD:itgmania/src/Unified/AssetSyncManager.cpp
}

std::vector<std::string> AssetSyncManager::GetSupportedGames() const
{
	std::vector<std::string> games;
	games.push_back("Bob's Game");
	games.push_back("Etterna Online");
	games.push_back("StepManiaX");
	return games;
}

int AssetSyncManager::SyncWithGame( const std::string& gameName )
{
	LOG->Info("Syncing assets with %s...", gameName.c_str());

	int count = 0;
	EconomyManager* pEco = EconomyManager::Instance();

	if( gameName == "Bob's Game" )
	{
		// Mock Data from "Bob's Game"
		if( !pEco->HasAsset("BG_Sword") ) {
			pEco->AddToInventory( { "bg_001", "BG_Sword", "Avatar", "WALLET_PLAYER", 500 } );
			count++;
		}
		if( !pEco->HasAsset("BG_Shield") ) {
			pEco->AddToInventory( { "bg_002", "BG_Shield", "Avatar", "WALLET_PLAYER", 300 } );
			count++;
		}
	}
	else if( gameName == "Etterna Online" )
	{
		if( !pEco->HasAsset("Etterna_Skill_Badge") ) {
			pEco->AddToInventory( { "eo_001", "Etterna_Skill_Badge", "Title", "WALLET_PLAYER", 1000 } );
			count++;
		}
	}

	LOG->Info("Synced %d new assets.", count);
	return count;
}

=======
    SAFE_DELETE( m_pDownloader );
}

void AssetSyncManager::Init()
{
    LOG->Trace("AssetSyncManager::Init()");
}

void AssetSyncManager::StartSync()
{
    PullFromCloud();
}

void AssetSyncManager::PushToCloud()
{
    if( m_bSyncing ) return;
    m_bSyncing = true;
    m_sStatus = "Pushing to Cloud...";
    m_fProgress = 0.0f;

    // Stub: Upload local save files to endpoint
    m_pDownloader->StartUpload("http://127.0.0.1:8080/api/push", "Save/Economy.xml", "Economy.xml");
}

void AssetSyncManager::PullFromCloud()
{
    if( m_bSyncing ) return;
    m_bSyncing = true;
    m_sStatus = "Pulling from Cloud...";
    m_fProgress = 0.0f;

    // Stub: Download latest save file
    m_pDownloader->StartDownload("http://127.0.0.1:8080/api/pull", "Save/Downloads/Economy_sync.xml");
}

void AssetSyncManager::Update( float fDeltaTime )
{
    if( !m_bSyncing ) return;

    m_pDownloader->Update(fDeltaTime);

    // Simulate progress if actual download doesn't report well (or is fast)
    if( m_pDownloader->IsFinished() )
    {
        m_bSyncing = false;
        m_fProgress = 1.0f;
        if( m_pDownloader->GetResponseCode() == 200 )
            m_sStatus = "Complete";
        else
            m_sStatus = ssprintf("Failed: %d", m_pDownloader->GetResponseCode());
    }
    else
    {
        m_sStatus = m_pDownloader->GetStatus();
        // m_fProgress = m_pDownloader->GetProgress(); // FileDownload might not expose this directly in headers
        // Mock progress for visual feedback
        m_fProgress += fDeltaTime * 0.1f;
        if( m_fProgress > 0.9f ) m_fProgress = 0.9f;
    }
}

bool AssetSyncManager::IsSyncing() const { return m_bSyncing; }
float AssetSyncManager::GetProgress() const { return m_fProgress; }
RString AssetSyncManager::GetStatus() const { return m_sStatus; }

>>>>>>> origin/unified-ui-features-13937230807013224518:src/Unified/AssetSyncManager.cpp
// Lua Bindings
class LunaAssetSyncManager: public Luna<AssetSyncManager>
{
public:
<<<<<<< HEAD:itgmania/src/Unified/AssetSyncManager.cpp
	static int SyncWithGame( T* p, lua_State *L )
	{
		RString gameName = SArg(1);
		lua_pushnumber( L, p->SyncWithGame(gameName) );
		return 1;
	}

	LunaAssetSyncManager()
	{
		ADD_METHOD( SyncWithGame );
	}
=======
    static int StartSync( T* p, lua_State *L )
    {
        p->StartSync();
        return 0;
    }
    static int PushToCloud( T* p, lua_State *L )
    {
        p->PushToCloud();
        return 0;
    }
    static int PullFromCloud( T* p, lua_State *L )
    {
        p->PullFromCloud();
        return 0;
    }
    static int GetProgress( T* p, lua_State *L )
    {
        lua_pushnumber(L, p->GetProgress());
        return 1;
    }
    static int GetStatus( T* p, lua_State *L )
    {
        lua_pushstring(L, p->GetStatus());
        return 1;
    }

    LunaAssetSyncManager()
    {
        ADD_METHOD( StartSync );
        ADD_METHOD( PushToCloud );
        ADD_METHOD( PullFromCloud );
        ADD_METHOD( GetProgress );
        ADD_METHOD( GetStatus );
    }
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Unified/AssetSyncManager.cpp
};

LUA_REGISTER_CLASS( AssetSyncManager )
