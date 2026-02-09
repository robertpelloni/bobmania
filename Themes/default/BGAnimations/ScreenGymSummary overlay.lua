local t = Def.ActorFrame {}

-- Fetch Stats
local function GetStats()
    local pss = STATSMAN:GetCurStageStats():GetPlayerStageStats(PLAYER_1)
    local calories = 0
    local duration = 0
    local songs = {}

    if pss then
        calories = pss:GetCaloriesBurned()
        duration = pss:GetAliveSeconds()
        -- In a real Course, we'd list the songs.
        -- For MVP, we just show the count.
        table.insert(songs, "Workout Session")
    end

    return calories, duration, songs
end

local cals, dur, songs = GetStats()

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Background
    Def.Quad {
        InitCommand=function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):diffuse(0,0,0,0.8) end
    },

    -- Header
    LoadFont("Common Normal")..{
        Text="Workout Complete",
        InitCommand=function(self) self:y(-200):zoom(1.5):diffuse(Color.Green) end
    },

    -- Stats Container
    Def.ActorFrame {
        InitCommand=function(self) self:y(-50) end,

        -- Calories
        LoadFont("Common Normal")..{
            Text=string.format("Calories Burned:\n%.1f kcal", cals),
            InitCommand=function(self) self:x(-200):zoom(1.2) end
        },

        -- Time
        LoadFont("Common Normal")..{
            Text=string.format("Duration:\n%s", SecondsToMMSS(dur)),
            InitCommand=function(self) self:x(200):zoom(1.2) end
        },

        -- XP / Rewards Animation
        LoadFont("Common Normal")..{
            Text="XP Earned: " .. math.floor(cals * 1.5),
            InitCommand=function(self) self:y(100):zoom(0):diffuse(Color.Yellow) end,
            OnCommand=function(self) self:sleep(0.5):linear(0.5):zoom(1.5):pulse():effectmagnitude(1.1, 0.9, 0) end
        }
    },

    -- Continue Button
    LoadFont("Common Normal")..{
        Text="Press START to Continue",
        InitCommand=function(self) self:y(250):zoom(0.8):diffuseblink():effectcolor1(1,1,1,1):effectcolor2(0.5,0.5,0.5,1) end
    }
}

-- Input
local function InputHandler(event)
    if event.type ~= "InputEventType_FirstPress" then return false end
    if event.GameButton == "Start" or event.GameButton == "Center" then
        SCREENMAN:SetNewScreen("ScreenGymWelcome")
        return true
    end
    return false
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
end

return t
