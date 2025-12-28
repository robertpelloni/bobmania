#ifndef SPECTATOR_MANAGER_H
#define SPECTATOR_MANAGER_H

#include <string>
#include <vector>
#include "RageUtil.h"

struct SpectatorPacket {
	float timestamp;
	int column;
	int tapScore; // TNS_W1, etc.
	float life;
	long long score;
};

class SpectatorManager
{
public:
	static SpectatorManager* Instance();
	static void Destroy();

	SpectatorManager();
	~SpectatorManager();

	// Broadcasting (Active Player)
	void StartBroadcasting();
	void StopBroadcasting();
	void BroadcastUpdate( float fDeltaTime, float fLife, long long iScore );
	void BroadcastNoteHit( int col, int tapScore );

	// Watching (Passive Viewer)
	void ConnectToMatch( const std::string& matchID );
	void Disconnect();
	bool IsWatching() const { return m_bIsWatching; }

	// Mock Data for UI
	std::vector<std::string> GetLiveMatches() const;

private:
	static SpectatorManager* s_pInstance;

	bool m_bIsBroadcasting;
	bool m_bIsWatching;
	std::string m_CurrentMatchID;

	std::vector<SpectatorPacket> m_ReplayBuffer;
};

#endif
