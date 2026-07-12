#ifndef BOBCOIN_BRIDGE_H
#define BOBCOIN_BRIDGE_H

#include "BlockchainBridge.h"
#include <vector>

class BobcoinBridge : public BlockchainBridge
{
public:
    BobcoinBridge() : m_bConnected(false) {}

    virtual void Init() override {
        m_bConnected = InitRPC();
    }

    virtual void Shutdown() override {
        m_bConnected = false;
    }

    virtual long long GetBalance(const RString& sAddress) override;
    virtual bool SendTransaction(const RString& sFrom, const RString& sTo, long long iAmount) override;
    virtual RString CreateWallet() override { return "BOB-NEW-WALLET"; }
    virtual bool IsConnected() override { return m_bConnected; }

    virtual std::vector<TransactionRecord> GetTransactionHistory(const RString& sAddress) override;
    virtual std::vector<MarketplaceItem> GetMarketplaceItems() override;

private:
    bool InitRPC();
    bool m_bConnected;
};

#endif
