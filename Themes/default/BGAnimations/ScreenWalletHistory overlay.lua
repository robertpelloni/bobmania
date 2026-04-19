-- ScreenWalletHistory overlay
-- Visualizes the Bobcoin transaction ledger

local t = Def.ActorFrame{}

t[#t+1] = Def.Quad{
    InitCommand=cmd(FullScreen; diffuse,color("0.1,0.1,0.15,0.9")),
}

t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Bobcoin Wallet Ledger",
    InitCommand=cmd(x, SCREEN_CENTER_X; y, 40; zoom, 1.5; diffuse, color("1,0.8,0,1")),
}

t[#t+1] = Def.Actor{
    InitCommand=function(self)
        local history = GetWalletHistory() or {}
        local yPos = 100

        for i, tx in ipairs(history) do
            local colorStr = "1,1,1,1" -- White default
            if tx.amount > 0 then colorStr = "0,1,0,1" end -- Green for deposit/mining
            if tx.amount < 0 then colorStr = "1,0,0,1" end -- Red for purchase

            -- UI elements created dynamically in standard stepmania lua
            self:GetParent():AddChild(
                Def.BitmapText{
                    Font="Common Normal",
                    Text=string.format("[%s] %s: %d BBC", tx.id, tx.type, tx.amount),
                    InitCommand=cmd(x, SCREEN_CENTER_X; y, yPos; diffuse, color(colorStr))
                }
            )
            yPos = yPos + 30
        end
    end
}

return t
