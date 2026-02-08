local t = Def.ActorFrame {}

-- Helper to get missions
local function GetMissions()
    if not MISSIONMAN then return {} end
    return MISSIONMAN:GetMissions()
end

local missions = GetMissions()
local current_index = 1

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header
    LoadFont("Common Normal")..{
        Text="Mission Mode",
        InitCommand=function(self) self:y(-200):zoom(1.5) end
    },

    -- Mission List
    Def.ActorFrame {
        InitCommand=function(self) self:y(-50) end,
        -- Generate Items
        unpack((function()
            local children = {}
            for i, m in ipairs(missions) do
                local row = i - 1

                children[#children+1] = Def.ActorFrame {
                    InitCommand=function(self)
                        self:xy(0, row*60)
                    end,

                    -- Background
                    Def.Quad {
                        InitCommand=function(self) self:zoomto(500, 50):diffuse(0,0,0,0.5) end,
                        UpdateSelectionMessageCommand=function(self)
                            if i == current_index then
                                self:diffuse(0.2,0.2,0.5,0.8):glow(Color.White)
                            else
                                self:diffuse(0,0,0,0.5):glow(0,0,0,0)
                            end
                        end
                    },

                    -- Name
                    LoadFont("Common Normal")..{
                        Text=m.Title,
                        InitCommand=function(self) self:xy(-230, -10):zoom(1):halign(0) end
                    },

                    -- Description
                    LoadFont("Common Normal")..{
                        Text=m.Desc,
                        InitCommand=function(self) self:xy(-230, 15):zoom(0.6):halign(0):diffuse(0.7,0.7,0.7,1) end
                    },

                    -- Status
                    LoadFont("Common Normal")..{
                        Text=m.Status,
                        InitCommand=function(self)
                            self:xy(230, -10):zoom(0.8):halign(1)
                            if m.Status == "Complete" then self:diffuse(Color.Green)
                            elseif m.Status == "Claimed" then self:diffuse(Color.Gray)
                            else self:diffuse(Color.Yellow) end
                        end
                    },

                    -- Progress Bar
                    Def.Quad {
                        InitCommand=function(self) self:xy(0, 15):zoomto(100, 5):halign(0):diffuse(Color.Black) end
                    },
                    Def.Quad {
                        InitCommand=function(self)
                            local pct = m.Progress / m.Target
                            if pct > 1 then pct = 1 end
                            self:xy(0, 15):zoomto(100*pct, 5):halign(0):diffuse(Color.Orange)
                        end
                    },

                    -- Reward
                    LoadFont("Common Normal")..{
                        Text=m.Reward,
                        InitCommand=function(self) self:xy(230, 15):zoom(0.6):halign(1):diffuse(Color.Orange) end
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

    if event.GameButton == "MenuUp" or event.GameButton == "Up" then
        if current_index > 1 then current_index = current_index - 1 end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuDown" or event.GameButton == "Down" then
        if current_index < #missions then current_index = current_index + 1 end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" or event.GameButton == "Center" then
        local m = missions[current_index]
        if m.Status == "Complete" then
             if MISSIONMAN:ClaimReward(m.ID) then
                SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
                SCREENMAN:SystemMessage("Reward Claimed!")
                SCREENMAN:SetNewScreen("ScreenMissionSelect") -- Refresh
             end
        elseif m.Status == "Active" then
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
            SCREENMAN:SetNewScreen("ScreenSelectMusic") -- Proceed to attempt mission
        else
            SOUND:PlayOnce(THEME:GetPathS("Common", "cancel"))
        end
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    end

    return true
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
    MESSAGEMAN:Broadcast("UpdateSelection")
end

return t
