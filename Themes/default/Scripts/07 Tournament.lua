-- Tournament & Ladder API Wrappers

function GetTournamentStandings()
    if not TOURNAMENTMAN then return {} end
    return TOURNAMENTMAN:GetLadder()
end

function GetUpcomingMatches()
    if not TOURNAMENTMAN then return {} end
    return TOURNAMENTMAN:GetMatches()
end
