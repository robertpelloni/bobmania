-- Unified StepMania - Tournament Leaderboard Lua Wrapper
-- Parses the C++ vector string return from TOURNAMENTMAN into a Lua table

function GetTopRankedPlayers()
    if not TOURNAMENTMAN then return {} end

    local rawStr = TOURNAMENTMAN:GetGlobalLeaderboard()
    if type(rawStr) ~= "string" or rawStr == "" then return {} end

    local leaderboard = {}
    for entry in string.gmatch(rawStr, "([^,]+)") do
        local player = {}
        local count = 1
        for field in string.gmatch(entry, "([^|]+)") do
            if count == 1 then player.rank = tonumber(field) end
            if count == 2 then player.username = field end
            if count == 3 then player.elo = tonumber(field) end
            count = count + 1
        end
        table.insert(leaderboard, player)
    end

    return leaderboard
end
