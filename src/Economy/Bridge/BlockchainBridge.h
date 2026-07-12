#ifndef BLOCKCHAIN_BRIDGE_H
#define BLOCKCHAIN_BRIDGE_H

#include "global.h"
#include <vector>

struct TransactionRecord {
    RString sTxId;
    RString sType; // "Deposit", "Withdrawal", "Mining", "Purchase"
    long long iAmount;
    long long iTimestamp;
};

struct MarketplaceItem {
    RString sID;
    RString sName;
    long long iPrice;
};

class BlockchainBridge
{
public:
    virtual ~BlockchainBridge() {}
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual long long GetBalance(const RString& sAddress) = 0;
    virtual bool SendTransaction(const RString& sFrom, const RString& sTo, long long iAmount) = 0;
    virtual RString CreateWallet() = 0;
    virtual bool IsConnected() = 0;

    // Added for Phase 2 UI Completeness
    virtual std::vector<TransactionRecord> GetTransactionHistory(const RString& sAddress) = 0;
    virtual std::vector<MarketplaceItem> GetMarketplaceItems() = 0;
};

#endif
