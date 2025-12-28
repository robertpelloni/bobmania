#include "global.h"
#include "SpectatorManager.h"
#include "Network/GameClient.h" // Send data via GameClient
#include "RageLog.h"

SpectatorManager* SpectatorManager::s_pInstance = NULL;

SpectatorManager* SpectatorManager::Instance()
{
	if( !s_pInstance )
		s_pInstance = new SpectatorManager;
	return s_pInstance;
}

void SpectatorManager::Destroy()
{
	delete s_pInstance;
	s_pInstance = NULL;
}

SpectatorManager::SpectatorManager()
{
	m_bIsBroadcasting = false;
	m_bIsWatching = false;
}

SpectatorManager::~SpectatorManager()
{
}

void SpectatorManager::StartBroadcasting()
{
	m_bIsBroadcasting = true;
	LOG->Info("SpectatorManager: Started Broadcasting Match Data.");
	// In real impl, we would send a handshake to the server
}

void SpectatorManager::StopBroadcasting()
{
	m_bIsBroadcasting = false;
	LOG->Info("SpectatorManager: Stopped Broadcasting.");
}

void SpectatorManager::BroadcastUpdate( float fDeltaTime, float fLife, long long iScore )
{
	if( !m_bIsBroadcasting ) return;

	// Stub: Send packet via GameClient
	// GameClient::Instance()->SendPacket(...)
}

void SpectatorManager::BroadcastNoteHit( int col, int tapScore )
{
	if( !m_bIsBroadcasting ) return;

	SpectatorPacket pkt;
	pkt.timestamp = 0; // GetTime();
	pkt.column = col;
	pkt.tapScore = tapScore;

	// LOG->Trace("Broadcasting Hit: Col %d Score %d", col, tapScore);
}

void SpectatorManager::ConnectToMatch( const std::string& matchID )
{
	m_bIsWatching = true;
	m_CurrentMatchID = matchID;
	LOG->Info("SpectatorManager: Tuning into match %s", matchID.c_str());
	GameClient::Instance()->SpectateMatch(matchID);
}

void SpectatorManager::Disconnect()
{
	m_bIsWatching = false;
	m_CurrentMatchID = "";
}

std::vector<std::string> SpectatorManager::GetLiveMatches() const
{
	std::vector<std::string> matches;
	matches.push_back("Pro Finals: Chris vs FEFEMZ");
	matches.push_back("Gold Semis: HappyF33t vs DDR_Fan");
	matches.push_back("Casual: Alice playing Butterfly");
	return matches;
}
