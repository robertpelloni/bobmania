#include "global.h"
#include "EconomyManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "XmlFile.h"
#include "XmlFileUtil.h"
#include "RageFile.h"
#include "RageUtil.h"
#include "DateTime.h"

EconomyManager*	ECONOMYMAN = nullptr;

static const RString ECONOMY_DAT = "Save/Economy.xml";

EconomyManager::EconomyManager()
{
	m_iBalance = 1000000; // Mock start balance
	m_bConnected = false;
	m_sWalletAddress = "0xMockAddress123";
    m_fCurrentHashRate = 0.0f;

    // Initialize Mock Catalog
    m_MarketplaceCatalog.push_back({ "song_pack_1", "Classic Pack 1", 500, "Song", "Graphics/song_pack_icon.png" });
    m_MarketplaceCatalog.push_back({ "avatar_frame_gold", "Gold Frame", 200, "Item", "Graphics/item_icon.png" });
    m_MarketplaceCatalog.push_back({ "xp_boost_1h", "1h XP Boost", 100, "Boost", "Graphics/boost_icon.png" });
    m_MarketplaceCatalog.push_back({ "theme_dark", "Dark Mode Theme", 0, "Theme", "Graphics/theme_icon.png" });
    m_MarketplaceCatalog.push_back({ "bobcoin_miner", "Bobcoin Miner", 5000, "Hardware", "Graphics/miner_icon.png" });
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

	RString sBalance;
	pNode->GetChildValue( "Balance", sBalance );
	m_iBalance = StringToLLong( sBalance );
	pNode->GetChildValue( "WalletAddress", m_sWalletAddress );

    const XNode *pItems = pNode->GetChild( "OwnedItems" );
    if( pItems )
    {
        FOREACH_CONST_Child( pItems, item )
        {
            RString sID;
            item->GetAttrValue( "ID", sID );
            m_OwnedItems[sID] = true;
        }
    }

    const XNode *pHistory = pNode->GetChild( "History" );
    if( pHistory )
    {
        FOREACH_CONST_Child( pHistory, txn )
        {
            Transaction t;
            txn->GetAttrValue( "Date", t.Date );
            txn->GetAttrValue( "Desc", t.Description );
            RString sAmt;
            txn->GetAttrValue( "Amount", sAmt );
            t.Amount = StringToLLong(sAmt);
            m_History.push_back( t );
        }
    }
}

XNode* EconomyManager::CreateNode() const
{
	XNode *xml = new XNode( "Economy" );
	xml->AppendChild( "Balance", ssprintf("%lld", m_iBalance) );
	xml->AppendChild( "WalletAddress", m_sWalletAddress );

    XNode *pItems = xml->AppendChild( "OwnedItems" );
    for( std::map<RString, bool>::const_iterator it = m_OwnedItems.begin(); it != m_OwnedItems.end(); ++it )
    {
        if( it->second )
        {
            XNode *item = pItems->AppendChild( "Item" );
            item->AppendAttr( "ID", it->first );
        }
    }

    XNode *pHistory = xml->AppendChild( "History" );
    for( std::vector<Transaction>::const_iterator it = m_History.begin(); it != m_History.end(); ++it )
    {
        XNode *txn = pHistory->AppendChild( "Transaction" );
        txn->AppendAttr( "Date", it->Date );
        txn->AppendAttr( "Desc", it->Description );
        txn->AppendAttr( "Amount", ssprintf("%lld", it->Amount) );
    }

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
    LogTransaction( "Sent Tip to " + sAddress, -iAmount );
	LOG->Trace( "Sent tip of %lld to %s. New Balance: %lld", iAmount, sAddress.c_str(), m_iBalance );
	return true;
}

const std::vector<EconomyItem>& EconomyManager::GetMarketplaceItems() const
{
    return m_MarketplaceCatalog;
}

bool EconomyManager::HasItem( const RString& sItemID ) const
{
    std::map<RString, bool>::const_iterator it = m_OwnedItems.find( sItemID );
    return it != m_OwnedItems.end() && it->second;
}

bool EconomyManager::BuyItem( const RString& sItemID )
{
    if( HasItem( sItemID ) ) return false; // Already owned

    // Find price
    long long price = 0;
    bool found = false;
    for( const auto& item : m_MarketplaceCatalog )
    {
        if( item.ID == sItemID )
        {
            price = item.Price;
            found = true;
            break;
        }
    }

    if( !found ) return false;
    if( m_iBalance < price ) return false;

    m_iBalance -= price;
    m_OwnedItems[sItemID] = true;
    LogTransaction( "Purchased " + sItemID, -price );

    // If Miner purchased, increase hashrate (simulated)
    if( sItemID == "bobcoin_miner" ) m_fCurrentHashRate += 50.0f;

    return true;
}

const std::vector<Transaction>& EconomyManager::GetHistory() const
{
    return m_History;
}

void EconomyManager::LogTransaction( const RString& sDesc, long long iAmount )
{
    Transaction t;
    t.Date = DateTime::GetNowDate().GetString();
    t.Description = sDesc;
    t.Amount = iAmount;
    m_History.insert( m_History.begin(), t ); // Prepend for newest first

    // Limit history size
    if( m_History.size() > 50 ) m_History.resize( 50 );
}

void EconomyManager::Deposit( long long iAmount, const RString& sDesc )
{
    if( iAmount <= 0 ) return;
    m_iBalance += iAmount;
    LogTransaction( sDesc, iAmount );
}

void EconomyManager::AwardMiningReward( float fScore, float fDifficulty )
{
    // Basic formula: Score % * Difficulty * Base
    float reward = (fScore * fDifficulty * 10.0f) + m_fCurrentHashRate;
    long long amount = (long long)reward;
    if( amount > 0 )
    {
        m_iBalance += amount;
        LogTransaction( "Mining Reward (Score: " + ssprintf("%.2f%%", fScore*100) + ")", amount );
    }
}

float EconomyManager::GetHashRate() const
{
    return m_fCurrentHashRate;
}

// Lua
class LunaEconomyManager: public Luna<EconomyManager>
{
public:
	static int GetBalance( T* p, lua_State *L )
	{
		lua_pushnumber( L, (double)p->GetBalance() );
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
		long long amount = (long long)FArg(2);
		lua_pushboolean( L, p->SendTip(addr, amount) );
		return 1;
	}
	static int IsConnected( T* p, lua_State *L )
	{
		lua_pushboolean( L, p->IsConnected() );
		return 1;
	}
    static int BuyItem( T* p, lua_State *L )
    {
        RString id = SArg(1);
        lua_pushboolean( L, p->BuyItem(id) );
        return 1;
    }
    static int HasItem( T* p, lua_State *L )
    {
        RString id = SArg(1);
        lua_pushboolean( L, p->HasItem(id) );
        return 1;
    }
    static int GetMarketplaceItems( T* p, lua_State *L )
    {
        const std::vector<EconomyItem>& items = p->GetMarketplaceItems();
        lua_newtable(L);
        for( size_t i=0; i<items.size(); ++i )
        {
            lua_newtable(L);
            lua_pushstring(L, "ID"); lua_pushstring(L, items[i].ID); lua_settable(L, -3);
            lua_pushstring(L, "Name"); lua_pushstring(L, items[i].Name); lua_settable(L, -3);
            lua_pushstring(L, "Price"); lua_pushnumber(L, (double)items[i].Price); lua_settable(L, -3);
            lua_pushstring(L, "Type"); lua_pushstring(L, items[i].Type); lua_settable(L, -3);
            lua_pushstring(L, "Icon"); lua_pushstring(L, items[i].Icon); lua_settable(L, -3);
            lua_rawseti(L, -2, i+1);
        }
        return 1;
    }
    static int GetHistory( T* p, lua_State *L )
    {
        const std::vector<Transaction>& hist = p->GetHistory();
        lua_newtable(L);
        for( size_t i=0; i<hist.size(); ++i )
        {
            lua_newtable(L);
            lua_pushstring(L, "Date"); lua_pushstring(L, hist[i].Date); lua_settable(L, -3);
            lua_pushstring(L, "Description"); lua_pushstring(L, hist[i].Description); lua_settable(L, -3);
            lua_pushstring(L, "Amount"); lua_pushnumber(L, (double)hist[i].Amount); lua_settable(L, -3);
            lua_rawseti(L, -2, i+1);
        }
        return 1;
    }
    static int AwardMiningReward( T* p, lua_State *L )
    {
        float score = FArg(1);
        float diff = FArg(2);
        p->AwardMiningReward(score, diff);
        return 0;
    }
    static int GetHashRate( T* p, lua_State *L )
    {
        lua_pushnumber(L, p->GetHashRate());
        return 1;
    }

	LunaEconomyManager()
	{
		ADD_METHOD( GetBalance );
		ADD_METHOD( GetWalletAddress );
		ADD_METHOD( SendTip );
		ADD_METHOD( IsConnected );
        ADD_METHOD( BuyItem );
        ADD_METHOD( HasItem );
        ADD_METHOD( GetMarketplaceItems );
        ADD_METHOD( GetHistory );
        ADD_METHOD( AwardMiningReward );
        ADD_METHOD( GetHashRate );
	}
};

LUA_REGISTER_CLASS( EconomyManager )
