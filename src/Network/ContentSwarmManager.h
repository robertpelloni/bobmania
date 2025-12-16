#ifndef CONTENT_SWARM_MANAGER_H
#define CONTENT_SWARM_MANAGER_H

#include <vector>
#include <string>
#include <map>
#include "RageThreads.h"

struct SwarmPack {
	std::string id;
	std::string name;
	long long sizeBytes;
	int seedCount;
	bool isLocal;
};

class ContentSwarmManager
{
public:
	static ContentSwarmManager* Instance();
	static void Destroy();

	ContentSwarmManager();
	~ContentSwarmManager();

	void Initialize();
	void Update(float fDeltaTime); // Background seeding logic

	// Discovery
	std::vector<SwarmPack> GetAvailablePacks() const;
	void RegisterLocalPack(const std::string& name);

	// Transfer
	bool StartDownload(const std::string& packID);
	float GetDownloadProgress(const std::string& packID); // 0.0 to 1.0

	// Rewards
	long long GetSessionBandwidthEarnings() const { return m_iSessionBandwidthEarnings; }

private:
	std::vector<SwarmPack> m_SwarmInventory;
	std::map<std::string, float> m_ActiveDownloads; // ID -> Progress

	long long m_iSessionBandwidthEarnings;
	float m_fSeedingTimer;

	RageMutex m_Mutex;
	static ContentSwarmManager* s_pInstance;
};

#endif
