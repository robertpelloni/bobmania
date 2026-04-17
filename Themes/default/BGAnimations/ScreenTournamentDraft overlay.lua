-- ScreenTournamentDraft overlay
-- Simulates the Ban/Pick phase before a ranked match begins.

local t = Def.ActorFrame{}

t[#t+1] = Def.Quad{
    InitCommand=cmd(FullScreen; diffuse,color("0.05,0.05,0.1,1")),
}

t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Tournament Draft Phase",
    InitCommand=cmd(x, SCREEN_CENTER_X; y, 40; zoom, 1.5; diffuse, color("1,1,1,1")),
}

-- Central status text
t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Waiting for opponent to Ban...",
    InitCommand=cmd(x, SCREEN_CENTER_X; y, 80; diffuse, color("1,0.5,0,1"); pulse; effectmagnitude, 1.0, 1.1, 0; effectclock, "beat"),
    UpdatePhaseCommand=function(self, params)
        self:settext(params.text)
        if params.color then self:diffuse(params.color) end
    end
}

-- Mock song list
local songList = {
    "Song A - 180 BPM",
    "Song B - 200 BPM",
    "Song C - 150 BPM",
    "Song D - 220 BPM",
    "Song E - 170 BPM"
}

-- Draw the list
for i, song in ipairs(songList) do
    local yPos = 150 + (i * 40)

    t[#t+1] = Def.BitmapText{
        Font="Common Normal",
        Text=song,
        InitCommand=cmd(x, SCREEN_CENTER_X; y, yPos; diffuse, color("1,1,1,1")),
        BanCommand=function(self)
            self:diffuse(color("0.5,0.5,0.5,1"))
            self:strikethrough(true)
        end,
        PickCommand=function(self)
            self:diffuse(color("0,1,0,1"))
            self:pulse()
            self:effectmagnitude(1.0, 1.1, 0)
        end
    }
end

-- State Machine Logic
t[#t+1] = Def.Actor{
    InitCommand=function(self)
        self:sleep(2.0):queuecommand("OpponentBan")
    end,
    OpponentBanCommand=function(self)
        -- Simulate network packet receiving opponent ban
        SCREENMAN:GetTopScreen():GetChild("Overlay"):playcommand("UpdatePhase", {text="Your Turn: Pick a Song", color=color("0,1,0,1")})

        -- Strike through Song B
        local songBText = SCREENMAN:GetTopScreen():GetChild("Overlay"):GetChild("")[6] -- Rough index
        if songBText then songBText:playcommand("Ban") end

        -- Enable local input
        SCREENMAN:GetTopScreen():AddInputCallback(function(event)
            if event.type == "InputEventType_FirstPress" and event.button == "Start" then
                SCREENMAN:GetTopScreen():GetChild("Overlay"):playcommand("UpdatePhase", {text="Match Starting...", color=color("1,1,1,1")})
                SCREENMAN:SetNewScreen("ScreenGameplay")
            end
        end)
    end
}

return t
