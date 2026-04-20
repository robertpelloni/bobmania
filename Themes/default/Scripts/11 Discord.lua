local DiscordRPC = {}

-- Utility wrapper for the global DISCORD singleton.
-- Fallback gracefully if the C++ module is missing or disabled.
function DiscordRPC.SetPresence(state, details)
    if DISCORD then
        -- The DISCORD singleton has `SetPresence` bound in Lua
        DISCORD:SetPresence(state or "", details or "")
    else
        -- print("Mock Discord RPC: " .. tostring(state) .. " | " .. tostring(details))
    end
end

-- Export globally
_G.DiscordRPC = DiscordRPC
