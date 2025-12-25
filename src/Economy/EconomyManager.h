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
	std::string type; // "Title", "Avatar", "License"
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
	void LoadState();
	void SaveState();
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
	void AwardBandwidthReward(CurrencyAmount amount);
	CurrencyAmount GetMiningReward() const { return m_iAccumulatedMiningReward; }

	// Competitive
	int GetPlayerElo() const { return m_iPlayerElo; }
	void UpdateElo(bool bWon, int iOpponentElo);
	int GetHighestElo() const { return m_iHighestEloAchieved; }

	// Shareholders
	int GetShareCount();
	CurrencyAmount CalculateDividend();

	// Inventory
	void AddToInventory(const Asset& asset);
	bool HasAsset(const std::string& name);
	void EquipAsset(const std::string& type, const std::string& name);
	std::string GetEquippedAsset(const std::string& type);
	std::vector<Asset> GetInventory() const;

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
	float m_fDividendTimer;
	CurrencyAmount m_iAccumulatedMiningReward;

	// Competitive
	int m_iPlayerElo;
	int m_iHighestEloAchieved;

	// Inventory
	std::vector<Asset> m_Inventory;
	std::map<std::string, std::string> m_Equipped; // Type -> Name

	static EconomyManager* s_pInstance;

	RageMutex m_Mutex; // Thread safety
};

#endif
