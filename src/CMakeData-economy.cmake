list(APPEND SMDATA_ECONOMY_SRC
            "Economy/EconomyManager.cpp"
            "Economy/ScreenGameplayCommercial.cpp"
            "Economy/Governance/ScreenGovernance.cpp"
            "Tournament/ScreenTournamentLadder.cpp"
            "Tournament/ScreenBettingEntry.cpp"
            "Tournament/ScreenEvaluationBetting.cpp"
            "Tournament/ScreenLoginQR.cpp"
            "Gym/ScreenGymWelcome.cpp"
            "Scoring/MsdCalculator.cpp")
list(APPEND SMDATA_ECONOMY_HPP
            "Economy/EconomyManager.h"
            "Economy/ScreenGameplayCommercial.h"
            "Economy/Governance/ScreenGovernance.h"
            "Tournament/ScreenTournamentLadder.h"
            "Tournament/ScreenBettingEntry.h"
            "Tournament/ScreenEvaluationBetting.h"
            "Tournament/ScreenLoginQR.h"
            "Gym/ScreenGymWelcome.h"
            "Scoring/MsdCalculator.h")

source_group("Economy" FILES ${SMDATA_ECONOMY_SRC} ${SMDATA_ECONOMY_HPP})
