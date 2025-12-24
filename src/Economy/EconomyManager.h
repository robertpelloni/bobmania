#ifndef ECONOMY_MANAGER_H
#define ECONOMY_MANAGER_H

#include "RageTypes.h"

struct lua_State;
class XNode;

class EconomyManager
{
public:
	EconomyManager();
	~EconomyManager();

	void Init();
	void Update( float fDeltaTime );

	// Basic Ledger
	RString GetWalletAddress() const;
	long long GetBalance() const;
	bool SendTip( const RString& sAddress, long long iAmount );

	// Mocking Tempo connection
	void ConnectToTempo();
	bool IsConnected() const;

	// Persistence
	void LoadFromNode( const XNode *pNode );
	XNode *CreateNode() const;
	void ReadFromDisk();
	void WriteToDisk();

	// Lua
	void PushSelf( lua_State *L );

private:
	RString m_sWalletAddress;
	long long m_iBalance; // In micro-units
	bool m_bConnected;
};

extern EconomyManager*	ECONOMYMAN;	// global and accessible from anywhere in our program

#endif
