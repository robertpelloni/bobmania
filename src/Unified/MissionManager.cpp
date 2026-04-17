#include "MissionManager.h"
#include "RageLog.h"
#include "Economy/EconomyManager.h"

MissionManager* MISSIONMAN = nullptr;

MissionManager::MissionManager() {}
MissionManager::~MissionManager() {}

void MissionManager::Init() {
    LOG->Info("MissionManager::Init - Loading missions from Unified Server");
    LoadMissionsFromBackend();
}

void MissionManager::Shutdown() {
    LOG->Info("MissionManager::Shutdown");
}

void MissionManager::LoadMissionsFromBackend() {
    // Stub: Simulate fetching daily missions from the server
    MissionData m1;
    m1.id = "daily_notes";
    m1.title = "Step Master";
    m1.description = "Hit 500 notes in any mode.";
    m1.targetValue = 500;
    m1.currentValue = 0;
    m1.rewardBobcoins = 100;
    m1.isClaimed = false;

    MissionData m2;
    m2.id = "daily_calories";
    m2.title = "Cardio Burn";
    m2.description = "Burn 50 calories in Gym Mode.";
    m2.targetValue = 50;
    m2.currentValue = 0;
    m2.rewardBobcoins = 250;
    m2.isClaimed = false;

    m_mMissions[m1.id] = m1;
    m_mMissions[m2.id] = m2;
}

void MissionManager::AddProgress(const RString& sMetric, int iAmount) {
    if (m_mMissions.find(sMetric) != m_mMissions.end()) {
        if (!m_mMissions[sMetric].isClaimed) {
            m_mMissions[sMetric].currentValue += iAmount;
            if (m_mMissions[sMetric].currentValue > m_mMissions[sMetric].targetValue) {
                m_mMissions[sMetric].currentValue = m_mMissions[sMetric].targetValue;
            }
        }
    }
}

void MissionManager::UpdateMissions() {
    // Called periodically if time-based missions exist
}

std::vector<MissionData> MissionManager::GetActiveMissions() const {
    std::vector<MissionData> v;
    for (std::map<RString, MissionData>::const_iterator it = m_mMissions.begin(); it != m_mMissions.end(); ++it) {
        v.push_back(it->second);
    }
    return v;
}

bool MissionManager::IsMissionComplete(const RString& missionId) const {
    std::map<RString, MissionData>::const_iterator it = m_mMissions.find(missionId);
    if (it != m_mMissions.end()) {
        return it->second.currentValue >= it->second.targetValue;
    }
    return false;
}

bool MissionManager::ClaimReward(const RString& missionId) {
    std::map<RString, MissionData>::iterator it = m_mMissions.find(missionId);

    if (it == m_mMissions.end()) {
        LOG->Warn("MissionManager::ClaimReward - Mission %s not found.", missionId.c_str());
        return false;
    }

    if (it->second.isClaimed) {
        LOG->Warn("MissionManager::ClaimReward - Mission %s already claimed.", missionId.c_str());
        return false;
    }

    if (it->second.currentValue < it->second.targetValue) {
        LOG->Warn("MissionManager::ClaimReward - Mission %s incomplete.", missionId.c_str());
        return false;
    }

    // Server Validation Stub
    // Here we would send an HTTP POST to the backend with a cryptographic signature
    // proving the gameplay actually occurred (e.g., verifying ReplayManager hashes).
    LOG->Info("MissionManager::ClaimReward - Contacting server to validate %s...", missionId.c_str());
    bool bServerValidated = true; // Assume success for MVP

    if (bServerValidated) {
        it->second.isClaimed = true;

        // Connect directly to the EconomyManager to award the Bobcoin payout securely
        if (ECONOMYMAN) {
            ECONOMYMAN->AwardMiningReward(it->second.rewardBobcoins);
            LOG->Info("MissionManager::ClaimReward - Success! Awarded %lld Bobcoins.", it->second.rewardBobcoins);
            return true;
        } else {
            LOG->Warn("MissionManager::ClaimReward - ECONOMYMAN not found!");
            return false;
        }
    } else {
        LOG->Warn("MissionManager::ClaimReward - Server rejected validation for %s.", missionId.c_str());
        return false;
    }
}

// ----------------------------------------------------------------------------
// Lua Bindings
// ----------------------------------------------------------------------------
#include "LuaManager.h"
#include "LuaBinding.h"

class LunaMissionManager : public Luna<MissionManager>
{
public:
    static int GetMissionCount(T* p, lua_State* L) {
        lua_pushnumber(L, p->GetActiveMissions().size());
        return 1;
    }

    static int IsMissionComplete(T* p, lua_State* L) {
        RString missionId = SArg(1);
        bool bComplete = p->IsMissionComplete(missionId);
        lua_pushboolean(L, bComplete);
        return 1;
    }

    static int ClaimReward(T* p, lua_State* L) {
        RString missionId = SArg(1);
        bool bSuccess = p->ClaimReward(missionId);
        lua_pushboolean(L, bSuccess);
        return 1;
    }

    LunaMissionManager()
    {
        ADD_METHOD(GetMissionCount);
        ADD_METHOD(IsMissionComplete);
        ADD_METHOD(ClaimReward);
    }
};

LUA_REGISTER_CLASS(MissionManager)
