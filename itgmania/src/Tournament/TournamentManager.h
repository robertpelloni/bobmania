#ifndef TOURNAMENT_MANAGER_H
#define TOURNAMENT_MANAGER_H

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
};

class TournamentManager
{
public:
	TournamentManager();
	~TournamentManager();

	void Init();

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

#endif
