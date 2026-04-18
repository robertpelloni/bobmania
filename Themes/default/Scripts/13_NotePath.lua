-- Unified StepMania - NotITG Parity Lua Bindings
-- Allows dynamic injection of control points into the NotePath spline for modding.

NotePathHelper = {}

-- Helper to inject a massive table of points into a specific NoteField path
function NotePathHelper.InjectSpline(noteFieldActor, pointsTable)
    if not noteFieldActor or not noteFieldActor.GetNotePath then
        Warn("Actor is not a NoteField or does not support NotePaths.")
        return
    end

    local path = noteFieldActor:GetNotePath()
    path:Clear()

    for _, pt in ipairs(pointsTable) do
        -- yOffset, px, py, pz, rx, ry, rz
        path:AddControlPoint(pt[1], pt[2] or 0, pt[3] or 0, pt[4] or 0, pt[5] or 0, pt[6] or 0, pt[7] or 0)
    end

    path:BuildSpline()
end
