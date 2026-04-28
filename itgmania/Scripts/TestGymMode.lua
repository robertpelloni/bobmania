-- Test script for GymPlaylistGenerator
local duration = 600 -- 10 minutes
local intensity = "WorkoutIntensity_Moderate"

Trace("Generating Gym Circuit...")
local course = GymPlaylistGenerator:GenerateCircuit(duration, intensity)

if course then
    Trace("Course Generated: " .. course:GetTitle())
    local entries = course:GetCourseEntries()
    Trace("Number of songs: " .. #entries)
    
    for i, entry in ipairs(entries) do
        local song = entry:GetSong()
        if song then
            Trace("Song " .. i .. ": " .. song:GetMainTitle())
            local calories = GymPlaylistGenerator:EstimateCalories(song, 1.0)
            Trace("  Estimated Calories: " .. calories)
        end
    end

    -- Test Writing
    local path = "Save/TestWorkout.crs"
    Trace("Writing course to " .. path)
    local success = GymCourseWriter:WriteCourse(course, path)
    if success then
        Trace("Successfully wrote course file.")
    else
        Trace("Failed to write course file.")
    end
else
    Trace("Failed to generate course.")
end
