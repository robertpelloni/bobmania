#include "global.h"
#include "EconomyManager.h"
#include "RageLog.h"
#include "LuaBinding.h"

EconomyManager*	ECONOMYMAN = nullptr;

EconomyManager::EconomyManager()
{
	m_iBalance = 1000000; // Mock start balance
	m_bConnected = false;
	m_sWalletAddress = "0xMockAddress123";
}

EconomyManager::~EconomyManager()
{
}

void EconomyManager::Init()
{
	LOG->Trace( "EconomyManager::Init()" );
	ConnectToTempo();
}

void EconomyManager::Update( float fDeltaTime )
{
	// Simulate network activity?
}

void EconomyManager::ConnectToTempo()
{
	// Mock connection
	m_bConnected = true;
}

bool EconomyManager::IsConnected() const
{
	return m_bConnected;
}

RString EconomyManager::GetWalletAddress() const
{
	return m_sWalletAddress;
}

long long EconomyManager::GetBalance() const
{
	return m_iBalance;
}

bool EconomyManager::SendTip( const RString& sAddress, long long iAmount )
{
	if( iAmount <= 0 ) return false;
	if( m_iBalance < iAmount ) return false;

	m_iBalance -= iAmount;
	LOG->Trace( "Sent tip of %lld to %s. New Balance: %lld", iAmount, sAddress.c_str(), m_iBalance );
	return true;
}

// Lua
class LunaEconomyManager: public Luna<EconomyManager>
{
public:
	static int GetBalance( T* p, lua_State *L )
	{
		lua_pushnumber( L, (double)p->GetBalance() ); // Lua 5.1 uses double
		return 1;
	}
	static int GetWalletAddress( T* p, lua_State *L )
	{
		lua_pushstring( L, p->GetWalletAddress() );
		return 1;
	}
	static int SendTip( T* p, lua_State *L )
	{
		RString addr = SArg(1);
		long long amount = (long long)FArg(2); // Lua numbers are floats
		lua_pushboolean( L, p->SendTip(addr, amount) );
		return 1;
	}
	static int IsConnected( T* p, lua_State *L )
	{
		lua_pushboolean( L, p->IsConnected() );
		return 1;
	}

	LunaEconomyManager()
	{
		ADD_METHOD( GetBalance );
		ADD_METHOD( GetWalletAddress );
		ADD_METHOD( SendTip );
		ADD_METHOD( IsConnected );
	}
};

LUA_REGISTER_CLASS( EconomyManager )
