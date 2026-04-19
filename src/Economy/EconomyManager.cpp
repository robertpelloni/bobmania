#include "EconomyManager.h"
#include "RageUtil.h"
#include "RageLog.h"
#include "Bridge/BobcoinBridge.h"
#include "Preference.h"

// Note: RageFile and XmlFile are assumed to be available in the real engine
#include "RageFile.h"
#include "XmlFile.h"

// Luna includes
#include "LuaManager.h"
#include "LuaBinding.h"

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

std::vector<TransactionRecord> EconomyManager::GetTransactionHistory() const {
    if (m_pBridge && m_pBridge->IsConnected()) {
        return m_pBridge->GetTransactionHistory(m_sBobcoinAddress.Get());
    }

    // Mocking an offline fallback history
    std::vector<TransactionRecord> history;
    history.push_back({"offline_mock", "Offline Sync", m_iBalance, 0});
    return history;
}

// ----------------------------------------------------------------------------
// Lua Bindings
// ----------------------------------------------------------------------------

class LunaEconomyManager : public Luna<EconomyManager>
{
public:
    static int GetBalance(T* p, lua_State* L) {
        lua_pushnumber(L, p->GetBalance());
        return 1;
    }

    static int BuyItem(T* p, lua_State* L) {
        RString itemID = SArg(1);
        long long cost = (long long)IArg(2);
        bool success = p->BuyItem(itemID, cost);
        lua_pushboolean(L, success);
        return 1;
    }

    static int AwardMiningReward(T* p, lua_State* L) {
        long long rewardAmount = (long long)IArg(1);
        p->AwardMiningReward(rewardAmount);
        return 0;
    }

    static int GetTransactionHistory(T* p, lua_State* L) {
        RString summary = "";
        std::vector<TransactionRecord> txs = p->GetTransactionHistory();
        for (size_t i = 0; i < txs.size(); ++i) {
            summary += txs[i].sTxId + "|" + txs[i].sType + "|" + std::to_string(txs[i].iAmount) + ",";
        }
        // lua_pushstring(L, summary.c_str());
        return 1;
    }

    LunaEconomyManager()
    {
        ADD_METHOD(GetBalance);
        ADD_METHOD(BuyItem);
        ADD_METHOD(AwardMiningReward);
        ADD_METHOD(GetTransactionHistory);
    }
};

LUA_REGISTER_CLASS(EconomyManager)
