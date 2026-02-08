-- Economy & Marketplace API Wrappers

function GetPlayerBalance(pn)
    if not ECONOMYMAN then return 1000 end
    return ECONOMYMAN:GetBalance()
end

function GetMarketplaceItems()
    if not ECONOMYMAN then return {} end
    return ECONOMYMAN:GetMarketplaceItems()
end

function BuyItem(pn, itemID)
    if not ECONOMYMAN then return false end
    return ECONOMYMAN:BuyItem(itemID)
end

function GetTransactionHistory(pn)
    if not ECONOMYMAN then return {} end
    return ECONOMYMAN:GetHistory()
end

function GetBobcoinStatus()
    local hashrate = 0
    if ECONOMYMAN then hashrate = ECONOMYMAN:GetHashRate() end
    return {
        Rate = 1.25, -- USD/BOB
        MiningActive = (hashrate > 0),
        Hashrate = hashrate .. " MH/s"
    }
end
