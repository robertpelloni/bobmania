#ifndef BLOCKCHAIN_BRIDGE_H
#define BLOCKCHAIN_BRIDGE_H

#include "RageUtil.h"

/**
 * @brief Interface for connecting to real-world blockchain networks.
 */
class BlockchainBridge
{
public:
	virtual ~BlockchainBridge() {}

	virtual bool Connect() = 0;
	virtual long long GetBalance( const RString &sAddress ) = 0;
	virtual bool SendTransaction( const RString &sTo, long long amount ) = 0;
	virtual RString GetBridgeName() const = 0;
};

#endif
