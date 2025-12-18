#include "global.h"
#include "TournamentBracket.h"
#include "RageUtil.h"

void TournamentBracket::Generate(const std::vector<std::string>& players)
{
	m_Matches.clear();
	if (players.size() < 8) return; // Fixed size for MVP

	// Quarter Finals (Matches 0-3)
	for(int i=0; i<4; ++i) {
		BracketMatch m;
		m.id = i;
		m.p1 = players[i*2];
		m.p2 = players[i*2+1];
		m.nextMatchId = 4 + (i/2);
		m_Matches.push_back(m);
	}

	// Semi Finals (Matches 4-5)
	for(int i=0; i<2; ++i) {
		BracketMatch m;
		m.id = 4+i;
		m.p1 = "TBD";
		m.p2 = "TBD";
		m.nextMatchId = 6;
		m_Matches.push_back(m);
	}

	// Final (Match 6)
	BracketMatch m;
	m.id = 6;
	m.p1 = "TBD";
	m.p2 = "TBD";
	m.nextMatchId = -1; // Root
	m_Matches.push_back(m);
}

void TournamentBracket::SetWinner(int matchId, const std::string& winner)
{
	if(matchId >= (int)m_Matches.size()) return;

	m_Matches[matchId].winner = winner;

	int next = m_Matches[matchId].nextMatchId;
	if(next != -1 && next < (int)m_Matches.size()) {
		// Propagate to next match
		// Determine if we are p1 or p2 slot in next match
		// Simple logic: If current ID is even, we are p1 in next. Odd -> p2.
		// QF 0,1 -> SF 4. 0 is even(p1), 1 is odd(p2).
		// QF 2,3 -> SF 5. 2 is even(p1), 3 is odd(p2).

		if(matchId % 2 == 0) m_Matches[next].p1 = winner;
		else                 m_Matches[next].p2 = winner;
	}
}
