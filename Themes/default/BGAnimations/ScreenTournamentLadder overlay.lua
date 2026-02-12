local t = Def.ActorFrame {}

-- Fetch Data
local standings = {
    { Rank=1, Name="DDR_King", ELO=1500, Wins=10, Losses=2 },
    { Rank=2, Name="StaminaBot", ELO=1450, Wins=8, Losses=4 },
    { Rank=3, Name="PadSmasher", ELO=1400, Wins=7, Losses=5 },
    { Rank=4, Name="RhythmMaster", ELO=1350, Wins=5, Losses=7 },
    { Rank=5, Name="ArrowKeyUser", ELO=1300, Wins=3, Losses=9 },
}

local matches = {}
if TOURNAMENTMAN then
    local ladder = TOURNAMENTMAN:GetLadder()
    for i, m in ipairs(ladder) do
        if m.Status == "Pending" then
            matches[#matches+1] = {
                ID = m.MatchID,
                P1 = "You",
                P2 = m.Opponent,
                Song = m.Song,
                Time = "Now",
                Prize = "100 BOBC",
                Status = m.Status
            }
        end
    end
else
    matches = {
        { ID="mock1", P1="You", P2="DDR_King", Song="Legend of MAX", Time="15:00", Prize="100 BOBC" },
        { ID="mock2", P1="You", P2="StaminaBot", Song="Vertex Beta", Time="16:00", Prize="100 BOBC" }
    }
end

local selectedMatchIndex = 1

t[#t+1] = HelpOverlay.Create()

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header
    LoadFont("Common Normal")..{
        Text="Tournament Ladder",
        InitCommand=function(self) self:y(-220):zoom(1.5):diffuse(1,1,0,1) end
    },

    -- Upcoming Matches List
    Def.ActorFrame {
        InitCommand=function(self) self:y(-150) end,

        LoadFont("Common Normal")..{
            Text="Upcoming Matches:",
            InitCommand=function(self) self:y(-20):zoom(0.8):halign(0):x(-250):diffuse(Color.Yellow) end
        },

        unpack((function()
            local children = {}
            for i, m in ipairs(matches) do
                children[#children+1] = Def.ActorFrame {
                    InitCommand=function(self) self:y(i*30) end,
                    UpdateSelectionCommand=function(self)
                        if i == selectedMatchIndex then
                            self:diffuse(1,1,0,1):zoom(1.1)
                        else
                            self:diffuse(1,1,1,1):zoom(1.0)
                        end
                    end,

                    LoadFont("Common Normal")..{
                        Text=m.P1 .. " vs " .. m.P2 .. " (" .. m.Song .. ")",
                        InitCommand=function(self) self:x(-200):zoom(0.7):halign(0) end
                    },

                    LoadFont("Common Normal")..{
                        Text=m.Prize,
                        InitCommand=function(self) self:x(200):zoom(0.7):halign(1):diffuse(Color.Green) end
                    }
                }
            end
            return children
        end)())
    },

    -- Leaderboard Grid
    Def.ActorFrame {
        InitCommand=function(self) self:y(50) end,

        LoadFont("Common Normal")..{
            Text="Rank   Player               ELO    W/L",
            InitCommand=function(self) self:y(-25):zoom(0.8):halign(0):x(-250):diffuse(0.7,0.7,0.7,1) end
        },

        unpack((function()
            local children = {}
            for i, p in ipairs(standings) do
                children[#children+1] = Def.ActorFrame {
                    InitCommand=function(self)
                        self:y((i-1)*30)
                    end,

                    -- Rank
                    LoadFont("Common Normal")..{
                        Text="#"..p.Rank,
                        InitCommand=function(self) self:x(-240):zoom(0.8):halign(1) end
                    },

                    -- Name
                    LoadFont("Common Normal")..{
                        Text=p.Name,
                        InitCommand=function(self) self:x(-220):zoom(0.8):halign(0) end
                    },

                    -- ELO
                    LoadFont("Common Normal")..{
                        Text=p.ELO,
                        InitCommand=function(self) self:x(100):zoom(0.8):halign(1):diffuse(Color.Cyan) end
                    },

                    -- W/L
                    LoadFont("Common Normal")..{
                        Text=p.Wins .. "/" .. p.Losses,
                        InitCommand=function(self) self:x(200):zoom(0.8):halign(1):diffuse(Color.White) end
                    }
                }
            end
            return children
        end)())
    }
}

-- Actions Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Press START to Play Match\nPress BACK to Exit",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.8) end
}

-- Update Loop
local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
end
t.InitCommand = function(self) self:queuecommand("UpdateSelection") end
t.UpdateSelectionCommand = UpdateList

-- Input Handler
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" then
        selectedMatchIndex = selectedMatchIndex + 1
        if selectedMatchIndex > #matches then selectedMatchIndex = 1 end
        t:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedMatchIndex = selectedMatchIndex - 1
        if selectedMatchIndex < 1 then selectedMatchIndex = #matches end
        t:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        local m = matches[selectedMatchIndex]
        if m then
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
            -- Call Start Function from 07 Tournament.lua
            if StartTournamentMatch then
                StartTournamentMatch(m.ID)
            else
                SCREENMAN:SystemMessage("Tournament Logic Missing")
            end
        end
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    elseif event.GameButton == "Select" then
        MESSAGEMAN:Broadcast("HelpToggle")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
    end

    return true
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
end

return t
