#include "global.h"
#include "Network/SpectatorManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "LuaManager.h"

SpectatorManager *SPECTATORMAN = nullptr;
SpectatorManager *SpectatorManager::m_pInstance = nullptr;

SpectatorManager::SpectatorManager()
{
	m_pInstance = this;
	m_bConnected = false;
	m_bBroadcasting = false;
}

SpectatorManager::~SpectatorManager()
{
	m_pInstance = nullptr;
}

void SpectatorManager::ConnectToMatch(const RString& sMatchID)
{
	m_sCurrentMatchID = sMatchID;
	m_bConnected = true;
	LOG->Trace("SpectatorManager: Connected to match %s", sMatchID.c_str());
}

void SpectatorManager::StartBroadcasting(const RString& sStreamKey)
{
	m_bBroadcasting = true;
	LOG->Trace("SpectatorManager: Started broadcasting with key %s", sStreamKey.c_str());
}

// Lua Bindings
class LunaSpectatorManager: public Luna<SpectatorManager>
{
public:
	static int ConnectToMatch(T* p, lua_State *L)
	{
		p->ConnectToMatch(SArg(1));
		return 0;
	}
	static int StartBroadcasting(T* p, lua_State *L)
	{
		p->StartBroadcasting(SArg(1));
		return 0;
	}
	static int IsConnected(T* p, lua_State *L)
	{
		lua_pushboolean(L, p->IsConnected());
		return 1;
	}
	static int IsBroadcasting(T* p, lua_State *L)
	{
		lua_pushboolean(L, p->IsBroadcasting());
		return 1;
	}

	LunaSpectatorManager()
	{
		ADD_METHOD( ConnectToMatch );
		ADD_METHOD( StartBroadcasting );
		ADD_METHOD( IsConnected );
		ADD_METHOD( IsBroadcasting );
	}
};

LUA_REGISTER_CLASS( SpectatorManager )
