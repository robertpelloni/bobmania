local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenSpectate", {
    Description = "Spectator Hub\n\nWatch ongoing live matches.\nSTART to join stream.\nSELECT for Help.\nBACK to return."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "SPECTATE TV",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,0,0,1) end
}

-- Status
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Status: Connecting to Stream Server...",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 90):zoom(0.6):diffuse(0,1,0,1) end,
    OnCommand = function(self)
        if SPECTATORMAN then
            self:settext("Status: Connected to Spectator Network")
        else
            self:settext("Status: Spectator Network Offline (Mock Mode)")
            self:diffuse(1,1,0,1)
        end
    end
}

-- Mock Active Matches
local matches = {
    { ID = "m01", Player1 = "StepKing", Player2 = "ArrowSmasher", Song = "Hardcore Rave", Viewers = 145 },
    { ID = "m02", Player1 = "BeatFreak", Player2 = "RhythmMaster", Song = "Chillout Mix", Viewers = 89 },
    { ID = "m03", Player1 = "SpeedyG", Player2 = "SlowJamz", Song = "Tech Step 99", Viewers = 12 }
}

local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end

-- List Container
local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 250, 150) end
}

for i, m in ipairs(matches) do
    local yPos = (i-1) * 60

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
        InitCommand = function(self) self:zoomto(500, 50):x(250):diffuse(1,1,1,0.2):visible(false) end
    }

    -- Matchup
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = m.Player1 .. " vs " .. m.Player2,
        InitCommand = function(self) self:zoom(0.8):halign(0):y(-10):diffuse(1,1,0,1) end
    }

    -- Song
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = "Song: " .. m.Song,
        InitCommand = function(self) self:zoom(0.6):halign(0):y(10):diffuse(0.8,0.8,0.8,1) end
    }

    -- Viewers
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = "👁 " .. m.Viewers,
        InitCommand = function(self) self:x(450):zoom(0.7):halign(1):y(0):diffuse(0,1,1,1) end
    }

    list[#list+1] = row
end

t[#t+1] = list

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Up/Down: Browse | START: Watch Match | SELECT: Help | BACK: Dashboard",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #matches then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #matches end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        if SPECTATORMAN then
            SPECTATORMAN:ConnectToMatch(matches[selectedIndex].ID)
        end
        SCREENMAN:SystemMessage("Joining stream: " .. matches[selectedIndex].Player1 .. " vs " .. matches[selectedIndex].Player2)
        SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
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
    list:playcommand("UpdateSelection")

    if DiscordRPC then
        DiscordRPC.SetPresence("Watching Matches", "Spectator TV")
    end
end

return t
