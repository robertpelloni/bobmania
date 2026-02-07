-- Gym & Fitness API Wrappers

function GetGymProfile(pn)
    return {
        Weight = 75.5, -- kg
        Height = 180, -- cm
        BMI = 23.3,
        TotalCaloriesBurned = 15430,
        StreakDays = 5,
        DailyGoal = 500,
        TodayCalories = 120
    }
end

function GetWorkoutPlaylists()
    return {
        { Name="Cardio Blast", Duration="20 min", Difficulty="Hard", Description="High BPM stamina stream." },
        { Name="Warm Up", Duration="10 min", Difficulty="Easy", Description="Low BPM flow." },
        { Name="Leg Day", Duration="30 min", Difficulty="Medium", Description="Tech and jumps." },
        { Name="Endurance", Duration="60 min", Difficulty="Very Hard", Description="Non-stop marathon." }
    }
end
