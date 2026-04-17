-- ScreenMissionSelect overlay
-- Displays active missions and allows claiming rewards via the server.

local t = Def.ActorFrame{}

t[#t+1] = Def.Quad{
    InitCommand=cmd(FullScreen; diffuse,color("0.1,0.1,0.1,0.9")),
}

t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Mission Control Center",
    InitCommand=cmd(x, SCREEN_CENTER_X; y, 40; zoom, 1.5; diffuse, color("1,1,1,1")),
}

-- Hardcoded UI display for the mock missions created in MissionManager.cpp
local mockMissions = {
    {id="daily_notes", name="Step Master", reward="100 Bobcoins"},
    {id="daily_calories", name="Cardio Burn", reward="250 Bobcoins"}
}

for i, mission in ipairs(mockMissions) do
    local yPos = 100 + (i * 60)

    t[#t+1] = Def.BitmapText{
        Font="Common Normal",
        Text=mission.name .. " - Reward: " .. mission.reward,
        InitCommand=cmd(x, SCREEN_CENTER_X - 100; y, yPos; halign, 0; diffuse, color("0.8,0.8,0.8,1")),
    }

    t[#t+1] = Def.BitmapText{
        Font="Common Normal",
        Text="[ CLAIM ]",
        InitCommand=cmd(x, SCREEN_CENTER_X + 200; y, yPos; diffuse, color("0,1,0,1")),
        OnCommand=function(self)
            -- Check completion status every frame (or via broadcast usually)
            self:playcommand("UpdateStatus")
        end,
        UpdateStatusCommand=function(self)
            if CheckMissionComplete(mission.id) then
                self:diffuse(color("0,1,0,1"))
            else
                self:diffuse(color("0.5,0.5,0.5,1"))
            end
            self:sleep(0.5):queuecommand("UpdateStatus")
        end
    }
end

t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Press START to claim 'Cardio Burn' (Mock)",
    InitCommand=cmd(x, SCREEN_CENTER_X; y, SCREEN_BOTTOM - 50; diffuse, color("1,1,0,1"); pulse; effectmagnitude, 1.0, 1.1, 0; effectclock, "beat"),
    OnCommand=function(self)
        SCREENMAN:GetTopScreen():AddInputCallback(function(event)
            if event.type == "InputEventType_FirstPress" and event.button == "Start" then
                local success = AttemptClaimReward("daily_calories")
                if success then
                    SCREENMAN:SystemMessage("Reward Claimed! Validated by Server.")
                else
                    SCREENMAN:SystemMessage("Failed to claim reward.")
                end
            end
        end)
    end
}

return t
