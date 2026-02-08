local function AwardMining(score, meter)
    if not ECONOMYMAN then return end
    ECONOMYMAN:AwardMiningReward(score, meter)
    -- Optional: SystemMessage("Mining: " .. score .. "% Reward")
end

local t = Def.ActorFrame {
    OnCommand=function(self)
        -- Listen for stage end
        -- We can't rely on global MESSAGEMAN for everything in older SM5 versions,
        -- but ScreenGameplay usually broadcasts "Off" or "Out" when leaving.
    end,

    -- When the screen transitions out (Stage Finished)
    OffCommand=function(self)
        if not GAMESTATE:IsCourseMode() then -- Simplify for now
            for pn in ivalues(GAMESTATE:GetHumanPlayers()) do
                local pss = STATSMAN:GetCurStageStats():GetPlayerStageStats(pn)
                if pss and not pss:GetFailed() then
                    local score = pss:GetPercentDancePoints()
                    local steps = GAMESTATE:GetCurrentSteps(pn)
                    local meter = 1
                    if steps then meter = steps:GetMeter() end

                    AwardMining(score, meter)
                end
            end
        end
    end
}

return t
