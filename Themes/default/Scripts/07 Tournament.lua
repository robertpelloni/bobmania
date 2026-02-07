-- Tournament & Ladder API Wrappers

function GetTournamentStandings()
    return {
        { Rank=1, Name="SwiftFeet", ELO=2450, Wins=150, Losses=12 },
        { Rank=2, Name="RhythmMaster", ELO=2410, Wins=142, Losses=20 },
        { Rank=3, Name="ArrowKing", ELO=2380, Wins=130, Losses=15 },
        { Rank=4, Name="Dancer01", ELO=2200, Wins=100, Losses=50 },
        { Rank=5, Name="StepPro", ELO=2150, Wins=95, Losses=55 },
        { Rank=6, Name="Newbie", ELO=1500, Wins=10, Losses=5 }
    }
end

function GetUpcomingMatches()
    return {
        { ID="m1", P1="SwiftFeet", P2="RhythmMaster", Time="18:00 UTC", Prize="1000 BOB" },
        { ID="m2", P1="ArrowKing", P2="Dancer01", Time="19:00 UTC", Prize="500 BOB" }
    }
end
