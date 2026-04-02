# Ideas for Gym Module Improvements

## Refactoring
- [ ] **Profile Separation:** `GymManager` currently mixes Profile logic (Weight/BMI) with Session logic (Workouts). Split into `GymProfileManager` and `WorkoutManager`.
- [ ] **Biometrics Abstraction:** `HeartRateManager` is specific to HR. Generalize to `BiometricsManager` to support Cadence, Power (Cycling), etc.

## Features
- [ ] **VR Support:** Integration with SteamVR for fitness tracking in VR mode.
- [ ] **Calories 2.0:** Use METs (Metabolic Equivalent of Task) table based on chart difficulty and step density for accurate burn estimation.
- [ ] **Training Plans:** Multi-day workout schedules (e.g., "Couch to 5K" but for DDR).

## Technical Debt
- [ ] **Playlist Generation:** currently brute-forces random songs. Needs a smarter algorithm (Graph search?) to build smooth difficulty curves.
