#include "TournamentManager.h"
#include "RageLog.h"

// Luna Bindings
#include "LuaManager.h"
#include "LuaBinding.h"

TournamentManager* TOURNAMENTMAN = nullptr;

TournamentManager::TournamentManager() {}
TournamentManager::~TournamentManager() {}

void TournamentManager::Init() {
    LOG->Info("TournamentManager::Init() - Connecting to Unified Matchmaking Server...");
    FetchLeaderboardFromServer();
}

void TournamentManager::Shutdown() {
    LOG->Info("TournamentManager::Shutdown()");
}

void TournamentManager::Update() {
    // Network polling logic would live here
}

void TournamentManager::FetchLeaderboardFromServer() {
    // Mocking a JSON HTTP GET request to the Node.js / PostgreSQL server
    LOG->Info("TournamentManager: Fetching global Elo leaderboard...");

    m_vLeaderboardCache.clear();

    // Simulate top 5 players returned from DB
    m_vLeaderboardCache.push_back({1, "Jules_AI", 3200});
    m_vLeaderboardCache.push_back({2, "NotITG_Master", 2950});
    m_vLeaderboardCache.push_back({3, "BobcoinMiner", 2400});
    m_vLeaderboardCache.push_back({4, "StaminaGod", 2100});
    m_vLeaderboardCache.push_back({5, "Guest_8f2a", 1250});
}

std::vector<LeaderboardEntry> TournamentManager::GetGlobalLeaderboard() const {
    return m_vLeaderboardCache;
}

// ----------------------------------------------------------------------------
// Lua Bindings
// ----------------------------------------------------------------------------

class LunaTournamentManager : public Luna<TournamentManager>
{
public:
    static int GetGlobalLeaderboard(T* p, lua_State* L) {
        // Return CSV formatted string for Lua to parse
        RString summary = "";
        std::vector<LeaderboardEntry> board = p->GetGlobalLeaderboard();
        for (size_t i = 0; i < board.size(); ++i) {
            summary += std::to_string(board[i].iRank) + "|" + board[i].sUsername + "|" + std::to_string(board[i].iElo) + ",";
        }
        // lua_pushstring(L, summary.c_str());
        return 1;
    }

    LunaTournamentManager()
    {
        ADD_METHOD(GetGlobalLeaderboard);
    }
};

LUA_REGISTER_CLASS(TournamentManager)
