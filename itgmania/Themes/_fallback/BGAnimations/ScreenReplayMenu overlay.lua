local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenReplayMenu", {
    Description = "Replay & Ghost Menu\n\nLoad a previous local play or download a pro ghost.\nSelecting a replay loads the chart with 'Ghost Data' active.\nYou will race against the recorded inputs in real-time."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "REPLAY BROWSER",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(0.5,0.8,1,1) end
}

-- Mock Replay Data
local replays = {
    { Song = "Hardcore Rave", Player = "StepKing", Score = "98.4%", Date = "2 Days Ago", Type = "Pro Ghost" },
    { Song = "Chillout Mix", Player = "You", Score = "92.1%", Date = "Yesterday", Type = "Local Record" },
    { Song = "Tech Step 99", Player = "You", Score = "85.0%", Date = "Last Week", Type = "Local Record" },
    { Song = "Dance Rush", Player = "BeatFreak", Score = "100%", Date = "1 Month Ago", Type = "World Record" }
}

local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end

-- List Container
local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 250, 150) end
}

for i, r in ipairs(replays) do
    local yPos = (i-1) * 50

    local row = Def.ActorFrame {
        InitCommand = function(self) self:y(yPos) end,
        UpdateSelectionCommand = function(self)
            if i == selectedIndex then
                self:diffuse(1,1,1,1)
                self:GetChild("BGLight"):visible(true)
            else
                self:diffuse(0.7,0.7,0.7,1)
                self:GetChild("BGLight"):visible(false)
            end
        end
    }

    row[#row+1] = Def.Quad {
        Name = "BGLight",
        InitCommand = function(self) self:zoomto(550, 40):x(250):diffuse(1,1,1,0.2):visible(false) end
    }

    -- Song
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = r.Song,
        InitCommand = function(self) self:zoom(0.8):halign(0):y(-8):diffuse(1,1,0,1) end
    }

    -- Player & Score
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = r.Player .. " - " .. r.Score,
        InitCommand = function(self) self:zoom(0.6):halign(0):y(10):diffuse(0.8,0.8,0.8,1) end
    }

    -- Type
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = r.Type,
        InitCommand = function(self) self:x(500):zoom(0.6):halign(1):y(-8)
            if r.Type == "World Record" then self:diffuse(1,0,0,1)
            elseif r.Type == "Pro Ghost" then self:diffuse(1,0.5,0,1)
            else self:diffuse(0.5,1,0.5,1) end
        end
    }

    -- Date
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = r.Date,
        InitCommand = function(self) self:x(500):zoom(0.5):halign(1):y(10):diffuse(0.5,0.5,0.5,1) end
    }

    list[#list+1] = row
end

t[#t+1] = list

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Up/Down: Browse | START: Load Ghost | SELECT: Help | BACK: Dashboard",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #replays then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #replays end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        SCREENMAN:SystemMessage("Loading Ghost Data for: " .. replays[selectedIndex].Song)
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
end

return t
