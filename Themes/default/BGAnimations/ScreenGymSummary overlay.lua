local t = Def.ActorFrame {}

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "WORKOUT COMPLETE!",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 80):zoom(1.0):diffuse(0,1,0,1):pulse():effectmagnitude(1,1.1,1) end
}

-- Stats Summary
t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 200) end,

    LoadFont("Common Normal") .. {
        Text = "Total Calories Burned:",
        InitCommand = function(self) self:y(-50):zoom(0.8):diffuse(0.7,0.7,0.7,1) end
    },

    LoadFont("Common Large") .. {
        Text = "0.0",
        InitCommand = function(self) self:zoom(1.2):diffuse(1,1,0,1) end,
        OnCommand = function(self)
            if GYMMAN then
                local prof = GYMMAN:GetProfile()
                self:settext(string.format("%.1f", prof.CaloriesToday))
            else
                self:settext("150.0 (Mock)")
            end
        end
    },

    LoadFont("Common Normal") .. {
        Text = "Duration: 25:00",
        InitCommand = function(self) self:y(80):zoom(0.8) end
    },

    LoadFont("Common Normal") .. {
        Text = "Streak Bonus: +5 BOBC",
        InitCommand = function(self) self:y(120):zoom(0.8):diffuse(0,1,1,1) end
    }
}

-- Continue Button
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Press START to Continue",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 100):zoom(0.8):pulse() end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber then return end
    if event.type ~= "InputEventType_FirstPress" then return end

    if event.GameButton == "Start" or event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    end
end

t[#t+1] = Def.ActorFrame {
    OnCommand = function(self) SCREENMAN:GetTopScreen():AddInputCallback(Input) end
}

-- Help Overlay
t[#t+1] = HelpOverlay.Create()

return t
