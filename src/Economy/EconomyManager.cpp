#include "global.h"
#include "EconomyManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "XmlFile.h"
#include "XmlFileUtil.h"
#include "RageFile.h"
#include "RageUtil.h"

EconomyManager*	ECONOMYMAN = nullptr;

static const RString ECONOMY_DAT = "Save/Economy.xml";

EconomyManager::EconomyManager()
{
	m_iBalance = 1000000; // Mock start balance
	m_bConnected = false;
	m_sWalletAddress = "0xMockAddress123";
}

EconomyManager::~EconomyManager()
{
	WriteToDisk();
}

void EconomyManager::Init()
{
	LOG->Trace( "EconomyManager::Init()" );
	ReadFromDisk();
	ConnectToTempo();
}

void EconomyManager::LoadFromNode( const XNode *pNode )
{
	if( pNode->GetName() != "Economy" )
	{
		LOG->Warn( "Error loading economy: unexpected \"%s\"", pNode->GetName().c_str() );
		return;
	}

	pNode->GetChildValue( "Balance", m_iBalance );
	pNode->GetChildValue( "WalletAddress", m_sWalletAddress );
}

XNode* EconomyManager::CreateNode() const
{
	XNode *xml = new XNode( "Economy" );
	xml->AppendChild( "Balance", (double)m_iBalance ); // Serialize as double to handle large ints in XML if needed, or text
	xml->AppendChild( "WalletAddress", m_sWalletAddress );
	return xml;
}

void EconomyManager::ReadFromDisk()
{
	if( !IsAFile(ECONOMY_DAT) )
		return;

	XNode xml;
	if( !XmlFileUtil::LoadFromFileShowErrors(xml, ECONOMY_DAT) )
		return;

	LoadFromNode( &xml );
}

void EconomyManager::WriteToDisk()
{
	RageFile f;
	if( !f.Open(ECONOMY_DAT, RageFile::WRITE|RageFile::SLOW_FLUSH) )
	{
		LOG->Warn( "Couldn't open file \"%s\" for writing: %s", ECONOMY_DAT.c_str(), f.GetError().c_str() );
		return;
	}

	std::unique_ptr<XNode> xml( CreateNode() );
	XmlFileUtil::SaveToFile( xml.get(), f );
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
