local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenAssetSync", {
    Description = "Unified Cloud Sync\n\nCompare your local profile data against the centralized server.\n'Push' uploads your recent Gym workouts and High Scores.\n'Pull' fetches your latest Bobcoin balance and unlocked Marketplace items.\nUse Left/Right to select action, START to execute."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "CLOUD SYNC",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(0,1,1,1) end
}

-- Connection Status
local bConnected = true
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Status: Connected to Unified Server",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 90):zoom(0.6):diffuse(0,1,0,1) end,
    SyncMessageCommand = function(self)
        self:settext("Status: Synchronizing...")
        self:diffuse(1,1,0,1)
        self:sleep(2):queuecommand("Done")
    end,
    DoneCommand = function(self)
        self:settext("Status: Sync Complete")
        self:diffuse(0,1,0,1)
        MESSAGEMAN:Broadcast("UpdateStats")
    end
}

-- Mock Stat Fetching
local localStats = { Coins = 1250, Calories = 4500, ELO = 1200, Items = 12 }
local cloudStats = { Coins = 1000, Calories = 4050, ELO = 1200, Items = 15 }

if ECONOMYMAN then localStats.Coins = ECONOMYMAN:GetBalance() end
if GYMMAN then localStats.Calories = GYMMAN:GetProfile().TotalCaloriesBurned end

-- Local Display (Left)
t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 200, 200) end,

    LoadFont("Common Large") .. { Text = "LOCAL", InitCommand = function(self) self:y(-50):zoom(0.7):diffuse(1,1,1,1) end },

    LoadFont("Common Normal") .. { Name="c", Text = "Bobcoin: " .. localStats.Coins, InitCommand = function(self) self:y(0):zoom(0.7) end, UpdateStatsCommand = function(self) self:settext("Bobcoin: " .. localStats.Coins) end },
    LoadFont("Common Normal") .. { Name="cal", Text = "Calories: " .. localStats.Calories, InitCommand = function(self) self:y(30):zoom(0.7) end, UpdateStatsCommand = function(self) self:settext("Calories: " .. localStats.Calories) end },
    LoadFont("Common Normal") .. { Name="e", Text = "ELO: " .. localStats.ELO, InitCommand = function(self) self:y(60):zoom(0.7) end, UpdateStatsCommand = function(self) self:settext("ELO: " .. localStats.ELO) end },
    LoadFont("Common Normal") .. { Name="i", Text = "Items: " .. localStats.Items, InitCommand = function(self) self:y(90):zoom(0.7) end, UpdateStatsCommand = function(self) self:settext("Items: " .. localStats.Items) end }
}

-- Cloud Display (Right)
t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X + 200, 200) end,

    LoadFont("Common Large") .. { Text = "CLOUD", InitCommand = function(self) self:y(-50):zoom(0.7):diffuse(0,1,1,1) end },

    LoadFont("Common Normal") .. { Name="c", Text = "Bobcoin: " .. cloudStats.Coins, InitCommand = function(self) self:y(0):zoom(0.7):diffuse(0.7,0.7,0.7,1) end, UpdateStatsCommand = function(self) self:settext("Bobcoin: " .. cloudStats.Coins) end },
    LoadFont("Common Normal") .. { Name="cal", Text = "Calories: " .. cloudStats.Calories, InitCommand = function(self) self:y(30):zoom(0.7):diffuse(0.7,0.7,0.7,1) end, UpdateStatsCommand = function(self) self:settext("Calories: " .. cloudStats.Calories) end },
    LoadFont("Common Normal") .. { Name="e", Text = "ELO: " .. cloudStats.ELO, InitCommand = function(self) self:y(60):zoom(0.7):diffuse(0.7,0.7,0.7,1) end, UpdateStatsCommand = function(self) self:settext("ELO: " .. cloudStats.ELO) end },
    LoadFont("Common Normal") .. { Name="i", Text = "Items: " .. cloudStats.Items, InitCommand = function(self) self:y(90):zoom(0.7):diffuse(0.7,0.7,0.7,1) end, UpdateStatsCommand = function(self) self:settext("Items: " .. cloudStats.Items) end }
}

-- Actions
local actions = { "PUSH TO CLOUD", "PULL FROM CLOUD" }
local selectedAction = 1

t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 350) end,
    UpdateSelectionCommand = function(self)
        self:GetChild("A1"):diffuse(selectedAction == 1 and color("1,1,0,1") or color("0.5,0.5,0.5,1"))
        self:GetChild("A2"):diffuse(selectedAction == 2 and color("1,1,0,1") or color("0.5,0.5,0.5,1"))
    end,

    LoadFont("Common Large") .. { Name="A1", Text = "<< PUSH TO CLOUD", InitCommand = function(self) self:x(-150):zoom(0.6) end },
    LoadFont("Common Large") .. { Name="A2", Text = "PULL FROM CLOUD >>", InitCommand = function(self) self:x(150):zoom(0.6) end }
}

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Left/Right: Select Action | START: Execute | SELECT: Help | BACK: Dashboard",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuLeft" then
        selectedAction = 1
        t:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuRight" then
        selectedAction = 2
        t:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        if ASSETSYNCMAN then
            if selectedAction == 1 then
                ASSETSYNCMAN:PushToCloud()
                cloudStats.Calories = localStats.Calories
            else
                ASSETSYNCMAN:PullFromCloud()
                localStats.Items = cloudStats.Items
                localStats.Coins = cloudStats.Coins
            end
        end
        MESSAGEMAN:Broadcast("Sync")
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
    self:playcommand("UpdateSelection")

    if DiscordRPC then
        DiscordRPC.SetPresence("Syncing Profile", "Asset Sync")
    end
end

return t
