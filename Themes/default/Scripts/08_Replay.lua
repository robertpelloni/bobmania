-- Unified StepMania - Ghost Replay Lua Wrapper
-- Safely wraps the C++ REPLAYMAN singleton for legacy UI compatibility.

function LoadGhostReplay(path)
    if REPLAYMAN then
        return REPLAYMAN:LoadReplay(path)
    else
        Warn("REPLAYMAN is missing. Ghost Replay feature disabled.")
        return false
    end
end

function ClearGhostReplay()
    if REPLAYMAN then
        REPLAYMAN:ClearReplay()
    end
end
