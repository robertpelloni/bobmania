#include "ContentSwarmManager.h"
#include "RageLog.h"
#include <stdlib.h>

// In the future: #include <libtorrent/session.hpp>

ContentSwarmManager* SWARMMAN = nullptr;

ContentSwarmManager::ContentSwarmManager() {
    m_MockTask.progress = 0.0f;
    m_MockTask.seeds = 0;
    m_MockTask.leechers = 0;
    m_MockTask.complete = false;
}

ContentSwarmManager::~ContentSwarmManager() {
    Shutdown();
}

void ContentSwarmManager::Init() {
    LOG->Info("ContentSwarmManager::Init() - Initializing P2P Stub (libtorrent pending)");
}

void ContentSwarmManager::Shutdown() {
    LOG->Info("ContentSwarmManager::Shutdown() - Stopping P2P swarms");
}

void ContentSwarmManager::Update(float fDeltaTime) {
    // Simulate a download progressing over time
    if (!m_MockTask.complete && m_MockTask.hash != "") {
        // Randomly fluctuate seeds and leeches
        if (rand() % 100 > 90) m_MockTask.seeds += (rand() % 3) - 1;
        if (rand() % 100 > 90) m_MockTask.leechers += (rand() % 3) - 1;

        // Ensure they don't go negative
        if (m_MockTask.seeds < 1) m_MockTask.seeds = 1;
        if (m_MockTask.leechers < 0) m_MockTask.leechers = 0;

        // Progress based on number of seeds
        m_MockTask.progress += (0.005f * m_MockTask.seeds) * fDeltaTime;

        if (m_MockTask.progress >= 1.0f) {
            m_MockTask.progress = 1.0f;
            m_MockTask.complete = true;
            LOG->Info("ContentSwarmManager: Finished downloading %s", m_MockTask.hash.c_str());
        }
    }
}

bool ContentSwarmManager::StartDownload(const RString& sContentHash) {
    LOG->Info("ContentSwarmManager: Connecting to swarm for %s", sContentHash.c_str());
    m_MockTask.hash = sContentHash;
    m_MockTask.progress = 0.0f;
    m_MockTask.complete = false;
    m_MockTask.seeds = 3 + (rand() % 10);
    m_MockTask.leechers = rand() % 5;
    return true;
}

void ContentSwarmManager::CancelDownload(const RString& sContentHash) {
    if (m_MockTask.hash == sContentHash) {
        LOG->Info("ContentSwarmManager: Cancelled download for %s", sContentHash.c_str());
        m_MockTask.hash = "";
    }
}

float ContentSwarmManager::GetDownloadProgress(const RString& sContentHash) const {
    if (m_MockTask.hash == sContentHash) {
        return m_MockTask.progress;
    }
    return 0.0f;
}

int ContentSwarmManager::GetSeedCount(const RString& sContentHash) const {
    if (m_MockTask.hash == sContentHash) {
        return m_MockTask.seeds;
    }
    return 0;
}

int ContentSwarmManager::GetLeechCount(const RString& sContentHash) const {
    if (m_MockTask.hash == sContentHash) {
        return m_MockTask.leechers;
    }
    return 0;
}

bool ContentSwarmManager::IsDownloadComplete(const RString& sContentHash) const {
    if (m_MockTask.hash == sContentHash) {
        return m_MockTask.complete;
    }
    return false;
}
