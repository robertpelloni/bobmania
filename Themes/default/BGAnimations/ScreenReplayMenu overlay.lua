local t = Def.ActorFrame {}

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "REPLAY BROWSER",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,1,0,1) end
}

-- Replay List (Mock)
local replays = {
    { Name="Example Replay 1 (99.5%)", Path="Save/Replays/example1.rpl" },
    { Name="Example Replay 2 (98.2%)", Path="Save/Replays/example2.rpl" },
    { Name="Example Replay 3 (95.0%)", Path="Save/Replays/example3.rpl" },
}

local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end

-- List Container
local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 100, 150) end
}

for i, replay in ipairs(replays) do
    list[#list+1] = LoadFont("Common Normal") .. {
        Text = replay.Name,
        InitCommand = function(self) self:y((i-1)*30):zoom(0.8) end,
        UpdateSelectionCommand = function(self)
            if i == selectedIndex then
                self:diffuse(1,1,0,1):zoom(1.0)
            else
                self:diffuse(0.6,0.6,0.6,1):zoom(0.8)
            end
        end
    }
end

list.InitCommand = function(self) self:queuecommand("UpdateSelection") end
t[#t+1] = list

-- Actions
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Press START to Watch\nPress BACK to Exit",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.8) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber then return end
    if event.type ~= "InputEventType_FirstPress" then return end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #replays then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #replays end
        list:playcommand("UpdateSelection")
    elseif event.GameButton == "Start" then
        local replay = replays[selectedIndex]
        if REPLAYMAN then
            REPLAYMAN:LoadReplay(replay.Path)
            REPLAYMAN:StartPlayback()
            SCREENMAN:SetNewScreen("ScreenGameplay")
        else
            SCREENMAN:SystemMessage("ReplayManager not available")
        end
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    end
end

t[#t+1] = Def.ActorFrame {
    OnCommand = function(self) SCREENMAN:GetTopScreen():AddInputCallback(Input) end
}

-- Help Overlay
t[#t+1] = HelpOverlay.Create()

return t
