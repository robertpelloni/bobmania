#ifndef ECONOMY_MANAGER_H
#define ECONOMY_MANAGER_H

#include <string>
#include <map>
#include <vector>
#include "RageThreads.h"

// Basic types for our simulated economy
typedef std::string WalletAddress;
typedef long long CurrencyAmount;

struct Transaction {
	std::string txID;
	WalletAddress from;
	WalletAddress to;
	CurrencyAmount amount;
	std::string reason;
	long long timestamp;
};

struct Asset {
	std::string assetID;
	std::string name;
	WalletAddress owner;
	CurrencyAmount value;
};

class EconomyManager
{
public:
	static EconomyManager* Instance();
	static void Destroy();

	EconomyManager();
	~EconomyManager();

	void Initialize();
	void Update(float fDeltaTime); // Called every frame for background mining

	// Wallet Functions
	CurrencyAmount GetBalance(const WalletAddress& address);
	bool Transfer(const WalletAddress& from, const WalletAddress& to, CurrencyAmount amount, const std::string& reason);
	WalletAddress RegisterUser(const std::string& username);

	// Betting System
	bool IsBetActive() const { return m_bBetActive; }
	void StartMatchBet(CurrencyAmount amount);
	void ResolveMatchBet(bool playerWon); // Simple single player vs House/Score for MVP

	// Industry / Infrastructure
	void PaySongRoyalty(const std::string& songTitle, const std::string& artistName);
	CurrencyAmount GetMiningReward() const { return m_iAccumulatedMiningReward; }

	// Data Access for UI
	std::vector<Transaction> GetRecentTransactions() const;

private:
	// Simulated Ledger
	std::map<WalletAddress, CurrencyAmount> m_Ledger;
	std::vector<Transaction> m_TransactionHistory;

	// Active Bet State
	bool m_bBetActive;
	CurrencyAmount m_iCurrentBetAmount;

	// Server Mode Simulation
	float m_fMiningTimer;
	CurrencyAmount m_iAccumulatedMiningReward;

	static EconomyManager* s_pInstance;

	RageMutex m_Mutex; // Thread safety
};

#endif
