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
            "Gym/HeartRateManager.cpp"
=======
>>>>>>> main
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
<<<<<<< HEAD
<<<<<<< HEAD
            "NetworkManager.cpp"
=======
            "NetworkSyncManager.cpp"
            "Network/ChatManager.cpp"
            "Network/ContentSwarmManager.cpp"
            "Network/ContentSwarmManager.cpp"
            "Network/SpectatorManager.cpp"
            "Network/UnifiedNetwork.cpp"
>>>>>>> origin/unified-ui-features-13937230807013224518
=======
            "NetworkSyncManager.cpp"
>>>>>>> main
            "NoteSkinManager.cpp"
            "PrefsManager.cpp"
            "ProfileManager.cpp"
            "ScreenManager.cpp"
            "SongManager.cpp"
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
            "Gym/HeartRateManager.h"
=======
>>>>>>> main
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
            "NetworkManager.h"
=======
            "Unified/AssetSyncManager.h"
            "NetworkSyncManager.h"
            "Network/ChatManager.h"
            "Network/ContentSwarmManager.h"
            "Network/ContentSwarmManager.h"
            "Network/SpectatorManager.h"
            "Network/UnifiedNetwork.h"
>>>>>>> origin/unified-ui-features-13937230807013224518
=======
            "NetworkSyncManager.h"
>>>>>>> main
            "NoteSkinManager.h"
            "PrefsManager.h"
            "ProfileManager.h"
            "ScreenManager.h"
            "SongManager.h"
            "StatsManager.h"
            "ThemeManager.h"
            "UnlockManager.h")

<<<<<<< HEAD
=======
if(WITH_NETWORKING)
  list(APPEND SMDATA_GLOBAL_SINGLETON_SRC "ezsockets.cpp")
  list(APPEND SMDATA_GLOBAL_SINGLETON_HPP "ezsockets.h")
endif()

>>>>>>> main
source_group("Global Singletons"
             FILES
             ${SMDATA_GLOBAL_SINGLETON_SRC}
             ${SMDATA_GLOBAL_SINGLETON_HPP})
