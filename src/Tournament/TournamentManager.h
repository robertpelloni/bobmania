#ifndef TOURNAMENT_MANAGER_H
#define TOURNAMENT_MANAGER_H

#include "RageUtil.h"
#include <vector>

struct lua_State;

struct TournamentMatch
{
	RString MatchID;
	RString OpponentName;
	int OpponentELO;
	RString SongTitle;
	RString Difficulty;
	bool bCompleted;
	int UserScore;
	int OpponentScore;
    RString Result; // "Win", "Loss", "Draw"
};

class TournamentManager
{
public:
	TournamentManager();
	~TournamentManager();

	void Init();

    // API
	const std::vector<TournamentMatch>& GetLadder() const;
    void StartMatch(const RString& sMatchID);
    void ReportMatchResult(const RString& sMatchID, int iUserScore);

    // Lua
    void PushSelf(lua_State *L);

private:
    std::vector<TournamentMatch> m_Ladder;
    RString m_CurrentMatchID;
};

extern TournamentManager *TOURNAMENTMAN;

#endif
