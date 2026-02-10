local t = Def.ActorFrame {}

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "GYM WORKOUT",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,1,0,1) end
}

-- Current Song Info
t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 150) end,

    LoadFont("Common Normal") .. {
        Text = "Now Playing:",
        InitCommand = function(self) self:y(-30):zoom(0.7):diffuse(0.7,0.7,0.7,1) end
    },

    LoadFont("Common Large") .. {
        Name = "SongTitle",
        Text = "Loading...",
        InitCommand = function(self) self:zoom(0.8):maxwidth(400) end,
        OnCommand = function(self)
            local song = GAMESTATE:GetCurrentSong()
            if song then
                self:settext(song:GetDisplayMainTitle())
            else
                self:settext("Waiting for Song...")
            end
        end
    }
}

-- Workout Stats
t[#t+1] = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 300) end,

    -- Calories
    LoadFont("Common Normal") .. {
        Text = "Calories Burned: 0",
        InitCommand = function(self) self:x(-150):zoom(0.8) end,
        UpdateStatsCommand = function(self)
            if GYMMAN then
                local prof = GYMMAN:GetProfile()
                self:settext("Calories Burned: " .. string.format("%.1f", prof.TodayCalories))
            end
        end
    },

    -- Time Remaining (Mock)
    LoadFont("Common Normal") .. {
        Text = "Time Remaining: 15:00",
        InitCommand = function(self) self:x(150):zoom(0.8) end
    }
}

-- Actions
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Press START to Begin\nPress SELECT to Skip Song\nPress BACK to End Workout",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 80):zoom(0.7) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber then return end
    if event.type ~= "InputEventType_FirstPress" then return end

    if event.GameButton == "Start" then
        -- Start gameplay (mock transition)
        SCREENMAN:SetNewScreen("ScreenGameplay")
    elseif event.GameButton == "Select" then
        -- Skip current song (mock)
        SCREENMAN:SystemMessage("Skipping Song...")
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenGymSummary")
    end
end

t[#t+1] = Def.ActorFrame {
    OnCommand = function(self)
        SCREENMAN:GetTopScreen():AddInputCallback(Input)
        self:queuecommand("UpdateStatsLoop")
    end,
    UpdateStatsLoopCommand = function(self)
        self:runcommandonchildren("UpdateStats")
        self:sleep(1):queuecommand("UpdateStatsLoop")
    end
}

-- Help Overlay
t[#t+1] = HelpOverlay.Create()

return t
