#ifndef MISSION_MANAGER_H
#define MISSION_MANAGER_H

#include "global.h"
#include <map>

struct MissionData {
    RString id;
    RString title;
    RString description;
    int targetValue;
    int currentValue;
    long long rewardBobcoins;
    bool isClaimed;
};

class MissionManager
{
public:
    MissionManager();
    ~MissionManager();

    void Init();
    void Shutdown();

    // Core Tracking
    void AddProgress(const RString& sMetric, int iAmount);
    void UpdateMissions();

    // UI Queries
    std::vector<MissionData> GetActiveMissions() const;
    bool IsMissionComplete(const RString& missionId) const;

    // Server Validation Hook
    bool ClaimReward(const RString& missionId);

private:
    std::map<RString, MissionData> m_mMissions;
    void LoadMissionsFromBackend();
};

extern MissionManager* MISSIONMAN;

#endif
