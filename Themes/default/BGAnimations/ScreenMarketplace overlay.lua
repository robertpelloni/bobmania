local items = GetMarketplaceItems()
local current_index = 1
local t = Def.ActorFrame {}

-- Helper to get balance
local function UpdateBalance(self)
    local balance = GetPlayerBalance(PLAYER_1)
    self:settext("Balance: " .. balance .. " BOB")
end

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header
    LoadFont("Common Normal")..{
        Text="Marketplace",
        InitCommand=function(self) self:y(-200):zoom(1.5) end
    },

    -- Balance Display
    LoadFont("Common Normal")..{
        Text="Balance: ...",
        InitCommand=function(self) self:xy(250, -200):zoom(0.8):diffuse(Color.Green) end,
        OnCommand=function(self) UpdateBalance(self) end,
        TransactionMessageCommand=function(self) UpdateBalance(self) end
    },

    -- Transaction History Button Hint
    LoadFont("Common Normal")..{
        Text="Press SELECT for Wallet History",
        InitCommand=function(self) self:xy(0, 220):zoom(0.6):diffuse(0.7,0.7,0.7,1) end
    },

    -- Mining Status
    LoadFont("Common Normal")..{
        Text="Mining: Active (45 MH/s)",
        InitCommand=function(self)
            local status = GetBobcoinStatus()
            if status.MiningActive then
                self:xy(250, 220):zoom(0.5):diffuse(Color.Cyan):settext("Mining: Active (" .. status.Hashrate .. ")")
            else
                self:xy(250, 220):zoom(0.5):diffuse(0.5,0.5,0.5,1):settext("Mining: Inactive")
            end
        end
    },

    -- Item Grid
    Def.ActorFrame {
        InitCommand=function(self) self:y(-50) end,
        -- Generate Items
        unpack((function()
            local children = {}
            for i, item in ipairs(items) do
                local col = (i-1) % 3
                local row = math.floor((i-1) / 3)

                children[#children+1] = Def.ActorFrame {
                    InitCommand=function(self)
                        self:xy((col-1)*200, (row-1)*120)
                    end,

                    -- Background
                    Def.Quad {
                        InitCommand=function(self) self:zoomto(180, 100):diffuse(0,0,0,0.5) end,
                        UpdateSelectionMessageCommand=function(self)
                            if i == current_index then
                                self:diffuse(0.2,0.2,0.5,0.8):glow(Color.White)
                            else
                                self:diffuse(0,0,0,0.5):glow(0,0,0,0)
                            end
                        end
                    },

                    -- Icon (Placeholder)
                    Def.Quad {
                         InitCommand=function(self) self:zoomto(40,40):x(-60):diffuse(Color.Orange) end
                    },

                    -- Name
                    LoadFont("Common Normal")..{
                        Text=item.Name,
                        InitCommand=function(self) self:xy(0, -20):zoom(0.8):maxwidth(120) end
                    },

                    -- Price
                    LoadFont("Common Normal")..{
                        Text=item.Price .. " BOB",
                        InitCommand=function(self) self:xy(0, 10):zoom(0.7):diffuse(Color.Yellow) end
                    },

                    -- Type
                    LoadFont("Common Normal")..{
                        Text=item.Type,
                        InitCommand=function(self) self:xy(0, 30):zoom(0.5):diffuse(0.6,0.6,0.6,1) end
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

    local num_items = #items
    local cols = 3

    if event.GameButton == "MenuLeft" or event.GameButton == "Left" then
        if current_index > 1 then current_index = current_index - 1 end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuRight" or event.GameButton == "Right" then
        if current_index < num_items then current_index = current_index + 1 end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" or event.GameButton == "Up" then
        if current_index > cols then current_index = current_index - cols end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuDown" or event.GameButton == "Down" then
        if current_index + cols <= num_items then current_index = current_index + cols end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" or event.GameButton == "Center" then
        local item = items[current_index]
        if BuyItem(PLAYER_1, item.ID) then
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
            MESSAGEMAN:Broadcast("Transaction")
            SCREENMAN:SystemMessage("Bought " .. item.Name)
        else
            SOUND:PlayOnce(THEME:GetPathS("Common", "cancel"))
            SCREENMAN:SystemMessage("Insufficient Funds")
        end
    elseif event.GameButton == "Select" then
        SCREENMAN:SetNewScreen("ScreenWalletHistory")
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    end

    return true
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
    MESSAGEMAN:Broadcast("UpdateSelection")
end

return t
