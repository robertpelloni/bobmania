local function AwardMining(score, meter)
    if not ECONOMYMAN then return end
    ECONOMYMAN:AwardMiningReward(score, meter)
end

local function UpdateMissions(pn)
    if not MISSIONMAN then return end

    -- Report Song Clear
    MISSIONMAN:ReportMetric("SongsPassed", 1)

    -- Report Notes Hit (Simplified)
    local pss = STATSMAN:GetCurStageStats():GetPlayerStageStats(pn)
    if pss then
        local hits = pss:GetTapNoteScores('TapNoteScore_W1') +
                     pss:GetTapNoteScores('TapNoteScore_W2') +
                     pss:GetTapNoteScores('TapNoteScore_W3')
        MISSIONMAN:ReportMetric("NotesHit", hits)

        -- Report Calories
        local cals = pss:GetCaloriesBurned()
        MISSIONMAN:ReportMetric("Calories", cals)

        -- Also log to GymManager
        if GYMMAN then
            local dur = pss:GetAliveSeconds() / 60.0
            GYMMAN:LogWorkout("Arcade", dur, cals)
        end
    end
end

local t = Def.ActorFrame {
    OffCommand=function(self)
        if not GAMESTATE:IsCourseMode() then
            for pn in ivalues(GAMESTATE:GetHumanPlayers()) do
                local pss = STATSMAN:GetCurStageStats():GetPlayerStageStats(pn)
                if pss and not pss:GetFailed() then
                    local score = pss:GetPercentDancePoints()
                    local steps = GAMESTATE:GetCurrentSteps(pn)
                    local meter = 1
                    if steps then meter = steps:GetMeter() end

                    AwardMining(score, meter)
                    UpdateMissions(pn)
                end
            end
        end
    end
}

return t
