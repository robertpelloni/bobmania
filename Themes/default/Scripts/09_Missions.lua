-- Unified StepMania - Missions Lua Wrapper
-- Safely wraps the C++ MISSIONMAN singleton for legacy UI compatibility.

function GetMissionCount()
    if MISSIONMAN then
        return MISSIONMAN:GetMissionCount()
    end
    return 0
end

function CheckMissionComplete(missionId)
    if MISSIONMAN then
        return MISSIONMAN:IsMissionComplete(missionId)
    end
    return false
end

function AttemptClaimReward(missionId)
    if MISSIONMAN then
        return MISSIONMAN:ClaimReward(missionId)
    end
    return false
end
