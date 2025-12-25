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
            "NetworkSyncManager.cpp"
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
            "GrooveStats/GrooveStatsManager.h"
            "GameState.h"
            "InputFilter.h"
            "InputMapper.h"
            "InputQueue.h"
            "LightsManager.h"
            "LuaManager.h"
            "MemoryCardManager.h"
            "MessageManager.h"
            "NetworkSyncManager.h"
            "NoteSkinManager.h"
            "PrefsManager.h"
            "ProfileManager.h"
            "ScreenManager.h"
            "SongManager.h"
            "StatsManager.h"
            "ThemeManager.h"
            "UnlockManager.h")

if(WITH_NETWORKING)
  list(APPEND SMDATA_GLOBAL_SINGLETON_SRC "ezsockets.cpp")
  list(APPEND SMDATA_GLOBAL_SINGLETON_HPP "ezsockets.h")
endif()

source_group("Global Singletons"
             FILES
             ${SMDATA_GLOBAL_SINGLETON_SRC}
             ${SMDATA_GLOBAL_SINGLETON_HPP})
