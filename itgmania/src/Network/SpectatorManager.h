#ifndef SPECTATOR_MANAGER_H
#define SPECTATOR_MANAGER_H

<<<<<<< HEAD:itgmania/src/Network/SpectatorManager.h
#include <string>
#include <vector>
#include "RageUtil.h"

struct lua_State;

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

	// Lua
	void PushSelf( lua_State *L );

private:
	static SpectatorManager* s_pInstance;

	bool m_bIsBroadcasting;
	bool m_bIsWatching;
	std::string m_CurrentMatchID;

	std::vector<SpectatorPacket> m_ReplayBuffer;
};

=======
#include "RageUtil.h"
#include "json/json.h"

struct lua_State;

class SpectatorManager
{
public:
	SpectatorManager();
	~SpectatorManager();

	// Singleton instance
	static SpectatorManager *Instance() { return m_pInstance; }

	// API
	void ConnectToMatch(const RString& sMatchID);
	void StartBroadcasting(const RString& sStreamKey);
	bool IsConnected() const { return m_bConnected; }
	bool IsBroadcasting() const { return m_bBroadcasting; }
	RString GetCurrentMatchID() const { return m_sCurrentMatchID; }

    // Callbacks
    void HandleSpectateStatus(const Json::Value& payload);

	// Lua
	void PushSelf(lua_State *L);

private:
	static SpectatorManager *m_pInstance;
	bool m_bConnected;
	bool m_bBroadcasting;
	RString m_sCurrentMatchID;
};

extern SpectatorManager *SPECTATORMAN;

>>>>>>> origin/unified-ui-features-13937230807013224518:src/Network/SpectatorManager.h
#endif
