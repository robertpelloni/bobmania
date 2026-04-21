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

## EconomyManager Integration
To fully integrate the Unified `EconomyManager` (Bobcoin RPC bridge) into the core engine lifecycle:

**1. `src/StepMania.cpp`**
Inside `sm_main()` during singleton allocation:
```cpp
ECONOMYMAN = new EconomyManager;
ECONOMYMAN->Init();
```
During cleanup:
```cpp
ECONOMYMAN->Shutdown();
SAFE_DELETE(ECONOMYMAN);
```

## ReplayManager Integration
To fully integrate the Unified `ReplayManager` into the core engine lifecycle:

**1. `src/StepMania.cpp`**
Inside `sm_main()` during singleton allocation:
```cpp
REPLAYMAN = new ReplayManager;
REPLAYMAN->Init();
```
During cleanup:
```cpp
REPLAYMAN->Shutdown();
SAFE_DELETE(REPLAYMAN);
```

## MissionManager Integration
To fully integrate the Unified `MissionManager` into the core engine lifecycle:

**1. `src/StepMania.cpp`**
Inside `sm_main()` during singleton allocation:
```cpp
MISSIONMAN = new MissionManager;
MISSIONMAN->Init();
```
During cleanup:
```cpp
MISSIONMAN->Shutdown();
SAFE_DELETE(MISSIONMAN);
```

## ScoreKeeperUnified Integration
To completely decouple legacy DDR scoring and utilize the new Etterna-parity `ScoreKeeperUnified`, integrators must update `src/ScreenGameplay.cpp`.

When allocating the `ScoreKeeper` for the player, modify the factory check:
```cpp
// Inside ScreenGameplay::Init()
if (GAMESTATE->m_pCurGame->m_szName == RString("unified") || PREFSMAN->m_bEtternaScoring) {
    m_pScoreKeeper[p] = new ScoreKeeperUnified;
} else {
    m_pScoreKeeper[p] = new ScoreKeeperNormal;
}
```

Then in `src/Player.cpp` inside the hit logic:
```cpp
// Do not pass TNS_W1/W2 buckets. Pass the raw millisecond offset.
float fNoteOffset = fPositionSeconds - pTN->m_fBeat;
if (m_pScoreKeeper) {
    // If it's the Unified keeper, it expects a float offset and J-scale
    m_pScoreKeeper->HandleTapScore(fNoteOffset, GAMESTATE->GetPlayerState()->m_fJudgeScale);
}
```

## PluginManager Integration
To fully integrate the Unified `PluginManager` into the core engine lifecycle:

**1. `src/StepMania.cpp`**
Inside `sm_main()` during singleton allocation:
```cpp
PLUGINMAN = new PluginManager;
PLUGINMAN->Init();
```
During cleanup:
```cpp
PLUGINMAN->Shutdown();
SAFE_DELETE(PLUGINMAN);
```

**2. `src/GameLoop.cpp`**
Inside `GameLoop::Update()`:
```cpp
if(PLUGINMAN) {
    PLUGINMAN->Update(fDeltaTime);
}
```

## TournamentManager Integration
To fully integrate the Unified `TournamentManager` into the core engine lifecycle:

**1. `src/StepMania.cpp`**
Inside `sm_main()` during singleton allocation:
```cpp
TOURNAMENTMAN = new TournamentManager;
TOURNAMENTMAN->Init();
```
During cleanup:
```cpp
TOURNAMENTMAN->Shutdown();
SAFE_DELETE(TOURNAMENTMAN);
```

**2. `src/GameLoop.cpp`**
Inside `GameLoop::Update()`:
```cpp
if(TOURNAMENTMAN) {
    TOURNAMENTMAN->Update();
}
```
