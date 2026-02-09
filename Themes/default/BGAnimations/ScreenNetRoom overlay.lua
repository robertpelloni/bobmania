local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()

-- Help Text
HelpOverlay.Register("ScreenNetRoom", {
    Description = "Online Lobby\n\nChat with other players and join rooms.\nSelect a room to enter game setup.\nRequires a network connection."
})

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Title
    LoadFont("Common Normal")..{
        Text="NetPlay Lobby",
        InitCommand=function(self) self:y(-200):zoom(1.5):diffuse(Color.Cyan) end
    },

    -- Status
    LoadFont("Common Normal")..{
        Text="Status: Connected (Lobby)",
        InitCommand=function(self) self:y(220):zoom(0.6):diffuse(Color.Green) end
    },

    -- Mock User List
    Def.ActorFrame {
        InitCommand=function(self) self:xy(-200, -50) end,

        LoadFont("Common Normal")..{
            Text="Players Online:",
            InitCommand=function(self) self:y(-20):zoom(0.8):halign(0):diffuse(Color.Yellow) end
        },

        LoadFont("Common Normal")..{
            Text="Player 1\nRhythmMaster\nNewbie\nArrowKing",
            InitCommand=function(self) self:y(20):zoom(0.7):halign(0):diffuse(1,1,1,1) end
        }
    },

    -- Mock Chat
    Def.ActorFrame {
        InitCommand=function(self) self:xy(100, 50) end,

        Def.Quad {
            InitCommand=function(self) self:zoomto(400, 200):diffuse(0,0,0,0.5) end
        },

        LoadFont("Common Normal")..{
            Text="System: Welcome to the lobby!\nRhythmMaster: Anyone up for a match?\nPlayer 1: Sure, let's play.",
            InitCommand=function(self) self:xy(-190, -90):zoom(0.6):halign(0):valign(0):wrapwidthpixels(600) end
        }
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
