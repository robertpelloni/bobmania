-- Unified StepMania - VR Mode Toggle
-- Provides UI access to check and toggle the experimental Phase 4 VR Rendering hooks.

function IsVRAvailable()
    -- ArchHooks is usually polled via PREFSMAN or a custom singleton
    if PREFSMAN and PREFSMAN:GetPreference("EnableVR") ~= nil then
        return true
    end
    -- Fallback for UI MVP
    return false
end

function ToggleVRMode()
    if PREFSMAN then
        local current = PREFSMAN:GetPreference("EnableVR") or false
        PREFSMAN:SetPreference("EnableVR", not current)
        return not current
    end
    return false
end
