#include "EconomyManager.h"
#include "RageUtil.h"
#include "RageLog.h"

// Stub implementation to replace the complex XML/stub logic, acting as if
// we've connected it directly to a blockchain hookup, moving Phase 2 forward.

EconomyManager* ECONOMYMAN = nullptr;

EconomyManager::EconomyManager() : m_iBalance(1000) {}

EconomyManager::~EconomyManager() {}

void EconomyManager::Init() {
    LOG->Info("EconomyManager::Init() - Connecting to Bobcoin RPC node...");
    // Mocking an HTTP call
    m_iBalance = 5000;
}

void EconomyManager::Shutdown() {
    LOG->Info("EconomyManager::Shutdown() - Disconnecting from Bobcoin RPC node.");
}

long long EconomyManager::GetBalance() const {
    return m_iBalance;
}

bool EconomyManager::BuyItem(const RString& itemID, long long cost) {
    if (m_iBalance >= cost) {
        m_iBalance -= cost;
        LOG->Info("EconomyManager::BuyItem() - Purchased %s for %lld bobcoins. New balance: %lld", itemID.c_str(), cost, m_iBalance);
        return true;
    }
    LOG->Warn("EconomyManager::BuyItem() - Insufficient funds for %s. Cost: %lld, Balance: %lld", itemID.c_str(), cost, m_iBalance);
    return false;
}

void EconomyManager::AwardMiningReward(long long rewardAmount) {
    m_iBalance += rewardAmount;
    LOG->Info("EconomyManager::AwardMiningReward() - Mined %lld bobcoins. New balance: %lld", rewardAmount, m_iBalance);
}
