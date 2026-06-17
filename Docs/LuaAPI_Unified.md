# Unified StepMania Lua API Documentation

This document outlines the Lua bindings for the new Singleton Managers introduced in the Unified StepMania foundation.

## ECONOMYMAN (EconomyManager)
Manages Bobcoin balance, transactions, and marketplace items.

- `GetBalance()`: Returns the current Bobcoin balance as a number.
- `BuyItem(itemID, cost)`: Attempts to purchase an item. Returns boolean success.
- `HasItem(itemID)`: Returns true if the player already owns the item.
- `GetMarketplaceItems()`: Returns a table of available items. Each item is a table with `{ID, Name, Price}`.

## GYMMAN (GymManager)
Manages fitness profiles, calorie tracking, and workout generation.

- `GetProfile()`: Returns a table with `{Weight, Height, BMI, DailyGoal, TotalCaloriesBurned, StreakDays, TodayCalories}`.
- `UpdateWeight(newWeight)`: Updates the player's weight and recalculates BMI.
- `LogWorkout(playlistName, duration, calories)`: Records a completed workout session.
- `GeneratePlaylist(targetDuration, minMeter, maxMeter)`: Generates and starts a Smart Playlist as a course. Returns boolean success.

## HRMAN (HeartRateManager)
Interfaces with Heart Rate Monitor hardware.

- `GetBPM()`: Returns the current Beats Per Minute.
- `IsConnected()`: Returns true if a device is actively transmitting.

## TOURNAMENTMAN (TournamentManager)
Handles ranked ladders and match result reporting.

- `GetLadder()`: Returns a table of matches. Each match is `{MatchID, Opponent, ELO, Song, Status}`.
- `StartMatch(matchID)`: Signals the network to begin a specific match.
- `ReportMatchResult(matchID, userScore)`: Submits the final score for a match.

## SWARMMAN (ContentSwarmManager)
Manages P2P content discovery and downloads.

- `StartDiscovery()`: Begins searching for available packs on the network.
- `GetAvailablePacks()`: Returns a table of packs. Each pack is `{ID, Name, Author, Seeders}`.
- `RequestPack(packID)`: Initiates a download for the specified pack.

## ASSETSYNCMAN (AssetSyncManager)
Handles cloud synchronization of local data.

- `StartSync()`: Performs a full pull/push cycle.
- `PushToCloud()`: Uploads local state to the server.
- `PullFromCloud()`: Downloads remote state to the local machine.
- `GetStatus()`: Returns a string describing the current sync operation (e.g., "Complete", "Syncing...").
- `GetProgress()`: Returns a number from 0.0 to 1.0 representing sync progress.

## CHATMAN (ChatManager)
Provides in-game global chat functionality.

- `SendChat(text)`: Sends a message to the global channel.
- `GetMessages()`: Returns a table of recent messages. Each message is `{User, Text, Time}`.

## SPECTATORMAN (SpectatorManager)
Allows watching live matches.

- `ConnectToMatch(matchID)`: Joins a specific match as a spectator.
- `StartBroadcasting(streamKey)`: Starts streaming local gameplay to the network.
- `IsConnected()`: Returns true if currently spectating.
- `IsBroadcasting()`: Returns true if currently streaming.
