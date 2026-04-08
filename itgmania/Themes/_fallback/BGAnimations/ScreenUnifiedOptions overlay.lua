local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenUnifiedOptions", {
    Description = "Unified Settings\n\nConfigure advanced options ported from NotITG, Etterna, and OutFox.\nUse Up/Down to browse, Left/Right to change values.\nPress START to save and return."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "UNIFIED SETTINGS",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(0.8,0.2,1,1) end
}

-- Mock Preferences for the UI
-- In a real environment, we'd query PREFSMAN:GetPreference("name")
local prefs = {
    { Name = "Practice Mode", Value = false, Type = "Bool", Desc = "Forces all hits to Marvelous (W1) to practice timing." },
    { Name = "Ghost Tapping", Value = true, Type = "Bool", Desc = "Removes the penalty for tapping when no note is present." },
    { Name = "Both At Once", Value = false, Type = "Bool", Desc = "Mirrors P1 inputs to P2 for co-op or dual-pad play." },
    { Name = "FOV Override", Value = 45.0, Type = "Number", Step = 5.0, Min = 10, Max = 120, Desc = "Custom Field of View for NotITG-style 3D charts." },
    { Name = "Pitch Dependent Rate", Value = true, Type = "Bool", Desc = "Rate mods affect audio pitch (vinyl effect) vs time stretch." },
    { Name = "VR Mode (Beta)", Value = false, Type = "Bool", Desc = "Enables side-by-side stereoscopic rendering." }
}

local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end

-- List Container
local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 300, 150) end
}

for i, pref in ipairs(prefs) do
    local yPos = (i-1) * 60

    local row = Def.ActorFrame {
        InitCommand = function(self) self:y(yPos) end,
        UpdateSelectionCommand = function(self)
            if i == selectedIndex then
                self:diffuse(1,1,1,1)
                self:GetChild("BGLight"):visible(true)
            else
                self:diffuse(0.5,0.5,0.5,1)
                self:GetChild("BGLight"):visible(false)
            end
        end
    }

    row[#row+1] = Def.Quad {
        Name = "BGLight",
        InitCommand = function(self) self:zoomto(600, 50):x(300):diffuse(1,1,1,0.2):visible(false) end
    }

    -- Name
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = pref.Name,
        InitCommand = function(self) self:zoom(0.8):halign(0):y(-10):diffuse(1,1,0,1) end
    }

    -- Desc
    row[#row+1] = LoadFont("Common Normal") .. {
        Text = pref.Desc,
        InitCommand = function(self) self:zoom(0.5):halign(0):y(10):diffuse(0.7,0.7,0.7,1) end
    }

    -- Value Display
    row[#row+1] = LoadFont("Common Large") .. {
        InitCommand = function(self) self:x(550):zoom(0.6):halign(1):y(-10) end,
        UpdateValueCommand = function(self)
            if pref.Type == "Bool" then
                self:settext(pref.Value and "ON" or "OFF")
                if pref.Value then self:diffuse(0,1,0,1) else self:diffuse(1,0,0,1) end
            elseif pref.Type == "Number" then
                self:settext(tostring(pref.Value))
                self:diffuse(0,1,1,1)
            end
        end
    }

    list[#list+1] = row
end

t[#t+1] = list

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Up/Down: Select | Left/Right: Change Value | START: Save & Exit | SELECT: Help",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

-- Input Handling
local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #prefs then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #prefs end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuLeft" then
        local p = prefs[selectedIndex]
        if p.Type == "Bool" then
            p.Value = not p.Value
        elseif p.Type == "Number" then
            p.Value = p.Value - p.Step
            if p.Value < p.Min then p.Value = p.Min end
        end
        list:playcommand("UpdateValue")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
    elseif event.GameButton == "MenuRight" then
        local p = prefs[selectedIndex]
        if p.Type == "Bool" then
            p.Value = not p.Value
        elseif p.Type == "Number" then
            p.Value = p.Value + p.Step
            if p.Value > p.Max then p.Value = p.Max end
        end
        list:playcommand("UpdateValue")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
    elseif event.GameButton == "Start" or event.GameButton == "Back" then
        -- In a real engine, we'd PREFSMAN:SetPreference("name", val) here.
        SCREENMAN:SystemMessage("Unified Settings Saved.")
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
    list:playcommand("UpdateSelection")
    list:playcommand("UpdateValue")

    if DiscordRPC then
        DiscordRPC.SetPresence("Configuring Engine", "Unified Settings")
    end
end

return t
