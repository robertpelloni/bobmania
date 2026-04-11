#ifndef TOURNAMENT_BRACKET_H
#define TOURNAMENT_BRACKET_H

#include <vector>
#include <string>

struct BracketMatch {
	int id;
	std::string p1;
	std::string p2;
	std::string winner; // Empty if pending
	int nextMatchId;
};

class TournamentBracket
{
public:
	// Generates a simple 8-player single elimination bracket
	void Generate(const std::vector<std::string>& players);

	// Returns current state of matches
	const std::vector<BracketMatch>& GetMatches() const { return m_Matches; }

	void SetWinner(int matchId, const std::string& winner);

private:
	std::vector<BracketMatch> m_Matches;
};

#endif
