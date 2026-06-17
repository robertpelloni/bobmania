local t = Def.ActorFrame {}

t[#t+1] = HelpOverlay.Create()

-- Help Text
HelpOverlay.Register("ScreenContentNetwork", {
    Description = "Content Network\n\nDownload new songs and packs from the community swarm.\nSelect a pack to start downloading."
})

-- Data Fetch
local packs = {}
if SWARMMAN then
    packs = SWARMMAN:GetAvailablePacks()
end

local selectedIndex = 1

local function UpdateList(self)
    self:runcommandonchildren("UpdateSelection")
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
        Text="Swarm Network",
        InitCommand=function(self) self:y(-200):zoom(1.5):diffuse(Color.Orange) end
    },

    -- Seed Button
    LoadFont("Common Normal")..{
        Text="[S] SEED CURRENT SONG",
        InitCommand=function(self) self:xy(250, -220):zoom(0.6):diffuse(Color.Green) end
    },

    -- Status
    LoadFont("Common Normal")..{
        Text="Status: Searching for Peers...",
        InitCommand=function(self) self:y(220):zoom(0.6):diffuse(Color.Green) end
    },

    -- Pack List
    Def.ActorFrame {
        InitCommand=function(self) self:y(-100) end,
        UpdateSelectionCommand=UpdateList,

        unpack((function()
            local children = {}
            for i, p in ipairs(packs) do
                children[#children+1] = Def.ActorFrame {
                    InitCommand=function(self) self:y(i*40) end,
                    UpdateSelectionCommand=function(self)
                        if i == selectedIndex then
                            self:diffuse(1,1,1,1)
                            self:GetChild("BGLight"):visible(true)
                        else
                            self:diffuse(0.6,0.6,0.6,1)
                            self:GetChild("BGLight"):visible(false)
                        end
                    end,

                    Def.Quad {
                        Name="BGLight",
                        InitCommand=function(self) self:zoomto(500, 30):diffuse(1,1,1,0.1):visible(false) end
                    },

                    LoadFont("Common Normal")..{
                        Text=p.Name,
                        InitCommand=function(self) self:x(-200):zoom(0.8):halign(0) end
                    },

                    LoadFont("Common Normal")..{
                        Text=p.Author,
                        InitCommand=function(self) self:x(50):zoom(0.6):halign(0):diffuse(Color.Yellow) end
                    },

                    LoadFont("Common Normal")..{
                        Text=p.Seeders .. " Peers",
                        InitCommand=function(self) self:x(200):zoom(0.6):halign(1):diffuse(Color.Blue) end
                    }
                }
            end
            return children
        end)())
    }
}

t.InitCommand = function(self) self:queuecommand("UpdateSelection") end
t.UpdateSelectionCommand = UpdateList

-- Input Handler
local function InputHandler(event)
    if not event.PlayerNumber or not event.button then return false end
    if event.type ~= "InputEventType_FirstPress" then return false end

    if event.GameButton == "MenuDown" then
        selectedIndex = selectedIndex + 1
        if selectedIndex > #packs then selectedIndex = 1 end
        t:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "MenuUp" then
        selectedIndex = selectedIndex - 1
        if selectedIndex < 1 then selectedIndex = #packs end
        t:playcommand("UpdateSelection")
        SOUND:PlayOnce(THEME:GetPathS("Common", "change"))
    elseif event.GameButton == "Start" then
        local p = packs[selectedIndex]
        if p and SWARMMAN then
            SWARMMAN:RequestPack(p.ID)
            SCREENMAN:SystemMessage("Requested Pack: " .. p.Name)
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
        end
    elseif event.DeviceI == "DeviceButton_s" then
        if SWARMMAN and GAMESTATE:GetCurrentSong() then
            local s = GAMESTATE:GetCurrentSong()
            SWARMMAN:AddLocalPack(s:GetSongDir())
            SCREENMAN:SystemMessage("Seeding: " .. s:GetDisplayMainTitle())
            SOUND:PlayOnce(THEME:GetPathS("Common", "start"))
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

    if DiscordRPC then
        DiscordRPC.SetPresence("Browsing Community Packs", "Swarm Network")
    end
end

return t
