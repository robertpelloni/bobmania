local missions = {
    { Name="First Steps", Desc="Clear 3 songs on Beginner", Status="Complete", Reward="50 XP" },
    { Name="Stamina Test", Desc="Play a 10-minute marathon course", Status="Locked", Reward="100 XP" },
    { Name="Speed Demon", Desc="Clear a song with 2x Speed Mod", Status="Active", Reward="200 XP" },
    { Name="Boss Rush", Desc="Defeat 3 Boss Songs in a row", Status="Locked", Reward="Avatar Frame" }
}

local current_index = 1
local t = Def.ActorFrame {}

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
                        Text=m.Name,
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
                            elseif m.Status == "Locked" then self:diffuse(Color.Red)
                            else self:diffuse(Color.Yellow) end
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
        if missions[current_index].Status ~= "Locked" then
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
