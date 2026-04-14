# External Engine Integration Required

**Note to developers and CI:**
The `HeartRateManager` (and `IHeartRateDriver`) have been implemented and bound to Lua (`LunaHeartRateManager`). However, the core engine files `src/StepMania.cpp` and `src/GameLoop.cpp` are currently absent from this fork's root tree (likely existing in an upstream/parent repo that links this code).

**Required Action upon full repo compilation:**
1. In `src/StepMania.cpp` (or equivalent entry point), you MUST instantiate the singleton:
   `HEARTRATEMAN = new HeartRateManager;`
   `HEARTRATEMAN->Init();`
   And clean it up on shutdown:
   `delete HEARTRATEMAN;`
2. In `src/GameLoop.cpp` (or equivalent main update loop), you MUST call:
   `HEARTRATEMAN->Update(fDeltaTime);`
