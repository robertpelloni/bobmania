#ifndef TOURNAMENT_MANAGER_H
#define TOURNAMENT_MANAGER_H

#include "global.h"
#include <vector>

struct LeaderboardEntry {
    int iRank;
    RString sUsername;
    int iElo;
};

class TournamentManager
{
public:
    TournamentManager();
    ~TournamentManager();

    void Init();
    void Shutdown();

    void Update();

    // Returns the Top 100 Elo players from the Unified Server
    std::vector<LeaderboardEntry> GetGlobalLeaderboard() const;

private:
    std::vector<LeaderboardEntry> m_vLeaderboardCache;
    void FetchLeaderboardFromServer();
};

extern TournamentManager* TOURNAMENTMAN;

#endif
