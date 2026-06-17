#include "global.h"
#include "AssetSyncManager.h"
#include "RageLog.h"
#include "LuaBinding.h"

AssetSyncManager* ASSETSYNCMAN = nullptr;

AssetSyncManager::AssetSyncManager()
{
    m_pDownloader = std::make_unique<FileDownload>();
    m_bSyncing = false;
    m_fProgress = 0.0f;
    m_sStatus = "Idle";
}

AssetSyncManager::~AssetSyncManager()
{
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

// Lua Bindings
class LunaAssetSyncManager: public Luna<AssetSyncManager>
{
public:
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
};

LUA_REGISTER_CLASS( AssetSyncManager )
