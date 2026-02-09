local t = Def.ActorFrame {}

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "ASSET SYNCHRONIZATION",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,1,0,1) end
}

-- Status Text
local status = "Idle"
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Status: Idle",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 100):zoom(1.0):diffuse(1,1,1,1) end,
    UpdateStatusCommand = function(self, params)
        self:settext("Status: " .. params.status)
    end
}

-- Sync Progress Bar
local progress = 0
t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:Center() end,

    Def.Quad {
        InitCommand = function(self) self:zoomto(400, 20):diffuse(0.2,0.2,0.2,1) end
    },

    Def.Quad {
        InitCommand = function(self) self:zoomto(0, 20):diffuse(0,1,0,1):align(0,0.5):x(-200) end,
        UpdateProgressCommand = function(self, params)
            self:zoomto(params.progress * 400, 20)
        end
    }
}

-- Actions
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Press START to Begin Sync\nPress BACK to Cancel",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.8) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber then return end
    if event.type ~= "InputEventType_FirstPress" then return end

    if event.GameButton == "Start" then
        if ASSETSYNCMAN then
            ASSETSYNCMAN:StartSync()
            MESSAGEMAN:Broadcast("UpdateStatus", {status="Syncing..."})
            -- Mock progress
            local p = 0
            local function UpdateLoop(self)
                p = p + 0.1
                if p > 1 then p = 1 end
                MESSAGEMAN:Broadcast("UpdateProgress", {progress=p})
                if p < 1 then self:sleep(0.5):queuecommand("UpdateLoop") end
            end
            self:queuecommand("UpdateLoop")
        else
            SCREENMAN:SystemMessage("AssetSyncManager not available")
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
