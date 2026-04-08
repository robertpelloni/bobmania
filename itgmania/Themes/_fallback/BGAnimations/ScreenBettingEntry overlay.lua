local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenBettingEntry", {
    Description = "Wager Bobcoins\n\nBet on the outcome of a competitive match.\nSelect a player and use Up/Down to change the wager amount.\nSTART to confirm your bet."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "PLACE YOUR BET",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,0.5,0,1) end
}

local currentWager = 100
local maxWager = 1000
if ECONOMYMAN then maxWager = ECONOMYMAN:GetBalance() end

-- Wager Display
t[#t+1] = LoadFont("Common Large") .. {
    Text = currentWager .. " BOBC",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 200):zoom(1.2):diffuse(1,1,0,1) end,
    UpdateWagerCommand = function(self)
        self:settext(currentWager .. " BOBC")
    end
}

t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Select Amount to Bet",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 150):zoom(0.8):diffuse(0.7,0.7,0.7,1) end
}

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Up/Down: Adjust Amount (+/- 50) | START: Confirm Bet | BACK: Cancel",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" or event.GameButton == "Down" then
        currentWager = currentWager - 50
        if currentWager < 50 then currentWager = 50 end
        t:playcommand("UpdateWager")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" or event.GameButton == "Up" then
        currentWager = currentWager + 50
        if currentWager > maxWager then currentWager = maxWager end
        t:playcommand("UpdateWager")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        SCREENMAN:SystemMessage("Wager Placed: " .. currentWager .. " BOBC!")
        SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
        SCREENMAN:SetNewScreen("ScreenTournamentLadder")
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenTournamentLadder")
    elseif event.GameButton == "Select" then
        MESSAGEMAN:Broadcast("HelpToggle")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
    end
    return true
end

t.OnCommand = function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(Input)
    t:playcommand("UpdateWager")
end

return t
