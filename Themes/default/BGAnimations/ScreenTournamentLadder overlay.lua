local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenTournamentLadder", {
    Description = "Tournament Ladder\n\nCompete globally to rank up in ELO.\nUse Up/Down to browse players.\nSTART to challenge a player (Matchmaking).\nBACK to return to Dashboard."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "GLOBAL LADDER",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,0,1,1) end
}

-- Current Player ELO
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Your ELO: ",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 90):zoom(0.7):diffuse(1,1,1,1) end,
    UpdateELOCommand = function(self)
        local elo = 1200
        if ECONOMYMAN then
            elo = ECONOMYMAN:GetPlayerElo(GAMESTATE:GetMasterPlayerNumber())
        end
        self:settext("Your ELO: " .. elo)
    end
}

-- Mock Ladder Data
-- Ladder Data
local players = {}
local function FetchPlayers()
    if TOURNAMENTMAN then
        local rawLadder = TOURNAMENTMAN:GetLadder()
        players = {}
        for i, p in ipairs(rawLadder) do
            players[i] = { Rank = i, Name = p.Opponent, ELO = p.ELO, Status = p.Status, ID = p.MatchID }
        end
    end

    -- Fallback
    if #players == 0 then
        players = {
            { Rank = 1, Name = "StepKing", ELO = 2450, Status = "Online" },
            { Rank = 2, Name = "ArrowSmasher", ELO = 2300, Status = "Online" },
            { Rank = 3, Name = "RhythmMaster", ELO = 2150, Status = "Offline" }
        }
    end
end

FetchPlayers()

local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end

-- List Container
local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 250, 150) end
}

for i, p in ipairs(players) do
    local yPos = (i-1) * 40

    local row = Def.ActorFrame {
        InitCommand = function(self) self:y(yPos) end,
        UpdateSelectionCommand = function(self)
            if i == selectedIndex then
                self:diffuse(1,1,1,1)
                self:GetChild("BGLight"):visible(true)
            else
                self:diffuse(0.6,0.6,0.6,1)
                self:GetChild("BGLight"):visible(false)
            end
        end
    }

    row[#row+1] = Def.Quad {
        Name = "BGLight",
        InitCommand = function(self) self:zoomto(600, 30):x(250):diffuse(1,1,1,0.2):visible(false) end
    }

    -- Rank
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = "#" .. p.Rank,
        InitCommand = function(self) self:zoom(0.8):halign(0):diffuse(1,1,0,1) end
    }

    -- Name
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = p.Name,
        InitCommand = function(self) self:x(80):zoom(0.8):halign(0):diffuse(1,1,1,1) end
    }

    -- ELO
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = "ELO: " .. p.ELO,
        InitCommand = function(self) self:x(350):zoom(0.7):halign(1):diffuse(0,1,1,1) end
    }

    -- Status
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = p.Status,
        InitCommand = function(self) self:x(500):zoom(0.6):halign(1)
            if p.Status == "Online" then self:diffuse(0,1,0,1)
            elseif p.Status == "In Match" then self:diffuse(1,0,0,1)
            else self:diffuse(0.5,0.5,0.5,1) end
        end
    }

    list[#list+1] = row
end

t[#t+1] = list

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Up/Down: Browse | START: Challenge | SELECT: Help | BACK: Dashboard",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #players then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #players end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        if players[selectedIndex].Name == "You" then
            SCREENMAN:SystemMessage("You cannot challenge yourself.")
        elseif players[selectedIndex].Status == "Online" then
            SCREENMAN:SetNewScreen("ScreenTournamentDraft")
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
            -- Would trigger tournament network logic here
        else
            SCREENMAN:SystemMessage("Player is not available to challenge.")
        end
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    elseif event.GameButton == "Select" then
        MESSAGEMAN:Broadcast("HelpToggle")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
    end
    return true
end

t.OnCommand = function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(Input)
    self:playcommand("UpdateELO")
    list:playcommand("UpdateSelection")

    if DiscordRPC then
        DiscordRPC.SetPresence("Climbing Ladder", "Tournament Mode")
    end
end

t.LadderChangedMessageCommand = function(self)
    FetchPlayers()
    -- Re-generate rows? (In real theme we would use an itemscroller)
    SCREENMAN:SetNewScreen("ScreenTournamentLadder") -- Refresh screen for now
end

return t
