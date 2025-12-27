#ifndef ASSET_SYNC_MANAGER_H
#define ASSET_SYNC_MANAGER_H

#include <string>
#include <vector>

class AssetSyncManager
{
public:
	static AssetSyncManager* Instance();
	static void Destroy();

	AssetSyncManager();
	~AssetSyncManager();

	// Simulates connecting to an external game's API to fetch owned items
	// Returns the number of items synced
	int SyncWithGame( const std::string& gameName );

	// Returns a list of supported external games
	std::vector<std::string> GetSupportedGames() const;

private:
	static AssetSyncManager* s_pInstance;
};

#endif
