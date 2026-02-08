local profile = GetGymProfile(PLAYER_1)
local t = Def.ActorFrame {}

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header
    LoadFont("Common Normal")..{
        Text="Gym Dashboard",
        InitCommand=function(self) self:y(-200):zoom(1.5) end
    },

    -- Stats Grid
    Def.ActorFrame {
        InitCommand=function(self) self:y(-50) end,

        -- Row 1: Weight & BMI
        LoadFont("Common Normal")..{
            Text="Weight: " .. profile.Weight .. " kg",
            InitCommand=function(self) self:xy(-150, -50):zoom(1):diffuse(Color.White) end
        },
        LoadFont("Common Normal")..{
            Text="BMI: " .. profile.BMI,
            InitCommand=function(self) self:xy(150, -50):zoom(1):diffuse(Color.White) end
        },

        -- Row 2: Calories Today vs Goal
        LoadFont("Common Normal")..{
            Text="Today: " .. profile.TodayCalories .. " / " .. profile.DailyGoal .. " kcal",
            InitCommand=function(self) self:xy(0, 0):zoom(1.2):diffuse(Color.Orange) end
        },

        -- Row 3: Total & Streak
        LoadFont("Common Normal")..{
            Text="Total Burned: " .. profile.TotalCaloriesBurned .. " kcal",
            InitCommand=function(self) self:xy(-150, 50):zoom(0.8):diffuse(0.7,0.7,0.7,1) end
        },
        LoadFont("Common Normal")..{
            Text="Streak: " .. profile.StreakDays .. " Days",
            InitCommand=function(self) self:xy(150, 50):zoom(0.8):diffuse(Color.Green) end
        }
    },

    -- Start Button Hint
    LoadFont("Common Normal")..{
        Text="Press START to Select Workout",
        InitCommand=function(self) self:y(200):zoom(1):diffuse(Color.Yellow):pulse():effectmagnitude(1.05,0.95,0):effectperiod(1) end
    },

    -- Back Button Hint
    LoadFont("Common Normal")..{
        Text="Press BACK to Dashboard",
        InitCommand=function(self) self:y(230):zoom(0.6):diffuse(0.7,0.7,0.7,1) end
    }
}

-- Input Handler
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "Start" or event.GameButton == "Center" then
        SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
        SCREENMAN:SetNewScreen("ScreenGymWorkout")
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    end

    return true
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
end

return t
