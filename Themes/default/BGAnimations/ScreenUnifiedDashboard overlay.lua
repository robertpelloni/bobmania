-- ScreenUnifiedDashboard overlay.lua
-- Unified Dashboard: Single consolidated command center for all StepMania modules.
-- Organized by high-value prominent placement with intuitive categories, tooltips,
-- direct server status indicators, and two-column consolidated navigation.

local categories = {
    {
        name = "CORE RHYTHM & FITNESS",
        color = {0, 0.9, 1, 1},
        col = 1,
        items = {
            { text = "Arcade Mode", screen = "ScreenSelectMusic", tag = "[CORE]", help = "Standard StepMania rhythm gameplay. Choose songs, select difficulty, and dance." },
            { text = "Gym & Fitness", screen = "ScreenGymWelcome", tag = "[FIT]", help = "Structured workouts, real-time calorie tracking, and heart-rate monitoring dashboard." },
            { text = "Tournament Ladder", screen = "ScreenTournamentLadder", tag = "[RANK]", help = "Compete in global ranked ELO matchmaking and tournament ladders." },
            { text = "Online NetRoom", screen = "ScreenNetRoom", tag = "[NET]", help = "Join multiplayer lobbies, connect to remote servers, and compete head-to-head." }
        }
    },
    {
        name = "ECONOMY & PROGRESSION",
        color = {0.2, 1, 0.4, 1},
        col = 1,
        items = {
            { text = "Marketplace", screen = "ScreenMarketplace", tag = "[SHOP]", help = "Spend Bobcoins earned from dancing to unlock custom songs, themes, and modifiers." },
            { text = "Wallet Ledger", screen = "ScreenWalletHistory", tag = "[BOB]", help = "Inspect Bobcoin wallet balance, block confirmations, and transaction history." },
            { text = "Missions & Quests", screen = "ScreenMissionSelect", tag = "[QUEST]", help = "Complete daily fitness and rhythm challenges for bonus token rewards." },
            { text = "Cloud Asset Sync", screen = "ScreenAssetSync", tag = "[CLOUD]", help = "Synchronize local scores, profiles, and unlocked assets with the decentralized cloud." }
        }
    },
    {
        name = "MEDIA & SYSTEM TOOLS",
        color = {1, 0.7, 0.2, 1},
        col = 2,
        items = {
            { text = "Spectator TV", screen = "ScreenSpectate", tag = "[LIVE]", help = "Watch live high-level tournament streams, spectate matches, and chat in real-time." },
            { text = "Replay Theater", screen = "ScreenReplayMenu", tag = "[GHOST]", help = "Review past performances, inspect millisecond timing, and race against ghost data." },
            { text = "P2P Content Swarm", screen = "ScreenContentNetwork", tag = "[P2P]", help = "Discover, seed, and download community song packs via peer-to-peer swarm transfer." },
            { text = "Unified Settings", screen = "ScreenUnifiedOptions", tag = "[CONF]", help = "Configure advanced engine options (Ghost Tapping, Both At Once, FOV overrides, VR)." },
            { text = "Exit / Server Shutdown", screen = "ScreenExit", tag = "[EXIT]", help = "Gracefully stop background network/RPC services, close connections, and exit to desktop." }
        }
    }
}

-- Compute coordinates for each item
local flat_choices = {}
local col_offsets = {
    [1] = { x = -160, start_y = -130, spacing = 32 },
    [2] = { x = 160, start_y = -130, spacing = 32 }
}
local col_counters = { [1] = 0, [2] = 0 }

for cat_idx, cat in ipairs(categories) do
    local col = cat.col
    for item_idx, item in ipairs(cat.items) do
        local count = col_counters[col]
        local x = col_offsets[col].x
        local y = col_offsets[col].start_y + (count * col_offsets[col].spacing)
        col_counters[col] = count + 1

        item.category_index = cat_idx
        item.category_name = cat.name
        item.category_color = cat.color
        item.rel_x = x
        item.rel_y = y
        flat_choices[#flat_choices + 1] = item
    end
end

local current_index = 1
local t = Def.ActorFrame {}

-- Help Overlay Registration
if HelpOverlay and HelpOverlay.Create then
    t[#t+1] = HelpOverlay.Create()
    HelpOverlay.Register("ScreenUnifiedDashboard", {
        Description = "UNIFIED DASHBOARD (Consolidated Command Center)\n\n" ..
                      "Navigate through all engine systems in one singular screen.\n" ..
                      "* Use Up/Down or Left/Right to cycle options.\n" ..
                      "* Press START / CENTER to launch selected module.\n" ..
                      "* Press SELECT for detailed tooltip explanation.\n" ..
                      "* High-value core modes (Arcade, Gym, Tournament, Online) are placed at the top."
    })
end

-- Center Container for the entire dashboard UI
local dashboardContainer = Def.ActorFrame {
    InitCommand=function(self) self:Center() end,

    -- Header Banner & Title
    Def.Quad {
        InitCommand=function(self)
            self:xy(0, -210):zoomto(SCREEN_WIDTH, 50):diffuse(0.05, 0.08, 0.12, 0.9)
        end
    },
    LoadFont("Common Normal")..{
        Text="UNIFIED DASHBOARD",
        InitCommand=function(self)
            self:xy(-280, -212):zoom(1.1):diffuse(0, 0.9, 1, 1):horizalign(left)
        end
    },
    LoadFont("Common Normal")..{
        Text="Consolidated Command Center  |  SELECT: Help  |  START: Launch",
        InitCommand=function(self)
            self:xy(280, -212):zoom(0.6):diffuse(0.7, 0.7, 0.7, 1):horizalign(right)
        end
    },

    -- Column 1 Background Panel
    Def.Quad {
        InitCommand=function(self)
            self:xy(-160, -18):zoomto(300, 290):diffuse(0.02, 0.04, 0.07, 0.7)
        end
    },

    -- Column 2 Background Panel
    Def.Quad {
        InitCommand=function(self)
            self:xy(160, -18):zoomto(300, 290):diffuse(0.02, 0.04, 0.07, 0.7)
        end
    },

    -- Selection Pointer Box (relative coordinates inside dashboardContainer)
    Def.Quad {
        Name="Selector",
        InitCommand=function(self)
            self:zoomto(290, 28):diffuse(0, 0.6, 0.9, 0.35)
        end,
        UpdateSelectionMessageCommand=function(self)
            local sel = flat_choices[current_index]
            self:finishtweening():easeoutquad(0.08):xy(sel.rel_x, sel.rel_y)
        end
    },

    -- Dynamic Tooltip & Explanation Bar (Bottom)
    Def.ActorFrame {
        InitCommand=function(self) self:y(180) end,

        Def.Quad {
            InitCommand=function(self)
                self:zoomto(SCREEN_WIDTH - 40, 56):diffuse(0.03, 0.05, 0.08, 0.95)
            end
        },

        LoadFont("Common Normal")..{
            Name="TooltipCategory",
            InitCommand=function(self)
                self:xy(-(SCREEN_WIDTH - 80) / 2, -14):zoom(0.55):diffuse(Color.Orange):horizalign(left)
            end,
            UpdateSelectionMessageCommand=function(self)
                local sel = flat_choices[current_index]
                self:settext(sel.tag .. " [" .. sel.category_name .. "]")
            end
        },

        LoadFont("Common Normal")..{
            Name="TooltipText",
            InitCommand=function(self)
                self:xy(-(SCREEN_WIDTH - 80) / 2, 8):zoom(0.65):wrapwidthpixels(SCREEN_WIDTH - 100):diffuse(1, 1, 1, 1):horizalign(left)
            end,
            UpdateSelectionMessageCommand=function(self)
                self:settext(flat_choices[current_index].help)
            end
        }
    }
}

-- Render Menu Choices inside the dashboardContainer ActorFrame
for i, choice in ipairs(flat_choices) do
    dashboardContainer[#dashboardContainer+1] = Def.ActorFrame {
        InitCommand=function(self) self:xy(choice.rel_x, choice.rel_y) end,

        -- Module Tag / Badge
        LoadFont("Common Normal")..{
            Text=choice.tag,
            InitCommand=function(self)
                self:x(-135):zoom(0.65):horizalign(left)
            end,
            UpdateSelectionMessageCommand=function(self)
                if i == current_index then
                    self:diffuse(Color.Orange)
                else
                    self:diffuse(0.4, 0.5, 0.6, 0.9)
                end
            end
        },

        -- Item Title
        LoadFont("Common Normal")..{
            Text=choice.text,
            InitCommand=function(self)
                self:x(-80):zoom(0.75):horizalign(left)
            end,
            UpdateSelectionMessageCommand=function(self)
                if i == current_index then
                    self:diffuse(1, 1, 1, 1):zoom(0.85)
                else
                    self:diffuse(0.7, 0.7, 0.7, 1):zoom(0.75)
                end
            end
        }
    }
end

t[#t+1] = dashboardContainer

-- Input Handling for Dashboard
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" or event.GameButton == "Down" then
        current_index = current_index + 1
        if current_index > #flat_choices then current_index = 1 end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
        return true
    elseif event.GameButton == "MenuUp" or event.GameButton == "Up" then
        current_index = current_index - 1
        if current_index < 1 then current_index = #flat_choices end
        MESSAGEMAN:Broadcast("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
        return true
    elseif event.GameButton == "MenuLeft" or event.GameButton == "Left" then
        if current_index > 8 then
            current_index = math.max(1, current_index - 8)
            MESSAGEMAN:Broadcast("UpdateSelection")
            SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
            return true
        end
    elseif event.GameButton == "MenuRight" or event.GameButton == "Right" then
        if current_index <= 8 then
            current_index = math.min(#flat_choices, current_index + 8)
            MESSAGEMAN:Broadcast("UpdateSelection")
            SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
            return true
        end
    elseif event.GameButton == "Start" or event.GameButton == "Center" then
        local screen = flat_choices[current_index].screen
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
