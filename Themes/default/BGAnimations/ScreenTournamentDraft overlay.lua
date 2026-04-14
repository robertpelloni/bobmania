local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenTournamentDraft", {
    Description = "Tournament Draft Phase\n\nTake turns with your opponent Banning and Picking charts from the competitive pool.\nUse Up/Down to navigate the pool, START to confirm your action.\nOnce Draft is complete, the match will automatically begin."
})

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Header
t[#t+1] = LoadFont("Common Large") .. {
    Text = "DRAFT PHASE",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 50):zoom(0.8):diffuse(1,0,1,1) end
}

-- State Logic
local draftState = "P1_BAN"
local pool = {
    { Title = "Hardcore Rave", Meter = 12, State = "Available" },
    { Title = "Chillout Mix", Meter = 8, State = "Available" },
    { Title = "Tech Step 99", Meter = 11, State = "Available" },
    { Title = "Dance Rush", Meter = 10, State = "Available" },
    { Title = "Speedy Gonzales", Meter = 14, State = "Available" }
}

local selectedIndex = 1

local function GetStateText()
    if draftState == "P1_BAN" then return "You: Ban a Chart"
    elseif draftState == "P2_BAN" then return "Opponent: Ban a Chart"
    elseif draftState == "P1_PICK" then return "You: Pick a Chart"
    elseif draftState == "P2_PICK" then return "Opponent: Pick a Chart"
    else return "Draft Complete! Starting Match..." end
end

t[#t+1] = LoadFont("Common Large") .. {
    Text = GetStateText(),
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, 100):zoom(0.8):diffuse(0,1,1,1) end,
    UpdateStateCommand = function(self) self:settext(GetStateText()) end
}

local list = Def.ActorFrame {
    InitCommand = function(self) self:xy(SCREEN_CENTER_X - 200, 180) end
}

for i, chart in ipairs(pool) do
    local yPos = (i-1) * 50

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
        end,
        UpdateStateCommand = function(self)
            local stateText = self:GetChild("StateText")
            stateText:settext(pool[i].State)
            if pool[i].State == "Available" then stateText:diffuse(1,1,1,1)
            elseif pool[i].State == "BANNED" then stateText:diffuse(1,0,0,1)
            elseif pool[i].State == "PICKED" then stateText:diffuse(0,1,0,1) end
        end
    }

    row[#row+1] = Def.Quad {
        Name = "BGLight",
        InitCommand = function(self) self:zoomto(500, 40):x(200):diffuse(1,1,1,0.2):visible(false) end
    }

    row[#row+1] = LoadFont("Common Normal") .. {
        Text = chart.Title .. " [" .. chart.Meter .. "]",
        InitCommand = function(self) self:zoom(0.8):halign(0):diffuse(1,1,0,1) end
    }

    row[#row+1] = LoadFont("Common Normal") .. {
        Name = "StateText",
        Text = chart.State,
        InitCommand = function(self) self:x(450):zoom(0.8):halign(1) end
    }

    list[#list+1] = row
end

t[#t+1] = list

-- Controls Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Up/Down: Select | START: Confirm Action | SELECT: Help",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.6):diffuse(0.5,0.5,0.5,1) end
}

local function Input(event)
    if not event.PlayerNumber or event.type ~= "InputEventType_FirstPress" then return false end

    if draftState == "COMPLETE" then return true end

    -- CPU Turn Simulation (Mock)
    if draftState == "P2_BAN" or draftState == "P2_PICK" then return true end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #pool then selectedIndex = 1 end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #pool end
        list:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        if pool[selectedIndex].State ~= "Available" then
            SCREENMAN:SystemMessage("Chart is already picked or banned.")
            return true
        end

        if draftState == "P1_BAN" then
            pool[selectedIndex].State = "BANNED"
            draftState = "P2_BAN"
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))

            -- Simulate Opponent
            t:sleep(1.5):queuecommand("CPUBan")

        elseif draftState == "P1_PICK" then
            pool[selectedIndex].State = "PICKED"
            draftState = "P2_PICK"
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))

            -- Simulate Opponent
            t:sleep(1.5):queuecommand("CPUPick")
        end

        t:playcommand("UpdateState")
        list:playcommand("UpdateState")
    elseif event.GameButton == "Select" then
        MESSAGEMAN:Broadcast("HelpToggle")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
    end
    return true
end

t.CPUBanCommand = function(self)
    -- Find first available
    for i, c in ipairs(pool) do
        if c.State == "Available" then
            c.State = "BANNED"
            break
        end
    end
    draftState = "P1_PICK"
    self:playcommand("UpdateState")
    list:playcommand("UpdateState")
end

t.CPUPickCommand = function(self)
    for i, c in ipairs(pool) do
        if c.State == "Available" then
            c.State = "PICKED"
            break
        end
    end
    draftState = "COMPLETE"
    self:playcommand("UpdateState")
    list:playcommand("UpdateState")
    SCREENMAN:SystemMessage("Draft Complete! Commencing Match...")
    self:sleep(2):queuecommand("StartMatch")
end

t.StartMatchCommand = function(self)
    SCREENMAN:SetNewScreen("ScreenGameplay")
end

t.OnCommand = function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(Input)
    list:playcommand("UpdateSelection")
    list:playcommand("UpdateState")
end

return t
