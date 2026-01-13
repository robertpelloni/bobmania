#include "global.h"
#include "GrooveStatsManager.h"
#include "RageLog.h"
#include "LuaBinding.h"

GrooveStatsManager*	GROOVESTATSMAN = nullptr;

GrooveStatsManager::GrooveStatsManager()
{
	m_bConnected = false;
	m_sApiKey = "MockApiKey";
}

GrooveStatsManager::~GrooveStatsManager()
{
}

void GrooveStatsManager::Init()
{
	LOG->Trace( "GrooveStatsManager::Init()" );
	m_bConnected = true;
}

void GrooveStatsManager::Update( float fDeltaTime )
{
}

void GrooveStatsManager::SubmitScore( const RString& sChartKey, int iScore )
{
	LOG->Info( "GrooveStats: Submitting score %d for chart %s...", iScore, sChartKey.c_str() );
	// In a real implementation, this would make an HTTP POST request.
}

void GrooveStatsManager::RequestLeaderboard( const RString& sChartKey )
{
	LOG->Info( "GrooveStats: Requesting leaderboard for chart %s...", sChartKey.c_str() );
	// In a real implementation, this would make an HTTP GET request and broadcast a message on completion.
}

// Lua
class LunaGrooveStatsManager: public Luna<GrooveStatsManager>
{
public:
	static int SubmitScore( T* p, lua_State *L )
	{
		p->SubmitScore( SArg(1), IArg(2) );
		return 0;
	}
	static int RequestLeaderboard( T* p, lua_State *L )
	{
		p->RequestLeaderboard( SArg(1) );
		return 0;
	}
	static int IsConnected( T* p, lua_State *L )
	{
		lua_pushboolean( L, true ); // Mock
		return 1;
	}

	LunaGrooveStatsManager()
	{
		ADD_METHOD( SubmitScore );
		ADD_METHOD( RequestLeaderboard );
		ADD_METHOD( IsConnected );
	}
};

LUA_REGISTER_CLASS( GrooveStatsManager )
