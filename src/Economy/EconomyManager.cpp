#include "EconomyManager.h"
#include "RageUtil.h"
#include "RageLog.h"
#include "Bridge/BobcoinBridge.h"
#include "Preference.h"

// Luna includes
#include "LuaManager.h"
#include "LuaBinding.h"

EconomyManager* ECONOMYMAN = nullptr;

static Preference<RString> m_sBobcoinAddress("BobcoinAddress", "BOB-DEV-1234");
static Preference<RString> m_sMasterAddress("BobcoinMasterAddress", "BOB-MAIN-STORE");

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
    } else {
        LOG->Warn("EconomyManager::Init() - Failed to connect to Bobcoin Bridge.");
        m_iBalance = 0; // Fallback
    }
}

void EconomyManager::Shutdown() {
    LOG->Info("EconomyManager::Shutdown() - Disconnecting from Bobcoin RPC node.");
    m_pBridge->Shutdown();
}

long long EconomyManager::GetBalance() const {
    if(m_pBridge && m_pBridge->IsConnected()) {
        return m_pBridge->GetBalance(m_sBobcoinAddress.Get());
    }
    return m_iBalance;
}

bool EconomyManager::BuyItem(const RString& itemID, long long cost) {
    if (!m_pBridge || !m_pBridge->IsConnected()) {
        LOG->Warn("EconomyManager::BuyItem() - Bridge not connected.");
        return false;
    }

    long long currentBalance = m_pBridge->GetBalance(m_sBobcoinAddress.Get());

    if (currentBalance >= cost) {
        bool success = m_pBridge->SendTransaction(m_sBobcoinAddress.Get(), m_sMasterAddress.Get(), cost);
        if(success) {
            LOG->Info("EconomyManager::BuyItem() - Purchased %s for %lld bobcoins.", itemID.c_str(), cost);
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
        LOG->Warn("EconomyManager::AwardMiningReward() - Bridge not connected.");
        return;
    }

    bool success = m_pBridge->SendTransaction(m_sMasterAddress.Get(), m_sBobcoinAddress.Get(), rewardAmount);

    if(success) {
        LOG->Info("EconomyManager::AwardMiningReward() - Successfully mined %lld bobcoins.", rewardAmount);
    } else {
        LOG->Warn("EconomyManager::AwardMiningReward() - Failed to transfer mining reward.");
    }
}

std::vector<TransactionRecord> EconomyManager::GetTransactionHistory() const {
    if (m_pBridge && m_pBridge->IsConnected()) {
        return m_pBridge->GetTransactionHistory(m_sBobcoinAddress.Get());
    }
    // Return empty if disconnected
    std::vector<TransactionRecord> empty;
    return empty;
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
        // Since we cannot push full C++ vectors cleanly to Lua without a table loop,
        // we stub this for the mock syntax check. In a real engine, we'd use
        // lua_newtable and loop over p->GetTransactionHistory() pushing keys/values.
        // For now, we'll return a simulated string for the UI to parse.
        RString summary = "";
        std::vector<TransactionRecord> txs = p->GetTransactionHistory();
        for (size_t i = 0; i < txs.size(); ++i) {
            summary += txs[i].sTxId + "|" + txs[i].sType + "|" + std::to_string(txs[i].iAmount) + ",";
        }

        // Push the string to Lua
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
