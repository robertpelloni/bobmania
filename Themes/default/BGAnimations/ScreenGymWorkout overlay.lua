--[[
  ScreenGymWorkout overlay.
  Hooks into the gameplay loop to visualize the active heart rate from HEARTRATEMAN
]]

local t = Def.ActorFrame{
    OnCommand=function(self)
        self:SetUpdateFunction( function(self)
            -- Ask the global Lua script wrapper for the current BPM
            local bpm = GetCurrentHeartRate()

            -- Find the text actor and update it
            local textActor = self:GetChild("BPMDisplay")
            if textActor then
                if bpm > 0 then
                    textActor:settext( "Heart Rate: " .. tostring(bpm) .. " BPM" )

                    -- Pulse effect scaling with BPM
                    local scale = 1.0 + (bpm / 200.0) * 0.2
                    textActor:zoom(scale)
                else
                    textActor:settext( "Searching for HR Monitor..." )
                end
            end
        end )
    end
}

t[#t+1] = LoadFont("Common Normal") .. {
    Name="BPMDisplay",
    Text="Loading...",
    InitCommand=cmd(xy, SCREEN_RIGHT - 150, SCREEN_TOP + 50; zoom, 1.0; diffuse, color("#FF3333"))
}

return t
