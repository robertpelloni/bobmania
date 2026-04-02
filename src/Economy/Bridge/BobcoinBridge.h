#ifndef BOBCOIN_BRIDGE_H
#define BOBCOIN_BRIDGE_H

#include "BlockchainBridge.h"
#include "RageLog.h"
#include "bobcoin.h" // Linked from extern/bobcoin/include

class BobcoinBridge : public BlockchainBridge
{
public:
    BobcoinBridge() : m_bConnected(false), m_pWallet(nullptr) {}

    virtual void Init() override
    {
        LOG->Trace("BobcoinBridge: Initializing...");
        Bobcoin::Ledger::Init(); // Initialize mock ledger
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

        // If we have a local wallet object for this address, use it
        // Otherwise, use static Ledger lookup
        return Bobcoin::Ledger::GetBalance( sAddress );
    }

    virtual bool SendTransaction(const RString& sFrom, const RString& sTo, long long iAmount) override
    {
        if( !m_bConnected ) return false;

        // Use Bobcoin::Ledger
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
        // Leak/Manage memory? For now just return address string.
        RString addr = w->GetAddress();
        delete w;
        return addr;
    }

    virtual bool IsConnected() override
    {
        return m_bConnected;
    }

private:
    bool m_bConnected;
    Bobcoin::Wallet* m_pWallet;
};

#endif
