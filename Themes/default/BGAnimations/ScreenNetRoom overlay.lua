local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()

-- Chat Helper
local function UpdateChat(self)
    self:runcommandonchildren("UpdateMessages")
end

-- Background
t[#t+1] = Def.Quad {
    InitCommand = function(self) self:zoomto(SCREEN_WIDTH, SCREEN_HEIGHT):Center():diffuse(0,0,0,1) end
}

-- Layout
t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Title
    LoadFont("Common Normal")..{
        Text="NetPlay Lobby",
        InitCommand=function(self) self:y(-200):zoom(1.5):diffuse(Color.Cyan) end
    },

    -- Status
    LoadFont("Common Normal")..{
        Text="Status: Connected",
        InitCommand=function(self) self:y(220):zoom(0.6):diffuse(Color.Green) end
    },

    -- Chat Box
    Def.ActorFrame {
        InitCommand=function(self) self:xy(100, 50) end,

        Def.Quad {
            InitCommand=function(self) self:zoomto(400, 200):diffuse(0,0,0,0.5) end
        },

        Def.ActorFrame {
            Name = "ChatDisplay",
            InitCommand=function(self) self:xy(-190, -90) end,
            NewChatMessageMessageCommand=function(self) self:playcommand("UpdateMessages") end,
            UpdateMessagesCommand=function(self)
                if not CHATMAN then return end
                local msgs = CHATMAN:GetMessages()
                -- Display last 10 messages
                self:RemoveAllChildren()

                local startIdx = math.max(1, #msgs - 9)
                for i=startIdx, #msgs do
                    local m = msgs[i]
                    local yPos = (i - startIdx) * 18

                    local line = LoadFont("Common Normal") .. {
                        Text = "["..m.Time.."] " .. m.User .. ": " .. m.Text,
                        InitCommand = function(s) s:y(yPos):zoom(0.6):halign(0):maxwidth(600) end
                    }
                    self:AddChild(line)
                end
            end
        }
    }
}

-- Mock Input Box Hint
t[#t+1] = LoadFont("Common Normal") .. {
    Text = "Press START to Type Message\nPress BACK to Exit",
    InitCommand = function(self) self:xy(SCREEN_CENTER_X, SCREEN_HEIGHT - 50):zoom(0.8) end
}

-- Input Handler
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "Back" then
        SCREENMAN:SetNewScreen("ScreenUnifiedDashboard")
    elseif event.GameButton == "Start" then
        -- Mock Typing
        local msgs = { "Hello everyone!", "Ready for a match?", "GG!", "Nice combo!" }
        local msg = msgs[math.random(#msgs)]
        if CHATMAN then CHATMAN:SendChat(msg) end
    elseif event.GameButton == "Select" then
        MESSAGEMAN:Broadcast("HelpToggle")
    end

    return true
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
    -- Initial Refresh
    MESSAGEMAN:Broadcast("NewChatMessage")
end

return t
