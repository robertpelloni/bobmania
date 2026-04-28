local wager = 100
local balance = GetPlayerBalance(PLAYER_1)
local t = Def.ActorFrame {}

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header
    LoadFont("Common Normal")..{
        Text="Place Your Bet",
        InitCommand=function(self) self:y(-150):zoom(1.5) end
    },

    -- Balance
    LoadFont("Common Normal")..{
        Text="Your Balance: " .. balance .. " BOB",
        InitCommand=function(self) self:y(-100):zoom(0.8):diffuse(Color.Green) end
    },

    -- Wager Input
    Def.ActorFrame {
        InitCommand=function(self) self:y(50) end,

        LoadFont("Common Normal")..{
            Text="Wager Amount:",
            InitCommand=function(self) self:y(-40):zoom(1) end
        },

        LoadFont("Common Normal")..{
            Text=wager .. " BOB",
            InitCommand=function(self) self:y(0):zoom(2):diffuse(Color.Yellow) end,
            WagerChangedMessageCommand=function(self) self:settext(wager .. " BOB") end
        },

        LoadFont("Common Normal")..{
            Text="(Up/Down to Adjust)",
            InitCommand=function(self) self:y(40):zoom(0.6):diffuse(0.7,0.7,0.7,1) end
        }
    },

    -- Instructions
    LoadFont("Common Normal")..{
        Text="Press START to Place Bet",
        InitCommand=function(self) self:y(200):zoom(0.8):diffuse(Color.Orange) end
    }
}

-- Input Handler
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuUp" or event.GameButton == "Up" then
        if wager < balance then wager = wager + 50 end
        MESSAGEMAN:Broadcast("WagerChanged")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuDown" or event.GameButton == "Down" then
        if wager > 50 then wager = wager - 50 end
        MESSAGEMAN:Broadcast("WagerChanged")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" or event.GameButton == "Center" then
        -- Mock bet placement
        SCREENMAN:SystemMessage("Bet Placed: " .. wager .. " BOB")
        SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
        SCREENMAN:SetNewScreen("ScreenGameplay") -- Proceed to game
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenSelectMusic")
    end

    return true
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
end

return t
