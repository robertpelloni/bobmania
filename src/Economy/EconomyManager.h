#ifndef ECONOMY_MANAGER_H
#define ECONOMY_MANAGER_H

<<<<<<< HEAD
#include "global.h"

/**
 * Singleton to manage the Economy interactions via Bobcoin RPC.
 */
=======
#include "RageTypes.h"
#include <map>
#include <vector>
#include "Bridge/BobcoinBridge.h"

struct lua_State;
class XNode;

struct EconomyItem {
    RString ID;
    RString Name;
    long long Price;
    RString Type;
    RString Icon;
};

struct Transaction {
    RString Date;
    RString Description;
    long long Amount;
};

>>>>>>> origin/unified-ui-features-13937230807013224518
class EconomyManager
{
public:
    EconomyManager();
    ~EconomyManager();

<<<<<<< HEAD
    void Init();
    void Shutdown();

    long long GetBalance() const;
    bool BuyItem(const RString& itemID, long long cost);
    void AwardMiningReward(long long rewardAmount);

private:
    long long m_iBalance;
=======
	void Init();
    void LoadCatalog();
	void Update( float fDeltaTime );

	// Basic Ledger
	RString GetWalletAddress() const;
	long long GetBalance() const;
	bool SendTip( const RString& sAddress, long long iAmount );

    // Marketplace
    bool BuyItem( const RString& sItemID );
    bool HasItem( const RString& sItemID ) const;
    const std::vector<EconomyItem>& GetMarketplaceItems() const;

    // Transaction History
    const std::vector<Transaction>& GetHistory() const;
    void LogTransaction( const RString& sDesc, long long iAmount );
    void Deposit( long long iAmount, const RString& sDesc );

    // Mining
    void AwardMiningReward( float fScore, float fDifficulty );
    float GetHashRate() const;

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
    float m_fCurrentHashRate;

    std::map<RString, bool> m_OwnedItems;
    std::vector<Transaction> m_History;
    std::vector<EconomyItem> m_MarketplaceCatalog;

    BobcoinBridge* m_pBridge;
>>>>>>> origin/unified-ui-features-13937230807013224518
};

extern EconomyManager* ECONOMYMAN;

#endif
