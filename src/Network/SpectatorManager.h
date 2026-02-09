#ifndef SPECTATOR_MANAGER_H
#define SPECTATOR_MANAGER_H

#include "RageUtil.h"

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

	// Lua
	void PushSelf(lua_State *L);

private:
	static SpectatorManager *m_pInstance;
	bool m_bConnected;
	bool m_bBroadcasting;
	RString m_sCurrentMatchID;
};

extern SpectatorManager *SPECTATORMAN;

#endif
