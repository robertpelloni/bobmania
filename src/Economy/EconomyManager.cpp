#include "EconomyManager.h"
#include "RageUtil.h"
#include "RageLog.h"
#include "Bridge/BobcoinBridge.h"
#include "Preference.h"

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
        // We could dynamically query here, but caching it locally is safer for UI rendering.
        // Let's assume the bridge updates the local balance for now.
        return m_pBridge->GetBalance(m_sBobcoinAddress.Get());
    }
    return m_iBalance;
}

bool EconomyManager::BuyItem(const RString& itemID, long long cost) {
    if (!m_pBridge || !m_pBridge->IsConnected()) {
        LOG->Warn("EconomyManager::BuyItem() - Bridge not connected.");
        return false;
    }

    // Step 1: Query live balance directly from the Blockchain Bridge
    long long currentBalance = m_pBridge->GetBalance(m_sBobcoinAddress.Get());

    if (currentBalance >= cost) {
        // Step 2: Issue a transaction to the master store address
        bool success = m_pBridge->SendTransaction(m_sBobcoinAddress.Get(), m_sMasterAddress.Get(), cost);
        if(success) {
            LOG->Info("EconomyManager::BuyItem() - Purchased %s for %lld bobcoins.", itemID.c_str(), cost);
            // Local cache update is handled by the Bridge query on next GetBalance
            return true;
        } else {
            LOG->Warn("EconomyManager::BuyItem() - Blockchain transaction failed for %s.", itemID.c_str());
            return false;
        }
    }

    LOG->Warn("EconomyManager::BuyItem() - Insufficient funds for %s. Cost: %lld, Balance: %lld", itemID.c_str(), cost, currentBalance);
    return false;
}

void EconomyManager::AwardMiningReward(long long rewardAmount) {
    if (!m_pBridge || !m_pBridge->IsConnected()) {
        LOG->Warn("EconomyManager::AwardMiningReward() - Bridge not connected.");
        return;
    }

    // A real implementation would verify cryptographic work here.
    // For now, the "Master" address mints/sends coins to the player.
    bool success = m_pBridge->SendTransaction(m_sMasterAddress.Get(), m_sBobcoinAddress.Get(), rewardAmount);

    if(success) {
        LOG->Info("EconomyManager::AwardMiningReward() - Successfully mined %lld bobcoins.", rewardAmount);
    } else {
        LOG->Warn("EconomyManager::AwardMiningReward() - Failed to transfer mining reward.");
    }
}
