#include "global.h"
#include "ContentSwarmManager.h"
#include "EconomyManager.h"
#include "RageUtil.h"
#include "RageLog.h"

// Local UUID helper
static std::string MakeUUID()
{
	std::string res = "";
	for(int i=0; i<32; ++i) res += ssprintf("%x", RandomInt(16));
	return res;
}

ContentSwarmManager* ContentSwarmManager::s_pInstance = NULL;

ContentSwarmManager* ContentSwarmManager::Instance()
{
	if (s_pInstance == NULL)
		s_pInstance = new ContentSwarmManager;
	return s_pInstance;
}

void ContentSwarmManager::Destroy()
{
	delete s_pInstance;
	s_pInstance = NULL;
}

ContentSwarmManager::ContentSwarmManager() : m_Mutex("ContentSwarmManager")
{
	m_iSessionBandwidthEarnings = 0;
	m_fSeedingTimer = 0;
}

ContentSwarmManager::~ContentSwarmManager()
{
}

void ContentSwarmManager::Initialize()
{
	LockMut(m_Mutex);
	LOG->Trace("ContentSwarmManager: Joining Mesh Network...");

	// Mock Discovery: Populate with fake remote packs
	m_SwarmInventory.push_back({ "PACK_001", "DDR 1st Mix (Remastered)", 150000000, 12, false });
	m_SwarmInventory.push_back({ "PACK_002", "In The Groove 3", 400000000, 45, false });
	m_SwarmInventory.push_back({ "PACK_003", "Community Pack 2025", 250000000, 8, false });

	LOG->Trace("ContentSwarmManager: Found %zu packs in swarm.", m_SwarmInventory.size());
}

void ContentSwarmManager::Update(float fDeltaTime)
{
	LockMut(m_Mutex);

	// Simulate Download Progress
	for (auto it = m_ActiveDownloads.begin(); it != m_ActiveDownloads.end(); )
	{
		it->second += fDeltaTime * 0.1f; // 10% per second
		if (it->second >= 1.0f)
		{
			LOG->Trace("ContentSwarmManager: Download Complete - %s", it->first.c_str());
			// Mark as local
			for(auto& p : m_SwarmInventory) {
				if(p.id == it->first) p.isLocal = true;
			}
			it = m_ActiveDownloads.erase(it);
		}
		else
		{
			++it;
		}
	}

	// Simulate Seeding Rewards (Bandwidth Contribution)
	m_fSeedingTimer += fDeltaTime;
	if (m_fSeedingTimer >= 5.0f) // Every 5 seconds
	{
		m_fSeedingTimer = 0;
		// Assume we are seeding random bits
		long long reward = 2;
		m_iSessionBandwidthEarnings += reward;

		// Credit Economy
		EconomyManager::Instance()->AwardBandwidthReward(reward);
	}
}

std::vector<SwarmPack> ContentSwarmManager::GetAvailablePacks() const
{
	return m_SwarmInventory;
}

void ContentSwarmManager::RegisterLocalPack(const std::string& name)
{
	LockMut(m_Mutex);
	std::string id = "LOC_" + MakeUUID().substr(0,8);
	m_SwarmInventory.push_back({ id, name, 100000, 1, true });
	LOG->Trace("ContentSwarmManager: Registered local pack %s", name.c_str());
}

void ContentSwarmManager::PublishLocalPack(const std::string& path)
{
	// Simulate hashing the file at 'path' and broadcasting to mesh
	std::string name = "User Course (" + path + ")";
	RegisterLocalPack(name);

	// Bonus for publishing
	EconomyManager::Instance()->AwardBandwidthReward(50);
}

bool ContentSwarmManager::StartDownload(const std::string& packID)
{
	LockMut(m_Mutex);
	if (m_ActiveDownloads.find(packID) != m_ActiveDownloads.end()) return false;

	m_ActiveDownloads[packID] = 0.0f;
	LOG->Trace("ContentSwarmManager: Started download for %s", packID.c_str());
	return true;
}

float ContentSwarmManager::GetDownloadProgress(const std::string& packID)
{
	LockMut(m_Mutex);
	if (m_ActiveDownloads.find(packID) == m_ActiveDownloads.end()) return 0.0f;
	return m_ActiveDownloads[packID];
}
