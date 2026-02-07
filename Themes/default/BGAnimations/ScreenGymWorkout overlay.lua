local playlists = GetWorkoutPlaylists()
local current_index = 1
local t = Def.ActorFrame {}

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header
    LoadFont("Common Normal")..{
        Text="Select Workout",
        InitCommand=function(self) self:y(-200):zoom(1.5) end
    },

    -- Playlist Grid
    Def.ActorFrame {
        InitCommand=function(self) self:y(-50) end,
        -- Generate Items
        unpack((function()
            local children = {}
            for i, p in ipairs(playlists) do
                local row = i - 1

                children[#children+1] = Def.ActorFrame {
                    InitCommand=function(self)
                        self:xy(0, row*50)
                    end,

                    -- Background
                    Def.Quad {
                        InitCommand=function(self) self:zoomto(400, 40):diffuse(0,0,0,0.5) end,
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
                        Text=p.Name,
                        InitCommand=function(self) self:x(-180):zoom(1):halign(0) end
                    },

                    -- Description
                    LoadFont("Common Normal")..{
                        Text=p.Description,
                        InitCommand=function(self) self:x(-180):y(12):zoom(0.6):halign(0):diffuse(0.7,0.7,0.7,1) end
                    },

                    -- Stats (Time & Difficulty)
                    LoadFont("Common Normal")..{
                        Text=p.Duration .. " | " .. p.Difficulty,
                        InitCommand=function(self) self:x(180):zoom(0.8):halign(1):diffuse(Color.Orange) end
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
        if current_index < #playlists then current_index = current_index + 1 end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" or event.GameButton == "Center" then
        SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
        -- Here we would load the course/playlist
        SCREENMAN:SetNewScreen("ScreenSelectMusic")
    elseif event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenGymWelcome")
    end

    return true
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
    MESSAGEMAN:Broadcast("UpdateSelection")
end

return t
