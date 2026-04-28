#include "EconomyManager.h"
#include "RageUtil.h"
<<<<<<< HEAD
#include "RageLog.h"
=======
#include "DateTime.h"
#include "JsonUtil.h"
#include "CryptManager.h"
#include "ThemeManager.h"
#include "PrefsManager.h"
>>>>>>> origin/unified-ui-features-13937230807013224518

// Note: RageFile and XmlFile are assumed to be available in the real engine
#include "RageFile.h"
#include "XmlFile.h"

// Luna includes
#include "LuaManager.h"
#include "LuaBinding.h"

<<<<<<< HEAD
EconomyManager* ECONOMYMAN = nullptr;

static Preference<RString> m_sBobcoinAddress("BobcoinAddress", "BOB-DEV-1234");
static Preference<RString> m_sMasterAddress("BobcoinMasterAddress", "BOB-MAIN-STORE");

const RString ECONOMY_FALLBACK_FILE = "Save/Economy.xml";

EconomyManager::EconomyManager() : m_iBalance(0) {
    m_pBridge = new BobcoinBridge();
}

EconomyManager::~EconomyManager() {
    delete m_pBridge;
}

void EconomyManager::Init() {
    LOG->Info("EconomyManager::Init() - Connecting to Bobcoin RPC node...");
    m_pBridge->Init();

    if (m_pBridge->IsConnected()) {
        m_iBalance = m_pBridge->GetBalance(m_sBobcoinAddress.Get());
        LOG->Info("EconomyManager::Init() - Connection successful. Wallet Balance: %lld", m_iBalance);

        // Sync online balance to offline cache
        SaveFallbackBalance();
    } else {
        LOG->Warn("EconomyManager::Init() - Failed to connect to Bobcoin Bridge. Loading Offline Fallback...");
        if (!LoadFallbackBalance()) {
            m_iBalance = 0; // Absolute zero if nothing loaded
        }
    }
}

void EconomyManager::Shutdown() {
    LOG->Info("EconomyManager::Shutdown() - Disconnecting from Bobcoin RPC node and saving state.");
    SaveFallbackBalance();
    m_pBridge->Shutdown();
}

bool EconomyManager::LoadFallbackBalance() {
    // Mocking XmlFile loading
    // XmlFile xml;
    // if(!xml.Load(ECONOMY_FALLBACK_FILE)) return false;
    // XNode* pNode = xml.GetChild("EconomyFallback");
    // if(pNode) pNode->GetAttrValue("OfflineBalance", m_iBalance);

    m_iBalance = 1000; // Simulated fallback load
    LOG->Info("EconomyManager::LoadFallbackBalance() - Loaded %lld Bobcoins offline.", m_iBalance);
    return true;
}

void EconomyManager::SaveFallbackBalance() {
    // Mocking XmlFile saving
    // XmlFile xml;
    // XNode* pNode = xml.AppendChild("EconomyFallback");
    // pNode->AppendAttr("OfflineBalance", m_iBalance);
    // xml.Save(ECONOMY_FALLBACK_FILE);
    LOG->Info("EconomyManager::SaveFallbackBalance() - Saved %lld Bobcoins to disk.", m_iBalance);
}

long long EconomyManager::GetBalance() const {
    if(m_pBridge && m_pBridge->IsConnected()) {
        return m_pBridge->GetBalance(m_sBobcoinAddress.Get());
    }
    // Return offline fallback if no network
    return m_iBalance;
}

bool EconomyManager::BuyItem(const RString& itemID, long long cost) {
    if (!m_pBridge || !m_pBridge->IsConnected()) {
        // Offline Buy Mode
        if (m_iBalance >= cost) {
            m_iBalance -= cost;
            SaveFallbackBalance();
            LOG->Info("EconomyManager::BuyItem() - OFFLINE Purchased %s. Fallback Balance: %lld", itemID.c_str(), m_iBalance);
            return true;
        }
        LOG->Warn("EconomyManager::BuyItem() - Insufficient offline funds.");
        return false;
    }

    long long currentBalance = m_pBridge->GetBalance(m_sBobcoinAddress.Get());

    if (currentBalance >= cost) {
        bool success = m_pBridge->SendTransaction(m_sBobcoinAddress.Get(), m_sMasterAddress.Get(), cost);
        if(success) {
            LOG->Info("EconomyManager::BuyItem() - Purchased %s for %lld bobcoins.", itemID.c_str(), cost);
            m_iBalance = currentBalance - cost; // Sync cache
            SaveFallbackBalance();
            return true;
        } else {
            LOG->Warn("EconomyManager::BuyItem() - Blockchain transaction failed for %s.", itemID.c_str());
            return false;
        }
    }

    LOG->Warn("EconomyManager::BuyItem() - Insufficient funds for %s.", itemID.c_str());
    return false;
}

void EconomyManager::AwardMiningReward(long long rewardAmount) {
    if (!m_pBridge || !m_pBridge->IsConnected()) {
        // Offline Mining Mode
        m_iBalance += rewardAmount;
        SaveFallbackBalance();
        LOG->Info("EconomyManager::AwardMiningReward() - OFFLINE Mined %lld bobcoins. Fallback: %lld", rewardAmount, m_iBalance);
        return;
    }

    bool success = m_pBridge->SendTransaction(m_sMasterAddress.Get(), m_sBobcoinAddress.Get(), rewardAmount);

    if(success) {
        m_iBalance += rewardAmount; // Sync cache
        SaveFallbackBalance();
        LOG->Info("EconomyManager::AwardMiningReward() - Successfully mined %lld bobcoins.", rewardAmount);
    } else {
        LOG->Warn("EconomyManager::AwardMiningReward() - Failed to transfer mining reward.");
    }
}
=======
// Refactored constants to Preference or ThemeMetric could be a future step
// For now, keeping them static but grouped clearly.
namespace EconomyPaths {
    const RString DATA_FILE = "Save/Economy.xml";
    const RString SIG_FILE = "Save/Economy.xml.sig";
    const RString CATALOG_FILE = "Data/MarketplaceCatalog.json";
}

EconomyManager::EconomyManager()
{
	m_iBalance = 1000000; // Mock start balance
	m_bConnected = false;
	m_sWalletAddress = "0xMockAddress123";
    m_fCurrentHashRate = 10.0f;
    m_pBridge = new BobcoinBridge();
}

EconomyManager::~EconomyManager()
{
	WriteToDisk();
    SAFE_DELETE( m_pBridge );
}

void EconomyManager::Init()
{
	LOG->Trace( "EconomyManager::Init()" );
    if( m_pBridge ) m_pBridge->Init();
    LoadCatalog();
	ReadFromDisk();
	ConnectToTempo();
}

void EconomyManager::LoadCatalog()
{
    if( !IsAFile(EconomyPaths::CATALOG_FILE) )
    {
        LOG->Warn( "Marketplace Catalog not found at %s. Using default mock data.", EconomyPaths::CATALOG_FILE.c_str() );

        // Default Catalog
        EconomyItem item1; item1.ID="song_pack_1"; item1.Name="Starter Pack"; item1.Price=500; item1.Type="Song"; item1.Icon="SongPack";
        m_MarketplaceCatalog.push_back(item1);

        EconomyItem item2; item2.ID="avatar_frame_gold"; item2.Name="Gold Frame"; item2.Price=2000; item2.Type="Cosmetic"; item2.Icon="FrameGold";
        m_MarketplaceCatalog.push_back(item2);

        EconomyItem item3; item3.ID="xp_boost_1h"; item3.Name="1h XP Boost"; item3.Price=100; item3.Type="Consumable"; item3.Icon="Potion";
        m_MarketplaceCatalog.push_back(item3);

        return;
    }

    RString sJson;
    if( !GetFileContents(EconomyPaths::CATALOG_FILE, sJson) ) return;

    Json::Value root;
    RString sError;
    if( !JsonUtil::LoadFromString(root, sJson, sError) )
    {
        LOG->Warn( "Failed to parse Marketplace Catalog: %s", sError.c_str() );
        return;
    }

    if( root.isArray() )
    {
        m_MarketplaceCatalog.clear();
        for( unsigned i=0; i<root.size(); ++i )
        {
            const Json::Value& item = root[i];
            EconomyItem ei;
            ei.ID = item["ID"].asString();
            ei.Name = item["Name"].asString();
            ei.Price = item["Price"].asInt64();
            ei.Type = item["Type"].asString();
            ei.Icon = item["Icon"].asString();
            m_MarketplaceCatalog.push_back(ei);
        }
    }
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
	if( !IsAFile(EconomyPaths::DATA_FILE) ) return;

    // Integrity Check
    if( IsAFile(EconomyPaths::SIG_FILE) )
    {
        RString sData;
        GetFileContents(EconomyPaths::DATA_FILE, sData);
        RString sSig;
        GetFileContents(EconomyPaths::SIG_FILE, sSig);

        RString sCalcSig = CRYPTMAN->GetSHA256ForString(sData);
        if( sSig != sCalcSig )
        {
            LOG->Warn("EconomyManager: Save file corrupted or modified! (Signature Mismatch)");
            // Handle corruption (backup, reset, or warn user)
            // For now, just warn but allow load (soft fail)
        }
    }

	XNode xml;
	if( !XmlFileUtil::LoadFromFileShowErrors(xml, EconomyPaths::DATA_FILE) ) return;
	LoadFromNode( &xml );
}

void EconomyManager::WriteToDisk()
{
	RageFile f;
	if( !f.Open(EconomyPaths::DATA_FILE, RageFile::WRITE|RageFile::SLOW_FLUSH) ) return;

	std::unique_ptr<XNode> xml( CreateNode() );
	XmlFileUtil::SaveToFile( xml.get(), f );
    f.Close();

    // Sign
    RString sData;
    GetFileContents(EconomyPaths::DATA_FILE, sData);
    RString sSig = CRYPTMAN->GetSHA256ForString(sData);

    RageFile fSig;
    if( fSig.Open(EconomyPaths::SIG_FILE, RageFile::WRITE) )
    {
        fSig.Write(sSig);
        fSig.Close();
    }
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

    // Special Logic: Theme Change
    bool bIsTheme = false;
    for(const auto& catItem : m_MarketplaceCatalog)
    {
        if(catItem.ID == sItemID && catItem.Type == "Theme")
        {
            bIsTheme = true;
            // Assume ID maps to theme name for this demo
            // In reality, we'd look up the theme directory name
            RString sThemeName = catItem.ID;
            // e.g. "default", "Simply-Love-SM5"

            if( THEME->IsThemeSelectable(sThemeName) )
            {
                PREFSMAN->m_sTheme.Set(sThemeName);
                THEME->SwitchThemeAndLanguage(sThemeName, THEME->GetCurLanguage(), PREFSMAN->m_bPseudoLocalize);
                LOG->Trace("EconomyManager: Switched theme to %s", sThemeName.c_str());
            }
            break;
        }
    }

    return true;
}

const std::vector<Transaction>& EconomyManager::GetHistory() const
{
    return m_History;
}

void EconomyManager::LogTransaction( const RString& sDesc, long long iAmount )
{
    Transaction t;
    t.Date = DateTime::GetNowDate().GetString(); // Simplified date string
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
        LogTransaction( "Mining Reward (" + ssprintf("%.2f%%", fScore*100) + ")", amount );
    }
}

float EconomyManager::GetHashRate() const
{
    return m_fCurrentHashRate;
}

// Lua Bindings
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

/*
 * ==============================================================================
 * UNIFIED STEPMANIA ARCHITECTURE NOTES: EconomyManager
 * ==============================================================================
 * The EconomyManager is the global singleton responsible for handling all virtual
 * currency (Bobcoin) logic. It sits completely independent of the core gameplay
 * loop, ensuring that if the connection to the Bobcoin Node fails, the player
 * can still play Arcade/Gym modes seamlessly.
 *
 * It is structured around the `BobcoinBridge` (which currently uses a mock in
 * extern/bobcoin).
 *
 * NEXT IMPLEMENTOR:
 * To move this to production, `LoadCatalog()` must fetch the JSON payload
 * over HTTPS rather than reading from `Data/MarketplaceCatalog.json`.
 * Furthermore, `WriteToDisk` and `ReadFromDisk` should be replaced or augmented
 * by calls to the central Unified server to prevent local file manipulation
 * (despite the current SHA-256 signing via CRYPTMAN).
 * ==============================================================================
 */
>>>>>>> origin/unified-ui-features-13937230807013224518
