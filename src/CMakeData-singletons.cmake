list(APPEND SMDATA_GLOBAL_SINGLETON_SRC
            "AnnouncerManager.cpp"
            "Bookkeeper.cpp"
            "Economy/EconomyManager.cpp"
            "CharacterManager.cpp"
            "CommandLineActions.cpp"
            "CryptManager.cpp"
            "FontManager.cpp"
            "GameManager.cpp"
            "GameSoundManager.cpp"
<<<<<<< HEAD
<<<<<<< HEAD
            "Gym/HeartRateManager.cpp"
=======
>>>>>>> main
=======
            "Gym/HeartRateManager.cpp"
>>>>>>> origin/unified-ui-features-13937230807013224518
            "GrooveStats/GrooveStatsManager.cpp"
            "Discord/DiscordManager.cpp"
            "GameState.cpp"
            "InputFilter.cpp"
            "InputMapper.cpp"
            "InputQueue.cpp"
            "LightsManager.cpp"
            "LuaManager.cpp"
            "LuaModules/LuaModule_File.cpp"
            "MemoryCardManager.cpp"
            "MessageManager.cpp"
            "NetworkManager.cpp"
            "NetworkSyncManager.cpp"
            "Network/ContentSwarmManager.cpp"
            "Network/LuaBinding_ContentSwarmManager.cpp"
            "Network/ChatManager.cpp"
            "Network/SpectatorManager.cpp"
            "Network/UnifiedNetwork.cpp"
<<<<<<< HEAD
>>>>>>> origin/unified-ui-features-13937230807013224518
=======
            "NetworkSyncManager.cpp"
>>>>>>> main
=======
>>>>>>> origin/unified-ui-features-13937230807013224518
            "NoteSkinManager.cpp"
            "PrefsManager.cpp"
            "ProfileManager.cpp"
            "ScreenManager.cpp"
            "SongManager.cpp"
            "Scoring/ReplayManager.cpp"
            "Unified/MissionManager.cpp"
            "Unified/PluginManager.cpp"
            "Tournament/TournamentManager.cpp"
            "StatsManager.cpp"
            "ThemeManager.cpp"
            "UnlockManager.cpp")

list(APPEND SMDATA_GLOBAL_SINGLETON_HPP
            "AnnouncerManager.h"
            "Bookkeeper.h"
            "Economy/EconomyManager.h"
            "CharacterManager.h"
            "CommandLineActions.h"
            "CryptManager.h"
            "FontManager.h"
            "GameManager.h"
            "GameSoundManager.h"
<<<<<<< HEAD
<<<<<<< HEAD
            "Gym/HeartRateManager.h"
=======
>>>>>>> main
=======
            "Gym/HeartRateManager.h"
>>>>>>> origin/unified-ui-features-13937230807013224518
            "GrooveStats/GrooveStatsManager.h"
            "GameState.h"
            "InputFilter.h"
            "InputMapper.h"
            "InputQueue.h"
            "LightsManager.h"
            "LuaManager.h"
            "MemoryCardManager.h"
            "MessageManager.h"
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
            "NetworkManager.h"
=======
=======
>>>>>>> origin/unified-ui-features-13937230807013224518
            "Unified/AssetSyncManager.h"
            "NetworkSyncManager.h"
            "Network/ContentSwarmManager.h"
            "Network/LuaBinding_ContentSwarmManager.h"
            "Unified/AssetSyncManager.h"
            "Network/ChatManager.h"
            "Network/SpectatorManager.h"
            "Network/UnifiedNetwork.h"
<<<<<<< HEAD
>>>>>>> origin/unified-ui-features-13937230807013224518
=======
            "NetworkSyncManager.h"
>>>>>>> main
=======
>>>>>>> origin/unified-ui-features-13937230807013224518
            "NoteSkinManager.h"
            "PrefsManager.h"
            "ProfileManager.h"
            "ScreenManager.h"
            "SongManager.h"
            "Scoring/ReplayManager.h"
            "Unified/MissionManager.h"
            "Unified/PluginManager.h"
            "Tournament/TournamentManager.h"
            "StatsManager.h"
            "ThemeManager.h"
            "UnlockManager.h")

if(WITH_NETWORKING)


endif()

source_group("Global Singletons"
             FILES
             ${SMDATA_GLOBAL_SINGLETON_SRC}
             ${SMDATA_GLOBAL_SINGLETON_HPP})
