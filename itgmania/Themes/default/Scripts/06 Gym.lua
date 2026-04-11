-- Gym & Fitness API Wrappers

function GetGymProfile(pn)
    if not GYMMAN then
        return { Weight=0, BMI=0, StreakDays=0, TodayCalories=0, TotalCaloriesBurned=0, DailyGoal=500 }
    end
    return GYMMAN:GetProfile()
end

function GetWorkoutPlaylists()
    -- Static lists are fine for now, but could be dynamic later
    return {
        { Name="Cardio Blast", Duration="20 min", Difficulty="Hard", Description="High BPM stamina stream." },
        { Name="Warm Up", Duration="10 min", Difficulty="Easy", Description="Low BPM flow." },
        { Name="Leg Day", Duration="30 min", Difficulty="Medium", Description="Tech and jumps." },
        { Name="Endurance", Duration="60 min", Difficulty="Very Hard", Description="Non-stop marathon." }
    }
end
