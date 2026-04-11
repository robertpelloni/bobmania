#ifndef GROOVESTATS_MANAGER_H
#define GROOVESTATS_MANAGER_H

#include "RageTypes.h"

struct lua_State;

class GrooveStatsManager
{
public:
	GrooveStatsManager();
	~GrooveStatsManager();

	void Init();
	void Update( float fDeltaTime );

	// Mocking submission
	void SubmitScore( const RString& sChartKey, int iScore );

	// Mocking leaderboard retrieval
	void RequestLeaderboard( const RString& sChartKey );

	// Lua
	void PushSelf( lua_State *L );

private:
	bool m_bConnected;
	RString m_sApiKey;
};

extern GrooveStatsManager*	GROOVESTATSMAN;

#endif
