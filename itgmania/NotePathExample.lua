-- Example usage of SetNotePath
-- This script can be run in a ScreenGameplay overlay or a modfile

local function MyNotePath(beat, col)
    -- Simple sine wave effect
    local x = math.sin(beat + col) * 50
    local y = 0 -- Standard Y position is handled by the engine if we return 0? No, we need to return the absolute Y.
    -- Wait, GetXYZPos replaces the standard position calculation.
    -- So we need to calculate the standard Y if we want it.
    -- But we don't have access to the standard Y calculation here easily without reimplementing it.
    
    -- Actually, NotITG's SetNotePath usually works as an offset or absolute?
    -- In NotITG, SetNotePath overrides the entire position calculation.
    -- So the user is responsible for calculating Y (scrolling).
    
    -- To simulate standard scrolling:
    -- y = (beat - CurrentSongBeat) * Speed * PixelsPerBeat
    
    -- But we don't have CurrentSongBeat passed in.
    -- We can get it from GAMESTATE:GetSongPosition():GetSongBeat()
    
    local song_beat = GAMESTATE:GetSongPosition():GetSongBeat()
    local y = (beat - song_beat) * 100 -- Simple 100 pixels per beat scroll
    
    local z = 0
    
    return x, y, z
end

-- Apply to Player 1
local ps = GAMESTATE:GetPlayerState(PLAYER_1)
ps:SetNotePath(MyNotePath)

-- To clear:
-- ps:SetNotePath(nil)
