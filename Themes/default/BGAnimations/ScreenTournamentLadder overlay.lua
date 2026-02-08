local standings = GetTournamentStandings()
local matches = GetUpcomingMatches()
local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header
    LoadFont("Common Normal")..{
        Text="Tournament Ladder",
        InitCommand=function(self) self:y(-220):zoom(1.5) end
    },

    -- Upcoming Matches
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
                    InitCommand=function(self)
                        self:y(i*30)
                    end,

                    LoadFont("Common Normal")..{
                        Text=m.P1 .. " vs " .. m.P2,
                        InitCommand=function(self) self:x(-200):zoom(0.7):halign(0) end
                    },

                    LoadFont("Common Normal")..{
                        Text=m.Time .. " | " .. m.Prize,
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

-- Input Handler
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "Back" then
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
