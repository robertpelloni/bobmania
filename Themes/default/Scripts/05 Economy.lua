-- Economy & Marketplace API Wrappers

function GetPlayerBalance(pn)
    if not ECONOMYMAN then return 1000 end -- Mock balance
    return ECONOMYMAN:GetBalance(pn)
end

function GetMarketplaceItems()
    -- Mock data if C++ binding is missing
    return {
        { ID="song_pack_1", Name="Classic Pack 1", Price=500, Type="Song", Icon="Graphics/song_pack_icon.png" },
        { ID="avatar_frame_gold", Name="Gold Frame", Price=200, Type="Item", Icon="Graphics/item_icon.png" },
        { ID="xp_boost_1h", Name="1h XP Boost", Price=100, Type="Boost", Icon="Graphics/boost_icon.png" },
        { ID="theme_dark", Name="Dark Mode Theme", Price=0, Type="Theme", Icon="Graphics/theme_icon.png" },
        { ID="bobcoin_miner", Name="Bobcoin Miner", Price=5000, Type="Hardware", Icon="Graphics/miner_icon.png" }
    }
end

function BuyItem(pn, itemID)
    -- Mock purchase logic
    Trace("Purchased item: " .. itemID .. " for player " .. tostring(pn))
    -- specific logic would go here: ECONOMYMAN:Purchase(pn, itemID)
    return true
end

function GetTransactionHistory(pn)
    return {
        { Date="2023-10-25", Description="Daily Login Bonus", Amount=50 },
        { Date="2023-10-24", Description="Match Win", Amount=100 },
        { Date="2023-10-23", Description="Purchase: Classic Pack 1", Amount=-500 }
    }
end

function GetBobcoinStatus()
    return {
        Rate = 1.25, -- USD/BOB
        MiningActive = true,
        Hashrate = "45 MH/s"
    }
end
