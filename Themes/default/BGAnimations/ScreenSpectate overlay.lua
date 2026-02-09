local t = Def.ActorFrame {}

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "SPECTATOR MODE",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,1,0,1) end
}

-- Stream Container
t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:Center() end,

    -- Mock Video Feed
    Def.Quad {
        InitCommand = function(self) self:zoomto(640, 480):diffuse(0.1,0.1,0.1,1) end
    },

    -- "Connecting..." Text
    LoadFont("Common Normal") .. {
        Text = "Connecting to Stream...",
        InitCommand = function(self) self:diffuse(1,1,1,1):pulse():effectmagnitude(1,0.5,1):effectperiod(1) end,
        OnCommand = function(self)
            self:sleep(2):settext("LIVE FEED"):stoptweening():zoom(1.2):diffuse(1,0,0,1)
        end
    },

    -- Live Indicator
    LoadFont("Common Normal") .. {
        Text = "LIVE",
        InitCommand = function(self) self:xy(-300, -220):zoom(0.8):diffuse(1,0,0,1):visible(false) end,
        OnCommand = function(self) self:sleep(2):visible(true):pulse():effectmagnitude(1,0.8,1):effectperiod(0.5) end
    }
}

-- Chat Box (Mock)
t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_WIDTH - 200, SCREEN_CENTER_Y) end,

    Def.Quad {
        InitCommand = function(self) self:zoomto(300, 400):diffuse(0,0,0,0.5) end
    },

    LoadFont("Common Normal") .. {
        Text = "Chat",
        InitCommand = function(self) self:y(-180):zoom(0.8) end
    },

    LoadFont("Common Normal") .. {
        Text = "System: Connected to match.\nUser1: Go go go!\nUser2: Nice combo!",
        InitCommand = function(self) self:y(-150):zoom(0.6):wrapwidthpixels(400):alignment("Left"):valignment("Top") end
    }
}

-- Back Button
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Press BACK to Exit",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.8) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber then return end
    if event.type ~= "InputEventType_FirstPress" then return end

    if event.GameButton == "Back" or event.GameButton == "Select" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    end
end

t[#t+1] = Def.ActorFrame {
    OnCommand = function(self) SCREENMAN:GetTopScreen():AddInputCallback(Input) end
}

-- Help Overlay
t[#t+1] = HelpOverlay.Create()

return t
