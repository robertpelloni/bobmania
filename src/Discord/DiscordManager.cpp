#include "global.h"
#include "DiscordManager.h"
#include "RageLog.h"
#include "LuaManager.h"

DiscordManager *DISCORD = nullptr;

DiscordManager::DiscordManager()
{
	m_bInitialized = false;
}

DiscordManager::~DiscordManager()
{
	Shutdown();
}

void DiscordManager::Init()
{
	if( m_bInitialized ) return;

	// Stub: In a real implementation, we would call Discord_Initialize here.
	// For now, we just log that we are ready to receive commands.
	LOG->Info( "DiscordManager: Initialized (Stub)" );
	m_bInitialized = true;
}

void DiscordManager::Shutdown()
{
	if( !m_bInitialized ) return;

	// Stub: Discord_Shutdown()
	LOG->Info( "DiscordManager: Shutdown" );
	m_bInitialized = false;
}

void DiscordManager::Update()
{
	// Stub: Discord_RunCallbacks()
}

void DiscordManager::SetPresence( const RString &sDetails, const RString &sState, const RString &sLargeImageKey, const RString &sLargeImageText )
{
	if( !m_bInitialized ) return;

	// Stub: Update presence
	// In the future, this would map to DiscordRichPresence struct
	LOG->Trace( "DiscordManager::SetPresence: %s | %s | %s", sDetails.c_str(), sState.c_str(), sLargeImageKey.c_str() );
}

// Lua Bindings
class LunaDiscordManager : public Luna<DiscordManager>
{
public:
	static int Initialize( T* p, lua_State *L )
	{
		p->Init();
		return 0;
	}

	static int Shutdown( T* p, lua_State *L )
	{
		p->Shutdown();
		return 0;
	}

	static int SetPresence( T* p, lua_State *L )
	{
		RString sDetails = SArg(1);
		RString sState = SArg(2);
		RString sLargeImageKey = SArg(3);
		RString sLargeImageText = SArg(4);
		p->SetPresence( sDetails, sState, sLargeImageKey, sLargeImageText );
		return 0;
	}

	LunaDiscordManager()
	{
		ADD_METHOD( Initialize );
		ADD_METHOD( Shutdown );
		ADD_METHOD( SetPresence );
	}
};

LUA_REGISTER_CLASS( DiscordManager )
