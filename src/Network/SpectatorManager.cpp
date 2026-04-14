#include "global.h"
#include "Network/SpectatorManager.h"
#include "Network/UnifiedNetwork.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "LuaManager.h"
#include "json/json.h"

SpectatorManager *SPECTATORMAN = nullptr;
SpectatorManager *SpectatorManager::m_pInstance = nullptr;

SpectatorManager::SpectatorManager()
{
	m_pInstance = this;
	m_bConnected = false;
	m_bBroadcasting = false;

    if( UNIFIED_NET )
    {
        UNIFIED_NET->RegisterCallback("spectate_status", std::bind(&SpectatorManager::HandleSpectateStatus, this, std::placeholders::_1));
    }
}

SpectatorManager::~SpectatorManager()
{
	m_pInstance = nullptr;
}

void SpectatorManager::ConnectToMatch(const RString& sMatchID)
{
	m_sCurrentMatchID = sMatchID;
	m_bConnected = true;

    if( UNIFIED_NET && UNIFIED_NET->IsConnected() )
    {
        Json::Value payload;
        payload["match_id"] = sMatchID;
        UNIFIED_NET->SendJSON("spectate_request", payload);
    }

	LOG->Trace("SpectatorManager: Connected to match %s", sMatchID.c_str());
}

void SpectatorManager::HandleSpectateStatus(const Json::Value& payload)
{
    RString matchID = payload["match_id"].asString();
    RString status = payload["status"].asString();
    int viewers = payload["viewers"].asInt();

    LOG->Trace("SpectatorManager: Spectating %s. Status: %s. Viewers: %d", matchID.c_str(), status.c_str(), viewers);

    if( status == "connected" )
    {
        SCREENMAN->SystemMessage( ssprintf("Joined Stream! %d Watching", viewers) );
        m_bConnected = true;
    }
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
