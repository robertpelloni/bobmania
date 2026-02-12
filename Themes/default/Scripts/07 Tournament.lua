-- Tournament & Ladder API Wrappers

function GetTournamentStandings()
    -- Mock standings for now, derived from ladder if possible
    -- In a real implementation, this would query a separate API or process ladder data
    return {
        { Rank=1, Name="DDR_King", ELO=1500, Wins=10, Losses=2 },
        { Rank=2, Name="StaminaBot", ELO=1450, Wins=8, Losses=4 },
        { Rank=3, Name="PadSmasher", ELO=1400, Wins=7, Losses=5 },
        { Rank=4, Name="RhythmMaster", ELO=1350, Wins=5, Losses=7 },
        { Rank=5, Name="ArrowKeyUser", ELO=1300, Wins=3, Losses=9 },
    }
end

function GetUpcomingMatches()
    if not TOURNAMENTMAN then return {} end

    local ladder = TOURNAMENTMAN:GetLadder()
    local matches = {}

    -- Convert C++ ladder structs to Lua table format expected by UI
    for i, m in ipairs(ladder) do
        local status = m.Status or "Pending"
        if status == "Pending" then
            matches[#matches+1] = {
                ID = m.MatchID,
                P1 = "You",
                P2 = m.Opponent,
                Song = m.Song,
                Time = "Now",
                Prize = "100 BOBC",
                Status = status
            }
        end
    end

    return matches
end

function StartTournamentMatch(matchID)
    if not TOURNAMENTMAN then return end

    -- Tell backend to set up match
    TOURNAMENTMAN:StartMatch(matchID)

    -- Set global flag for evaluation screen
    _G.ActiveTournamentMatchID = matchID

    -- For now, just go to Select Music.
    -- Ideally we would force the specific song here:
    -- GAMESTATE:SetCurrentSong( SONGMAN:FindSong(songTitle) )
    SCREENMAN:SetNewScreen("ScreenSelectMusic")
end

function ReportTournamentResult(score)
    if not _G.ActiveTournamentMatchID then return end
    if not TOURNAMENTMAN then return end

    TOURNAMENTMAN:ReportMatchResult(_G.ActiveTournamentMatchID, score)

    -- Clear flag
    _G.ActiveTournamentMatchID = nil
end
