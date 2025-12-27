#include "global.h"
#include "EconomyManager.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "IniFile.h"
#include <climits>

// Fix for missing macro in standalone compilation
#ifndef PRINTF
#define PRINTF(a,b)
#endif

// Local UUID helper to avoid dependency issues
static std::string MakeUUID()
{
	std::string res = "";
	for(int i=0; i<32; ++i) res += ssprintf("%x", RandomInt(16));
	return res;
}

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

EconomyManager::EconomyManager() : m_Mutex("EconomyManager")
{
	m_bBetActive = false;
	m_iCurrentBetAmount = 0;
	m_fMiningTimer = 0;
	m_fDividendTimer = 0;
	m_iAccumulatedMiningReward = 0;
	m_iPlayerElo = 1200; // Default Starting Elo (Gold/Silver border)
	m_iHighestEloAchieved = 1200;
	m_iCurrentTournamentMatchId = -1;
}

EconomyManager::~EconomyManager()
{
}

void EconomyManager::Initialize()
{
	static bool bInitialized = false;
	if( bInitialized ) return;
	bInitialized = true;

	// Simulate loading a ledger from a blockchain
	LOG->Trace("EconomyManager: Initializing Blockchain Link...");

	// Create a "House" wallet and a "Company DAO" wallet
	m_Ledger["WALLET_HOUSE"] = 100000000;
	m_Ledger["WALLET_DAO"] = 5000000;

	// Create a default Player wallet with signup bonus
	m_Ledger["WALLET_PLAYER"] = 100; // Sign-up bonus

	// Initialize Proposals
	m_Proposals.push_back( { 1, "Add Song: Butterfly", "License fee: 5000 Coins", 120, 10 } );
	m_Proposals.push_back( { 2, "Increase Tournament Fee", "Change from 5% to 7%", 45, 90 } );
	m_Proposals.push_back( { 3, "New Gym Feature", "Fund dev of Calorie Course", 300, 5 } );

	LOG->Trace("EconomyManager: Genesis Block Loaded.");

	LoadState();
}

void EconomyManager::LoadState()
{
	LockMut(m_Mutex);
	IniFile ini;
	if( !ini.ReadFile("Save/Economy.ini") ) return;

	// Load Ledger
	const XNode* pLedgerNode = ini.GetChild("Ledger");
	if( pLedgerNode ) {
		FOREACH_CONST_Attr(pLedgerNode, attr) {
			m_Ledger[attr->first] = StringToInt64(attr->second->GetValue());
		}
	}

	// Load Stats
	ini.GetValue("Stats", "Elo", m_iPlayerElo);
	ini.GetValue("Stats", "HighElo", m_iHighestEloAchieved);
	long long mining;
	if(ini.GetValue("Stats", "MiningRewards", mining)) m_iAccumulatedMiningReward = mining;

	// Load Inventory (Simplified: Comma separated list of names)
	std::string sInv;
	if(ini.GetValue("Inventory", "Items", sInv)) {
		std::vector<std::string> names;
		split(sInv, ",", names);
		for(const auto& name : names) {
			if(!HasAsset(name)) {
				// Reconstruct minimal asset
				Asset a; a.name = name; a.type = "Item"; a.value = 0;
				m_Inventory.push_back(a);
			}
		}
	}

	// Load Equipped
	const XNode* pEquipNode = ini.GetChild("Equipped");
	if( pEquipNode ) {
		FOREACH_CONST_Attr(pEquipNode, attr) {
			m_Equipped[attr->first] = attr->second->GetValue();
		}
	}

	LOG->Trace("EconomyManager: State Loaded.");
}

void EconomyManager::SaveState()
{
	LockMut(m_Mutex);
	IniFile ini;

	// Save Ledger
	for(auto const& [addr, bal] : m_Ledger) {
		ini.SetValue("Ledger", addr, ssprintf("%lld", bal));
	}

	// Save Stats
	ini.SetValue("Stats", "Elo", m_iPlayerElo);
	ini.SetValue("Stats", "HighElo", m_iHighestEloAchieved);
	ini.SetValue("Stats", "MiningRewards", ssprintf("%lld", m_iAccumulatedMiningReward));

	// Save Inventory
	std::string sInv;
	for(const auto& item : m_Inventory) sInv += item.name + ",";
	if(!sInv.empty()) sInv.pop_back();
	ini.SetValue("Inventory", "Items", sInv);

	// Save Equipped
	for(auto const& [slot, item] : m_Equipped) {
		ini.SetValue("Equipped", slot, item);
	}

	ini.WriteFile("Save/Economy.ini");
}

void EconomyManager::Update(float fDeltaTime)
{
	LockMut(m_Mutex);

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

	// Shareholder Dividends Logic
	m_fDividendTimer += fDeltaTime;
	if (m_fDividendTimer >= 60.0f) // Every minute (simulating a "period")
	{
		m_fDividendTimer = 0;
		CurrencyAmount div = CalculateDividend();
		if (div > 0)
		{
			// Pay out
			m_Ledger["WALLET_DAO"] -= div;
			m_Ledger["WALLET_PLAYER"] += div;

			Transaction tx;
			tx.txID = "DIV_" + MakeUUID();
			tx.from = "WALLET_DAO";
			tx.to = "WALLET_PLAYER";
			tx.amount = div;
			tx.reason = "Shareholder Dividend";
			tx.timestamp = time(NULL);
			m_TransactionHistory.push_back(tx);

			LOG->Trace("EconomyManager: Paid Dividend of %lld", div);
		}
	}
}

int EconomyManager::GetShareCount()
{
	LockMut(m_Mutex);
	int count = 0;
	for(const auto& item : m_Inventory) {
		if(item.name == "Company Share") count++;
	}
	return count;
}

CurrencyAmount EconomyManager::CalculateDividend()
{
	// Simple model: 1% of DAO Treasury distributed per Share
	// In reality this would be complex.
	// We only calculate what the PLAYER gets.

	int myShares = GetShareCount();
	if (myShares == 0) return 0;

	CurrencyAmount treasury = m_Ledger["WALLET_DAO"];
	if (treasury <= 1000) return 0; // Minimum threshold

	// Each share gets 0.01% of treasury
	double percentage = 0.0001 * myShares;
	CurrencyAmount payout = (CurrencyAmount)(treasury * percentage);

	return payout;
}

CurrencyAmount EconomyManager::GetBalance(const WalletAddress& address)
{
	LockMut(m_Mutex);
	if (m_Ledger.find(address) == m_Ledger.end())
		return 0;
	return m_Ledger[address];
}

bool EconomyManager::Transfer(const WalletAddress& from, const WalletAddress& to, CurrencyAmount amount, const std::string& reason)
{
	if (amount <= 0)
	{
		LOG->Warn("EconomyManager: Invalid transfer amount %lld", amount);
		return false;
	}

	LockMut(m_Mutex);

	// Check if from address exists and has funds using find() to avoid creating 0-entries
	auto it = m_Ledger.find(from);
	if (it == m_Ledger.end() || it->second < amount)
	{
		LOG->Warn("EconomyManager: Insufficient funds for transfer from %s", from.c_str());
		return false;
	}

	m_Ledger[from] -= amount;
	m_Ledger[to] += amount;

	Transaction tx;
	tx.txID = "TX_" + MakeUUID();
	tx.from = from;
	tx.to = to;
	tx.amount = amount;
	tx.reason = reason;
	tx.timestamp = time(NULL);

	m_TransactionHistory.push_back(tx);

	SaveState(); // Auto-save on transaction
	LOG->Trace("EconomyManager: Transfer Success! %lld coins from %s to %s (%s)", amount, from.c_str(), to.c_str(), reason.c_str());
	return true;
}

WalletAddress EconomyManager::RegisterUser(const std::string& username)
{
	LockMut(m_Mutex);
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

	LockMut(m_Mutex);
	// Need to release mutex before calling Transfer to avoid recursion deadlock if Transfer also locks
	// But our Transfer locks internally. Standard mutex isn't recursive usually in StepMania?
	// RageMutex IS recursive. "Recursive mutex class."

	if (Transfer("WALLET_PLAYER", "WALLET_ESCROW", amount, "Match Wager"))
	{
		m_bBetActive = true;
		m_iCurrentBetAmount = amount;
		LOG->Trace("EconomyManager: Bet placed for %lld", amount);
	}
}

void EconomyManager::ResolveMatchBet(bool playerWon)
{
	LockMut(m_Mutex);
	if (!m_bBetActive) return;

	if (playerWon)
	{
		// Return wager + winnings (1:1 payout from House for this MVP)
		// Check for overflow
		if (m_iCurrentBetAmount > LLONG_MAX / 2)
		{
			LOG->Warn("EconomyManager: Bet amount too high, potential overflow.");
			// Cap or handle error. For now just return original bet?
			Transfer("WALLET_ESCROW", "WALLET_PLAYER", m_iCurrentBetAmount, "Bet Refund (Overflow)");
		}
		else
		{
			CurrencyAmount winnings = m_iCurrentBetAmount; // House pays this part

			// 1. House pays matching amount to player
			Transfer("WALLET_HOUSE", "WALLET_PLAYER", winnings, "House Payout");

			// 2. Escrow returns original bet to player
			Transfer("WALLET_ESCROW", "WALLET_PLAYER", m_iCurrentBetAmount, "Bet Return");
		}
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

void EconomyManager::AwardBandwidthReward(CurrencyAmount amount)
{
	LockMut(m_Mutex);
	if (amount <= 0) return;

	m_Ledger["WALLET_PLAYER"] += amount;
	m_iAccumulatedMiningReward += amount; // Track as generic earning for now
	LOG->Trace("EconomyManager: Awarded %lld tokens for bandwidth.", amount);
}

void EconomyManager::UpdateElo(bool bWon, int iOpponentElo)
{
	LockMut(m_Mutex);

	// Basic Elo Calculation
	// K-Factor = 32 (Standard for new players)
	const int K = 32;

	// Expected Score = 1 / (1 + 10^((OpponentElo - PlayerElo) / 400))
	double expected = 1.0 / (1.0 + pow(10.0, (double)(iOpponentElo - m_iPlayerElo) / 400.0));

	double actual = bWon ? 1.0 : 0.0;
	int change = (int)(K * (actual - expected));

	m_iPlayerElo += change;

	if (m_iPlayerElo > m_iHighestEloAchieved)
	{
		int oldHigh = m_iHighestEloAchieved;
		m_iHighestEloAchieved = m_iPlayerElo;

		// Check for Threshold Crossing (e.g. every 500 points)
		// 1500 (Silver), 2000 (Gold), 2500 (Pro)
		if (oldHigh < 1500 && m_iPlayerElo >= 1500)
		{
			Transfer("WALLET_HOUSE", "WALLET_PLAYER", 500, "Rank Up Reward: SILVER");
		}
		if (oldHigh < 2000 && m_iPlayerElo >= 2000)
		{
			Transfer("WALLET_HOUSE", "WALLET_PLAYER", 1000, "Rank Up Reward: GOLD");
		}
		if (oldHigh < 2500 && m_iPlayerElo >= 2500)
		{
			Transfer("WALLET_HOUSE", "WALLET_PLAYER", 5000, "Rank Up Reward: PRO");
		}
	}

	LOG->Trace("EconomyManager: Elo Update. Old: %d, Opponent: %d, Result: %s, New: %d (Change: %d)",
		m_iPlayerElo - change, iOpponentElo, bWon ? "WIN" : "LOSS", m_iPlayerElo, change);
}

void EconomyManager::AddToInventory(const Asset& asset)
{
	LockMut(m_Mutex);
	m_Inventory.push_back(asset);
	LOG->Trace("EconomyManager: Added %s to inventory.", asset.name.c_str());
}

void EconomyManager::RemoveFromInventory(const std::string& name)
{
	LockMut(m_Mutex);
	for(auto it = m_Inventory.begin(); it != m_Inventory.end(); ++it) {
		if(it->name == name) {
			m_Inventory.erase(it);
			LOG->Trace("EconomyManager: Removed %s from inventory.", name.c_str());
			return;
		}
	}
}

bool EconomyManager::HasAsset(const std::string& name)
{
	LockMut(m_Mutex);
	for(const auto& a : m_Inventory) {
		if(a.name == name) return true;
	}
	return false;
}

void EconomyManager::EquipAsset(const std::string& type, const std::string& name)
{
	LockMut(m_Mutex);
	m_Equipped[type] = name;
	LOG->Trace("EconomyManager: Equipped %s as %s.", name.c_str(), type.c_str());
}

std::string EconomyManager::GetEquippedAsset(const std::string& type)
{
	LockMut(m_Mutex);
	if (m_Equipped.find(type) != m_Equipped.end()) return m_Equipped[type];
	return "";
}

std::vector<Asset> EconomyManager::GetInventory() const
{
	// Returning copy is thread safe enough for MVP if we assume list doesn't change during copy
	// Ideal: LockMut(m_Mutex); but const correctness + returning value is tricky without recursive mutex behavior guaranteed or copy.
	return m_Inventory;
}

std::vector<Transaction> EconomyManager::GetRecentTransactions() const
{
	return m_TransactionHistory;
}

void EconomyManager::VoteOnProposal(int proposalId, bool bYes)
{
	LockMut(m_Mutex);
	for(auto& p : m_Proposals) {
		if(p.id == proposalId) {
			// Vote weight is determined by share ownership
			// Minimum 1 vote for citizenship
			int shares = GetShareCount();
			int weight = (shares > 0) ? shares : 1;

			if( bYes ) p.yesVotes += weight;
			else       p.noVotes += weight;
			
			LOG->Trace("EconomyManager: Voted %s on Prop %d (Weight: %d)", bYes ? "YES" : "NO", proposalId, weight);
			return;
		}
	}
}
