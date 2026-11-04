-- ScreenSplineEditor overlay
-- Debug UI for manipulating NotePath 3D Splines (NotITG Parity)

local t = Def.ActorFrame{}

t[#t+1] = Def.Quad{
    InitCommand=cmd(FullScreen; diffuse,color("0,0,0,0.5")),
}

t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Unified Spline Editor (Debug Mode)",
    InitCommand=cmd(x, 150; y, 20; zoom, 1.0; diffuse, color("1,0.5,0,1")),
}

-- Simulated currently selected NoteField
local mockNoteField = {
    path = {},
    GetNotePath = function(self) return self.path end
}

-- Add stubs to the mock path so the UI code doesn't crash if the C++ backend isn't loaded
function mockNoteField.path:Clear() end
function mockNoteField.path:AddControlPoint(y, px, py, pz, rx, ry, rz) end
function mockNoteField.path:BuildSpline() end

local controlPoints = {
    {y = 0,   px = 0,   py = 0, pz = 0, rx = 0, ry = 0, rz = 0},
    {y = 100, px = 50,  py = 0, pz = 0, rx = 0, ry = 0, rz = 0},
    {y = 200, px = -50, py = 0, pz = 0, rx = 0, ry = 0, rz = 0},
    {y = 300, px = 0,   py = 0, pz = 0, rx = 0, ry = 0, rz = 0},
}

local selectedPoint = 1

local function UpdateSpline()
    if NotePathHelper then
        -- Convert table to the array format NotePathHelper expects
        local arr = {}
        for i, cp in ipairs(controlPoints) do
            table.insert(arr, {cp.y, cp.px, cp.py, cp.pz, cp.rx, cp.ry, cp.rz})
        end
        -- In a real environment, noteFieldActor would be fetched from SCREENMAN
        NotePathHelper.InjectSpline(mockNoteField, arr)
    end
end

t[#t+1] = Def.Actor{
    InitCommand=function(self)
        UpdateSpline()
    end
}

-- Render UI Editor list
for i, cp in ipairs(controlPoints) do
    local yPos = 60 + (i * 30)
    t[#t+1] = Def.BitmapText{
        Font="Common Normal",
        Text=string.format("Pt %d: Y-Off:%.0f  [X:%.0f  Y:%.0f  Z:%.0f]", i, cp.y, cp.px, cp.py, cp.pz),
        InitCommand=cmd(x, 200; y, yPos; diffuse, color("1,1,1,1")),
        OnCommand=function(self)
            self:playcommand("UpdateSelection")
        end,
        UpdateSelectionCommand=function(self)
            if i == selectedPoint then
                self:diffuse(color("0,1,0,1"))
            else
                self:diffuse(color("0.5,0.5,0.5,1"))
            end
            self:sleep(0.1):queuecommand("UpdateSelection")
        end
    }
end

-- Instructions
t[#t+1] = Def.BitmapText{
    Font="Common Normal",
    Text="Use UP/DOWN to select points.\nUse LEFT/RIGHT to shift X offset.\n(UI Simulator)",
    InitCommand=cmd(x, 150; y, SCREEN_BOTTOM - 60; diffuse, color("0.8,0.8,0.8,1")),
    OnCommand=function(self)
        SCREENMAN:GetTopScreen():AddInputCallback(function(event)
            if event.type == "InputEventType_FirstPress" then
                if event.button == "Up" then
                    selectedPoint = math.max(1, selectedPoint - 1)
                elseif event.button == "Down" then
                    selectedPoint = math.min(#controlPoints, selectedPoint + 1)
                elseif event.button == "Left" then
                    controlPoints[selectedPoint].px = controlPoints[selectedPoint].px - 10
                    UpdateSpline()
                elseif event.button == "Right" then
                    controlPoints[selectedPoint].px = controlPoints[selectedPoint].px + 10
                    UpdateSpline()
                end
            end
        end)
    end
}

return t
