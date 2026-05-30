#ifndef ECONOMY_MANAGER_H
#define ECONOMY_MANAGER_H

#include "global.h"
#include <vector>

class BlockchainBridge; // Forward declaration
struct TransactionRecord; // Forward declaration from Bridge

/**
 * Singleton to manage the Economy interactions via Bobcoin RPC.
 * Also includes an encrypted XML fallback mechanism for offline play.
 */
class EconomyManager
{
public:
    EconomyManager();
    ~EconomyManager();

    void Init();
    void Shutdown();

    long long GetBalance() const;
    bool BuyItem(const RString& itemID, long long cost);
    void AwardMiningReward(long long rewardAmount);

    std::vector<TransactionRecord> GetTransactionHistory() const;

private:
    long long m_iBalance;
};

extern EconomyManager* ECONOMYMAN;

#endif
