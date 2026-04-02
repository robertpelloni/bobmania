#ifndef BLOCKCHAIN_BRIDGE_H
#define BLOCKCHAIN_BRIDGE_H

#include "RageUtil.h"

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
};

#endif
