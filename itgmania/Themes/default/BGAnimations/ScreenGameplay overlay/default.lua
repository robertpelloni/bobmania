local t = Def.ActorFrame {}

-- Load existing gameplay elements (simplified for this task)
-- t[#t+1] = LoadActor("...existing...")

-- Load our Mining Hook
t[#t+1] = LoadActor("MiningHook")

return t
