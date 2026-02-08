local transactions = GetTransactionHistory(PLAYER_1)
local t = Def.ActorFrame {}

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header
    LoadFont("Common Normal")..{
        Text="Wallet History",
        InitCommand=function(self) self:y(-200):zoom(1.5) end
    },

    -- Back Button Hint
    LoadFont("Common Normal")..{
        Text="Press BACK to Return",
        InitCommand=function(self) self:xy(0, 220):zoom(0.6):diffuse(0.7,0.7,0.7,1) end
    },

    -- Transaction List
    Def.ActorFrame {
        InitCommand=function(self) self:y(-50) end,
        -- Generate Items
        unpack((function()
            local children = {}
            for i, txn in ipairs(transactions) do
                local row = i - 1

                children[#children+1] = Def.ActorFrame {
                    InitCommand=function(self)
                        self:xy(0, row*30)
                    end,

                    -- Date
                    LoadFont("Common Normal")..{
                        Text=txn.Date,
                        InitCommand=function(self) self:x(-200):zoom(0.7):halign(0) end
                    },

                    -- Description
                    LoadFont("Common Normal")..{
                        Text=txn.Description,
                        InitCommand=function(self) self:x(-50):zoom(0.7):halign(0):maxwidth(300) end
                    },

                    -- Amount
                    LoadFont("Common Normal")..{
                        Text=(txn.Amount > 0 and "+" or "") .. txn.Amount .. " BOB",
                        InitCommand=function(self)
                            self:x(250):zoom(0.7):halign(1)
                            if txn.Amount >= 0 then
                                self:diffuse(Color.Green)
                            else
                                self:diffuse(Color.Red)
                            end
                        end
                    }
                }
            end
            return children
        end)())
    }
}

-- Input Handler
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenMarketplace")
    end

    return true
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
end

return t
