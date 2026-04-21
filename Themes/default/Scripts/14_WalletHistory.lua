-- Unified StepMania - Economy Wallet History
-- Parses the C++ vector string return from ECONOMYMAN into a Lua table

function GetWalletHistory()
    if not ECONOMYMAN then return {} end

    -- The C++ backend returns a string formatted as "txId|Type|Amount,txId|Type|Amount,"
    -- In a full implementation, Luna<T> would construct a deep Lua table.
    -- For this Unified wrapper, we parse the CSV.
    local rawStr = ECONOMYMAN:GetTransactionHistory()
    if type(rawStr) ~= "string" or rawStr == "" then return {} end

    local history = {}
    for entry in string.gmatch(rawStr, "([^,]+)") do
        local tx = {}
        -- String split by pipe
        local count = 1
        for field in string.gmatch(entry, "([^|]+)") do
            if count == 1 then tx.id = field end
            if count == 2 then tx.type = field end
            if count == 3 then tx.amount = tonumber(field) end
            count = count + 1
        end
        table.insert(history, tx)
    end

    return history
end
