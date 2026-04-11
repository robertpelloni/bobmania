local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenMissionSelect", {
    Description = "Mission Mode\n\nComplete daily and weekly objectives to earn XP and Bobcoin.\nSelect a mission to view details or start the challenge.\nMissions rotate regularly."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "MISSION LOG",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,0.5,0,1) end
}

-- Fetch Missions
local missions = {}
local function FetchMissions()
    if MISSIONMAN then
        missions = MISSIONMAN:GetMissions()
    else
        missions = {
            { Title = "Daily Burn (Mock)", Desc = "Burn 200 Calories in Gym Mode", Reward = "50 BOBC", Progress = 120, Target = 200, Status = "Active", ID = "mock1" },
            { Title = "Accuracy King (Mock)", Desc = "Get 95% or higher on a Level 10+", Reward = "Gold Frame", Progress = 0, Target = 1, Status = "Active", ID = "mock2" },
            { Title = "Miner's Delight (Mock)", Desc = "Mine 100 Bobcoin through gameplay", Reward = "+10 Hashrate", Progress = 100, Target = 100, Status = "Complete", ID = "mock3" }
        }
    end
end

FetchMissions()
local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end

-- List Container
local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 250, 150) end
}

for i, mission in ipairs(missions) do
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
        InitCommand = function(self) self:zoomto(600, 50):x(250):diffuse(1,1,1,0.2):visible(false) end
    }

    -- Title
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = mission.Title,
        InitCommand = function(self) self:zoom(0.8):halign(0):y(-10):diffuse(1,1,0,1) end
    }

    -- Desc
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = mission.Desc,
        InitCommand = function(self) self:zoom(0.5):halign(0):y(10):diffuse(0.8,0.8,0.8,1) end
    }

    -- Status/Progress
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = mission.Status .. " (" .. string.format("%.0f", mission.Progress) .. "/" .. string.format("%.0f", mission.Target) .. ")",
        InitCommand = function(self) self:x(500):zoom(0.6):halign(1):y(-10)
            if mission.Status == "Complete" then self:diffuse(0,1,0,1)
            elseif mission.Status == "Claimed" then self:diffuse(0.5,0.5,0.5,1)
            else self:diffuse(1,0.5,0,1) end
        end,
        UpdateStateCommand = function(self)
            self:settext(missions[i].Status .. " (" .. string.format("%.0f", missions[i].Progress) .. "/" .. string.format("%.0f", missions[i].Target) .. ")")
            if missions[i].Status == "Complete" then self:diffuse(0,1,0,1)
            elseif missions[i].Status == "Claimed" then self:diffuse(0.5,0.5,0.5,1)
            else self:diffuse(1,0.5,0,1) end
        end
    }

    -- Reward
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = "Reward: " .. mission.Reward,
        InitCommand = function(self) self:x(500):zoom(0.5):halign(1):y(10):diffuse(0,1,1,1) end
    }

    list[#list+1] = row
end

t[#t+1] = list

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Up/Down: Select | START: Claim Reward | SELECT: Help | BACK: Exit",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #missions then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #missions end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        if missions[selectedIndex].Status == "Complete" then
            if MISSIONMAN then
                if MISSIONMAN:ClaimReward(missions[selectedIndex].ID) then
                    SCREENMAN:SystemMessage("Reward Claimed: " .. missions[selectedIndex].Reward)
                    FetchMissions() -- Refresh state
                    list:playcommand("UpdateState")
                    SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
                end
            else
                SCREENMAN:SystemMessage("Cannot claim in mock mode.")
            end
        elseif missions[selectedIndex].Status == "Claimed" then
            SCREENMAN:SystemMessage("Reward already claimed!")
        else
            SCREENMAN:SystemMessage("Mission not yet complete.")
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
    list:playcommand("UpdateSelection")
    list:playcommand("UpdateState")

    if DiscordRPC then
        DiscordRPC.SetPresence("Checking Objectives", "Mission Log")
    end
end

return t
