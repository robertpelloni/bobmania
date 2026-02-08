local choices = {
    "Arcade",
    "Gym",
    "Tournament",
    "Economy",
    "Missions",
    "NetPlay",
    "Exit"
}

local current_index = 1
local t = Def.ActorFrame {}

-- Helper to get metric text
local function GetMetricText(name)
    return THEME:GetMetric("ScreenUnifiedDashboard", name)
end

-- Helper to get choice details
local function GetChoiceInfo(choiceName)
    local raw = THEME:GetMetric("ScreenUnifiedDashboard", "Choice"..choiceName)
    local screen = "ScreenTitleMenu"
    local text = choiceName

    -- Parse "screen,ScreenName;text,DisplayName"
    for part in string.gmatch(raw, "([^;]+)") do
        local k, v = string.match(part, "([^,]+),(.+)")
        if k == "screen" then screen = v end
        if k == "text" then text = v end
    end

    local help = THEME:GetMetric("ScreenUnifiedDashboard", "HelpText"..choiceName)
    return screen, text, help
end

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self)
        self:Center()
    end,

    -- Title
    LoadFont("Common Normal")..{
        Text=GetMetricText("HeaderTitle"),
        InitCommand=function(self) self:y(-150):zoom(1.5) end
    },

    -- Subtitle
    LoadFont("Common Normal")..{
        Text=GetMetricText("HeaderSubtitle"),
        InitCommand=function(self) self:y(-120):zoom(0.8):diffuse(0.7,0.7,0.7,1) end
    },

    -- Selection Indicator (Simple Arrow)
    LoadFont("Common Normal")..{
        Text=">",
        InitCommand=function(self) self:x(-150):zoom(1.5):diffuse(Color.Orange) end,
        UpdateSelectionMessageCommand=function(self)
            self:finishtweening():linear(0.1):y((current_index - 1) * 40 - 50)
        end
    },

    -- Help Text Box
    Def.ActorFrame {
        InitCommand=function(self) self:y(180) end,

        Def.Quad {
            InitCommand=function(self) self:zoomto(600, 80):diffuse(0,0,0,0.5) end
        },

        LoadFont("Common Normal")..{
            InitCommand=function(self) self:zoom(0.8):wrapwidthpixels(580) end,
            UpdateSelectionMessageCommand=function(self)
                local _, _, help = GetChoiceInfo(choices[current_index])
                self:settext(help or "")
            end
        }
    }
}

-- Generate Menu Items
for i, choice in ipairs(choices) do
    local _, text, _ = GetChoiceInfo(choice)
    t[#t+1] = LoadFont("Common Normal")..{
        Text=text,
        InitCommand=function(self)
            self:Center():y((i - 1) * 40 - 50):zoom(1)
        end,
        UpdateSelectionMessageCommand=function(self)
            if i == current_index then
                self:diffuse(1,1,1,1):zoom(1.2)
            else
                self:diffuse(0.5,0.5,0.5,1):zoom(1)
            end
        end
    }
end

-- Input Handling
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" or event.GameButton == "Down" then
        current_index = current_index + 1
        if current_index > #choices then current_index = 1 end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
        return true
    elseif event.GameButton == "MenuUp" or event.GameButton == "Up" then
        current_index = current_index - 1
        if current_index < 1 then current_index = #choices end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
        return true
    elseif event.GameButton == "Start" or event.GameButton == "Center" then
        local screen, _, _ = GetChoiceInfo(choices[current_index])
        SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
        SCREENMAN:SetNewScreen(screen)
        return true
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen(THEME:GetMetric("ScreenUnifiedDashboard", "PrevScreen"))
        return true
    end

    return false
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
    MESSAGEMAN:Broadcast("UpdateSelection")
end

return t
