list(APPEND SMDATA_ECONOMY_SRC
            "Economy/EconomyManager.cpp"
            "Tournament/ScreenTournamentLadder.cpp"
            "Tournament/ScreenBettingEntry.cpp"
            "Gym/ScreenGymWelcome.cpp")
list(APPEND SMDATA_ECONOMY_HPP
            "Economy/EconomyManager.h"
            "Tournament/ScreenTournamentLadder.h"
            "Tournament/ScreenBettingEntry.h"
            "Gym/ScreenGymWelcome.h")

source_group("Economy" FILES ${SMDATA_ECONOMY_SRC} ${SMDATA_ECONOMY_HPP})
