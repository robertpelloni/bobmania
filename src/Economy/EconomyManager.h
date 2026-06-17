#ifndef ECONOMY_MANAGER_H
#define ECONOMY_MANAGER_H

#include "global.h"
#include <vector>

#include "Bridge/BobcoinBridge.h"

class BlockchainBridge; // Forward declaration

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
    void Deposit(long long amount, const RString& reason);
    bool HasItem(const RString& itemID);

    std::vector<TransactionRecord> GetTransactionHistory() const;
    std::vector<MarketplaceItem> GetMarketplaceItems() const;

private:
    long long m_iBalance;
    BobcoinBridge* m_pBridge;
};

extern EconomyManager* ECONOMYMAN;

#endif
