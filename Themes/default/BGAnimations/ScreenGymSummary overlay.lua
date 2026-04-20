local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenGymSummary", {
    Description = "Workout Summary\n\nReview the calories burned during your session.\nStats are automatically saved to your Gym Profile."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "WORKOUT COMPLETE",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(0,1,0,1) end
}

-- Summary Data
t[#t+1] = LoadFont("Common Large") .. {
    Text = "Calories Burned: 450",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 200):zoom(1.2):diffuse(1,0.5,0,1) end
}

t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Time Active: 45 Minutes",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 250):zoom(0.8):diffuse(1,1,1,1) end
}

t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Average BPM: 125",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 280):zoom(0.8):diffuse(1,0,0,1) end
}

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "START: Return to Dashboard | SELECT: Help",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "Start" then
        if GYMMAN then
            -- GYMMAN:LogWorkout("Custom Routine", 45, 450)
            SCREENMAN:SystemMessage("Workout Logged!")
        end
        SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    elseif event.GameButton == "Select" then
        MESSAGEMAN:Broadcast("HelpToggle")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
    end
    return true
end

t.OnCommand = function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(Input)
end

return t
