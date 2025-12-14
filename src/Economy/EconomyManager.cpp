#include "global.h"
#include "EconomyManager.h"
#include "RageLog.h"
#include "RageUtil.h"

EconomyManager* EconomyManager::s_pInstance = NULL;

EconomyManager* EconomyManager::Instance()
{
	if (s_pInstance == NULL)
		s_pInstance = new EconomyManager;
	return s_pInstance;
}

void EconomyManager::Destroy()
{
	delete s_pInstance;
	s_pInstance = NULL;
}

EconomyManager::EconomyManager()
{
	m_bBetActive = false;
	m_iCurrentBetAmount = 0;
	m_fMiningTimer = 0;
	m_iAccumulatedMiningReward = 0;
}

EconomyManager::~EconomyManager()
{
}

void EconomyManager::Initialize()
{
	// Simulate loading a ledger from a blockchain
	LOG->Trace("EconomyManager: Initializing Blockchain Link...");

	// Create a "House" wallet and a "Company DAO" wallet
	m_Ledger["WALLET_HOUSE"] = 100000000;
	m_Ledger["WALLET_DAO"] = 5000000;

	// Create a default Player wallet with signup bonus
	m_Ledger["WALLET_PLAYER"] = 100; // Sign-up bonus

	LOG->Trace("EconomyManager: Genesis Block Loaded.");
}

void EconomyManager::Update(float fDeltaTime)
{
	// Simulate "Server Mode" - earning small crypto rewards for uptime
	m_fMiningTimer += fDeltaTime;
	if (m_fMiningTimer >= 10.0f) // Every 10 seconds
	{
		m_fMiningTimer = 0;
		// Micro-reward for "Proof of Uptime"
		m_iAccumulatedMiningReward += 1;
		m_Ledger["WALLET_PLAYER"] += 1;

		// Log occasionally
		// LOG->Trace( ssprintf("EconomyManager: Server Node Reward Mined! Balance: %lld", m_Ledger["WALLET_PLAYER"]) );
	}
}

CurrencyAmount EconomyManager::GetBalance(const WalletAddress& address)
{
	if (m_Ledger.find(address) == m_Ledger.end())
		return 0;
	return m_Ledger[address];
}

bool EconomyManager::Transfer(const WalletAddress& from, const WalletAddress& to, CurrencyAmount amount, const std::string& reason)
{
	if (m_Ledger[from] < amount)
	{
		LOG->Warn("EconomyManager: Insufficient funds for transfer from %s", from.c_str());
		return false;
	}

	m_Ledger[from] -= amount;
	m_Ledger[to] += amount;

	Transaction tx;
	tx.txID = "TX_" + Rage::make_uuid(); // Using RageUtil helper
	tx.from = from;
	tx.to = to;
	tx.amount = amount;
	tx.reason = reason;
	tx.timestamp = time(NULL);

	m_TransactionHistory.push_back(tx);

	LOG->Trace("EconomyManager: Transfer Success! %lld coins from %s to %s (%s)", amount, from.c_str(), to.c_str(), reason.c_str());
	return true;
}

WalletAddress EconomyManager::RegisterUser(const std::string& username)
{
	WalletAddress newAddr = "WALLET_" + username; // Simplified for simulation
	if (m_Ledger.find(newAddr) == m_Ledger.end())
	{
		m_Ledger[newAddr] = 50; // Welcome bonus
		LOG->Trace("EconomyManager: Registered new user %s with 50 coins.", username.c_str());
	}
	return newAddr;
}

void EconomyManager::StartMatchBet(CurrencyAmount amount)
{
	if (amount <= 0) return;

	if (Transfer("WALLET_PLAYER", "WALLET_ESCROW", amount, "Match Wager"))
	{
		m_bBetActive = true;
		m_iCurrentBetAmount = amount;
		LOG->Trace("EconomyManager: Bet placed for %lld", amount);
	}
}

void EconomyManager::ResolveMatchBet(bool playerWon)
{
	if (!m_bBetActive) return;

	if (playerWon)
	{
		// Return wager + winnings (1:1 payout from House for this MVP)
		CurrencyAmount winnings = m_iCurrentBetAmount * 2;

		// House pays the winnings part
		Transfer("WALLET_HOUSE", "WALLET_ESCROW", m_iCurrentBetAmount, "House Loss");

		// Escrow pays player
		Transfer("WALLET_ESCROW", "WALLET_PLAYER", winnings, "Match Win Payout");
	}
	else
	{
		// Player lost. Escrow goes to House.
		Transfer("WALLET_ESCROW", "WALLET_HOUSE", m_iCurrentBetAmount, "House Win");
	}

	m_bBetActive = false;
	m_iCurrentBetAmount = 0;
}

void EconomyManager::PaySongRoyalty(const std::string& songTitle, const std::string& artistName)
{
	// Simulate finding the artist's wallet
	WalletAddress artistWallet = "ARTIST_" + artistName;

	// Default cost per play
	CurrencyAmount royaltyFee = 1;

	// The Player pays the royalty (or the Gym Owner in a commercial setting)
	// For this MVP, the Player pays.
	if (Transfer("WALLET_PLAYER", artistWallet, royaltyFee, "Song Royalty: " + songTitle))
	{
		LOG->Trace("EconomyManager: Paid royalty to %s", artistName.c_str());
	}
}

std::vector<Transaction> EconomyManager::GetRecentTransactions() const
{
	return m_TransactionHistory;
}
