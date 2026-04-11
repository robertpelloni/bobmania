#ifndef ECONOMY_MANAGER_H
#define ECONOMY_MANAGER_H

#include "global.h"

/**
 * Singleton to manage the Economy interactions via Bobcoin RPC.
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

private:
    long long m_iBalance;
};

extern EconomyManager* ECONOMYMAN;

#endif
