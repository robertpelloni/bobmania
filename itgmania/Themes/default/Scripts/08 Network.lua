-- Network / NetPlay API Wrappers

function IsNetConnected()
    -- Check if GameClient exists (singleton usually not exposed as global variable in standard SM5,
    -- but we might have exposed it as GAMECLIENT or similar if we implemented Lua bindings)
    -- For now, we mock the connection status if the backend isn't fully bound.
    if NSMAN and NSMAN:IsConnected() then return true end
    return false
end

function GetNetRooms()
    return {
        { Name="Lobby", Players=5, State="Open" },
        { Name="Beginner Room", Players=2, State="Playing" },
        { Name="Expert Only", Players=1, State="Open" }
    }
end

function NetConnect(ip)
    if NSMAN then
        -- NSMAN:Connect(ip) -- Standard SM5 API
        SCREENMAN:SystemMessage("Connecting to " .. ip .. "...")
        return true
    end
    return false
end
