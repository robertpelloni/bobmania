#include "global.h"
#include "AssetSyncManager.h"
#include "Economy/EconomyManager.h" // Relative include from src/
#include "RageLog.h"
#include "RageUtil.h"

AssetSyncManager* AssetSyncManager::s_pInstance = NULL;

AssetSyncManager* AssetSyncManager::Instance()
{
	if( !s_pInstance )
		s_pInstance = new AssetSyncManager;
	return s_pInstance;
}

void AssetSyncManager::Destroy()
{
	delete s_pInstance;
	s_pInstance = NULL;
}

AssetSyncManager::AssetSyncManager()
{
}

AssetSyncManager::~AssetSyncManager()
{
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
