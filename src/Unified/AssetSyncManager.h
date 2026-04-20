#ifndef ASSET_SYNC_MANAGER_H
#define ASSET_SYNC_MANAGER_H

#include "RageUtil.h"
#include "FileDownload.h"

struct lua_State;

class AssetSyncManager
{
public:
    AssetSyncManager();
    ~AssetSyncManager();

    void Init();
    void Update( float fDeltaTime );

    void StartSync();
    void PushToCloud();
    void PullFromCloud();
    bool IsSyncing() const;
    float GetProgress() const;
    RString GetStatus() const;

    // Lua
    void PushSelf( lua_State *L );

private:
    FileDownload* m_pDownloader;
    bool m_bSyncing;
    RString m_sStatus;
    float m_fProgress;
};

extern AssetSyncManager* ASSETSYNCMAN;

#endif
