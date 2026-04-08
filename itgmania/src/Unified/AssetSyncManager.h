#ifndef ASSET_SYNC_MANAGER_H
#define ASSET_SYNC_MANAGER_H

<<<<<<< HEAD:itgmania/src/Unified/AssetSyncManager.h
#include <string>
#include <vector>
=======
#include "RageUtil.h"
#include "FileDownload.h"
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Unified/AssetSyncManager.h

struct lua_State;

class AssetSyncManager
{
public:
<<<<<<< HEAD:itgmania/src/Unified/AssetSyncManager.h
	static AssetSyncManager* Instance();
	static void Destroy();

	AssetSyncManager();
	~AssetSyncManager();

	// Simulates connecting to an external game's API to fetch owned items
	// Returns the number of items synced
	int SyncWithGame( const std::string& gameName );

	// Returns a list of supported external games
	std::vector<std::string> GetSupportedGames() const;

	// Lua
	void PushSelf( lua_State *L );

private:
	static AssetSyncManager* s_pInstance;
};

=======
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

>>>>>>> origin/unified-ui-features-13937230807013224518:src/Unified/AssetSyncManager.h
#endif
