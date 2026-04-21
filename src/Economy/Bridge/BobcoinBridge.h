#ifndef BOBCOIN_BRIDGE_H
#define BOBCOIN_BRIDGE_H

#include "BlockchainBridge.h"
#include "RageLog.h"

// Note: In the real implementation, this links to extern/bobcoin
// For the Unified Engine compilation mock, we stub the namespace.
namespace Bobcoin {
    class Wallet {
    public:
        static Wallet* Create() { return new Wallet; }
        RString GetAddress() { return "BOB-MOCK-1234"; }
    };
    class Ledger {
    public:
        static void Init() {}
        static long long GetBalance(const RString&) { return 5000; }
        static bool Transfer(const RString&, const RString&, long long) { return true; }
    };
}

class BobcoinBridge : public BlockchainBridge
{
public:
    BobcoinBridge() : m_bConnected(false), m_pWallet(nullptr) {}

    virtual void Init() override
    {
        LOG->Trace("BobcoinBridge: Initializing...");
        Bobcoin::Ledger::Init();
        m_bConnected = true;
    }

    virtual void Shutdown() override
    {
        m_bConnected = false;
        if( m_pWallet ) { delete m_pWallet; m_pWallet = nullptr; }
    }

    virtual long long GetBalance(const RString& sAddress) override
    {
        if( !m_bConnected ) return 0;
        return Bobcoin::Ledger::GetBalance( sAddress );
    }

    virtual bool SendTransaction(const RString& sFrom, const RString& sTo, long long iAmount) override
    {
        if( !m_bConnected ) return false;

        bool success = Bobcoin::Ledger::Transfer(sFrom, sTo, iAmount);

        if( success )
            LOG->Trace("BobcoinBridge: Sent %lld from %s to %s", iAmount, sFrom.c_str(), sTo.c_str());
        else
            LOG->Warn("BobcoinBridge: Failed to send %lld from %s to %s", iAmount, sFrom.c_str(), sTo.c_str());

        return success;
    }

    virtual RString CreateWallet() override
    {
        Bobcoin::Wallet* w = Bobcoin::Wallet::Create();
        RString addr = w->GetAddress();
        delete w;
        return addr;
    }

    virtual bool IsConnected() override
    {
        return m_bConnected;
    }

    virtual std::vector<TransactionRecord> GetTransactionHistory(const RString& sAddress) override
    {
        // Mocking an RPC response from the Bobcoin ledger
        std::vector<TransactionRecord> history;
        if (!m_bConnected) return history;

        TransactionRecord t1 = {"tx_9a8b", "Deposit", 1000, 1699999000};
        TransactionRecord t2 = {"tx_7c6d", "Purchase", -250, 1699999500};
        TransactionRecord t3 = {"tx_5e4f", "Mining", 50, 1700000000};

        history.push_back(t3); // Newest first
        history.push_back(t2);
        history.push_back(t1);

        return history;
    }

private:
    bool m_bConnected;
    Bobcoin::Wallet* m_pWallet;
};

#endif
