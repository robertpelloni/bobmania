local choices = {
    { text = "Arcade", screen = "ScreenTitleMenu", help = "The classic StepMania gameplay experience." },
    { text = "Gym Mode", screen = "ScreenGymWelcome", help = "Track fitness goals, calories, and structured workouts." },
    { text = "Tournament Ladder", screen = "ScreenTournamentLadder", help = "Compete globally in ranked matches." },
    { text = "Marketplace", screen = "ScreenMarketplace", help = "Spend Bobcoins on songs, themes, and items." },
    { text = "Wallet History", screen = "ScreenWalletHistory", help = "View your Bobcoin balance and transaction ledger." },
    { text = "Missions", screen = "ScreenMissionSelect", help = "Complete daily objectives for rewards." },
    { text = "Swarm Network", screen = "ScreenContentNetwork", help = "Discover and download community content P2P." },
    { text = "Spectate TV", screen = "ScreenSpectate", help = "Watch high-level matches live and chat." },
    { text = "Replays", screen = "ScreenReplayMenu", help = "Review past performances and race against ghosts." },
    { text = "Asset Sync", screen = "ScreenAssetSync", help = "Synchronize local items and stats with the cloud." },
    { text = "Unified Settings", screen = "ScreenUnifiedOptions", help = "Configure advanced options (Ghost Tapping, FOV)." },
    { text = "Exit", screen = "ScreenExit", help = "Quit Unified StepMania." }
}

local current_index = 1
local t = Def.ActorFrame {}

-- Help Overlay Integration
t[#t+1] = HelpOverlay.Create()
HelpOverlay.Register("ScreenUnifiedDashboard", {
    Description = "Unified Dashboard\n\nNavigate through all major modules.\nUse Up/Down to select an option.\nSTART to confirm.\nSELECT for detailed help on the current option."
})

t[#t+1] = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Help Prompt
    LoadFont("Common Normal")..{
        Text="Press SELECT for Help",
        InitCommand=function(self) self:xy(300, -220):zoom(0.6):diffuse(0.5,0.5,0.5,1):horizalign(right) end,
        HelpToggleMessageCommand=function(self) self:visible(not self:GetVisible()) end
    },

    -- Title
    LoadFont("Common Normal")..{
        Text="UNIFIED DASHBOARD",
        InitCommand=function(self) self:y(-200):zoom(1.5):diffuse(0,1,1,1) end
    },

    -- Subtitle
    LoadFont("Common Normal")..{
        Text="Select a Module",
        InitCommand=function(self) self:y(-170):zoom(0.8):diffuse(0.7,0.7,0.7,1) end
    },

    -- Selection Indicator (Simple Arrow)
    LoadFont("Common Normal")..{
        Text=">",
        InitCommand=function(self) self:x(-150):zoom(1.5):diffuse(Color.Orange) end,
        UpdateSelectionMessageCommand=function(self)
            self:finishtweening():linear(0.1):y((current_index - 1) * 35 - 130)
        end
    },

    -- Dynamic Help Text Box for Selected Item
    Def.ActorFrame {
        InitCommand=function(self) self:y(200) end,

        Def.Quad {
            InitCommand=function(self) self:zoomto(600, 60):diffuse(0,0,0,0.5) end
        },

        LoadFont("Common Normal")..{
            InitCommand=function(self) self:zoom(0.7):wrapwidthpixels(580):diffuse(0.8,0.8,0.8,1) end,
            UpdateSelectionMessageCommand=function(self)
                self:settext(choices[current_index].help)
            end
        }
    }
}

-- Generate Menu Items
for i, choice in ipairs(choices) do
    t[#t+1] = LoadFont("Common Normal")..{
        Text=choice.text,
        InitCommand=function(self)
            self:Center():y((i - 1) * 35 - 130):zoom(1)
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
        local screen = choices[current_index].screen
        if screen == "ScreenExit" then
            SCREENMAN:SetNewScreen("ScreenExit")
        else
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
            SCREENMAN:SetNewScreen(screen)
        end
        return true
    elseif event.GameButton == "Select" then
        MESSAGEMAN:Broadcast("HelpToggle")
        SOUND:PlayOnce(THEME:GetPathS("Common", "value"))
        return true
    end

    return false
end

t.OnCommand=function(self)
    SCREENMAN:GetTopScreen():AddInputCallback(InputHandler)
    MESSAGEMAN:Broadcast("UpdateSelection")

    if DiscordRPC then
        DiscordRPC.SetPresence("In Menus", "Unified Dashboard")
    end
end

return t
