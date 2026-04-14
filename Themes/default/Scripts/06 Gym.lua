--[[
  GymManager Wrapper for Lua
  Provides easy access to HEARTRATEMAN and GYMMAN Singletons
]]

function GetCurrentHeartRate()
    if HEARTRATEMAN and HEARTRATEMAN:IsConnected() then
        return HEARTRATEMAN:GetBPM()
    end
    return 0 -- Offline
end

function GenerateWorkoutPlaylist(minMeters, maxMeters, lengthMinutes)
    if GYMMAN then
        return GYMMAN:GenerateWorkout(minMeters, maxMeters, lengthMinutes)
    end
    return false
end
