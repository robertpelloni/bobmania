-- ScreenReplayMenu overlay
-- This serves as the UI for selecting and watching Ghost Replays.

local t = Def.ActorFrame{}

t[#t+1] = Def.Quad{
    InitCommand=cmd(FullScreen; diffuse,color("0,0,0,0.8")),
}

t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Replay Viewer",
    InitCommand=cmd(x, SCREEN_CENTER_X; y, 50; zoom, 1.5; diffuse, color("1,1,1,1")),
}

-- Mock a simple button that loads a replay and transitions to ScreenGameplay
t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Press START to Watch Latest Replay",
    InitCommand=cmd(x, SCREEN_CENTER_X; y, SCREEN_CENTER_Y; diffuse, color("1,1,0,1"); pulse; effectmagnitude, 1.0, 1.1, 0; effectclock, "beat"),
    OnCommand=function(self)
        SCREENMAN:GetTopScreen():AddInputCallback(function(event)
            if event.type == "InputEventType_FirstPress" and event.button == "Start" then
                -- Hardcoded path for the mock, would read from a directory index normally
                if LoadGhostReplay("Save/Replays/latest.csv") then
                    SCREENMAN:SetNewScreen("ScreenGameplay")
                end
            end
        end)
    end
}

return t
