#ifndef TOURNAMENT_MANAGER_H
#define TOURNAMENT_MANAGER_H

<<<<<<< HEAD:itgmania/src/Tournament/TournamentManager.h
#include "RageTypes.h"
#include "TournamentBracket.h"
#include <vector>

struct lua_State;
class XNode;

struct LadderEntry {
    int Rank;
    RString Name;
    int ELO;
    int Wins;
    int Losses;
};

struct MatchInfo {
    RString P1;
    RString P2;
    RString Time;
    RString Prize;
=======
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
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Tournament/TournamentManager.h
};

class TournamentManager
{
public:
	TournamentManager();
	~TournamentManager();

	void Init();

<<<<<<< HEAD:itgmania/src/Tournament/TournamentManager.h
    // Ladder
    const std::vector<LadderEntry>& GetLadder() const;
    void UpdateELO( const RString& sPlayer, int iChange );

    // Matches
    const std::vector<MatchInfo>& GetUpcomingMatches() const;

    // Logic
    bool StartMatch( const RString& sOpponentName, int iSongID );
    void ReportMatchResult( const RString& sWinner );

	// Persistence
	void LoadFromNode( const XNode *pNode );
	XNode *CreateNode() const;
	void ReadFromDisk();
	void WriteToDisk();

	// Lua
	void PushSelf( lua_State *L );

private:
    std::vector<LadderEntry> m_Ladder;
    std::vector<MatchInfo> m_Matches;
    TournamentBracket m_CurrentBracket;
};

extern TournamentManager* TOURNAMENTMAN;
=======
    // API
	const std::vector<TournamentMatch>& GetLadder() const;
    void StartMatch(const RString& sMatchID);
    void ReportMatchResult(const RString& sMatchID, int iUserScore);

    // Callbacks
    void HandleMatchStarted(const Json::Value& payload);

    // Lua
    void PushSelf(lua_State *L);

private:
    std::vector<TournamentMatch> m_Ladder;
    RString m_CurrentMatchID;
};

extern TournamentManager *TOURNAMENTMAN;
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Tournament/TournamentManager.h

#endif
