#include "global.h"
#include "TournamentManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "GameState.h"
#include "ScreenManager.h"
#include "Economy/EconomyManager.h"

TournamentManager *TOURNAMENTMAN = nullptr;

TournamentManager::TournamentManager()
{
}

TournamentManager::~TournamentManager()
{
}

void TournamentManager::Init()
{
	LOG->Trace( "TournamentManager::Init()" );

    // Mock Ladder Data
    TournamentMatch m1;
    m1.MatchID="m001";
    m1.OpponentName="DDR_King";
    m1.OpponentELO=1500;
    m1.SongTitle="Legend of MAX";
    m1.Difficulty="Hard";
    m1.bCompleted=false;
    m_Ladder.push_back(m1);

    TournamentMatch m2;
    m2.MatchID="m002";
    m2.OpponentName="StaminaBot";
    m2.OpponentELO=1450;
    m2.SongTitle="Vertex Beta";
    m2.Difficulty="Expert";
    m2.bCompleted=false;
    m_Ladder.push_back(m2);
}

const std::vector<TournamentMatch>& TournamentManager::GetLadder() const
{
    return m_Ladder;
}

void TournamentManager::StartMatch(const RString& sMatchID)
{
    m_CurrentMatchID = sMatchID;
    LOG->Trace("TournamentManager: Starting Match %s", sMatchID.c_str());

    // Find match details (Mock)
    // In a real implementation, we would set GAMESTATE->m_pCurSong here based on SongTitle
    // For now, assume the frontend handles the screen transition
}

void TournamentManager::ReportMatchResult(const RString& sMatchID, int iUserScore)
{
    // If no ID provided, try last active
    RString id = sMatchID.empty() ? m_CurrentMatchID : sMatchID;

    for( std::vector<TournamentMatch>::iterator it = m_Ladder.begin(); it != m_Ladder.end(); ++it )
    {
        TournamentMatch& match = *it;
        if( match.MatchID == id )
        {
            if( match.bCompleted ) return; // Already reported

            match.bCompleted = true;
            match.UserScore = iUserScore;
            // Mock opponent score logic: Randomly slightly higher or lower
            match.OpponentScore = iUserScore + (rand() % 10000 - 5000);
            if( match.OpponentScore < 0 ) match.OpponentScore = 0;

            if( match.UserScore > match.OpponentScore )
            {
                match.Result = "Win";
                if( ECONOMYMAN ) ECONOMYMAN->Deposit(100, "Tournament Win: " + match.OpponentName);
            }
            else
            {
                match.Result = "Loss";
            }

            LOG->Trace("TournamentManager: Match %s finished. User: %d, Opp: %d. Result: %s",
                id.c_str(), match.UserScore, match.OpponentScore, match.Result.c_str());
            break;
        }
    }
    m_CurrentMatchID = "";
}

// Lua Bindings
class LunaTournamentManager: public Luna<TournamentManager>
{
public:
	static int GetLadder( T* p, lua_State *L )
	{
		const std::vector<TournamentMatch>& ladder = p->GetLadder();
		lua_newtable(L);
		for( size_t i=0; i<ladder.size(); ++i )
		{
			lua_newtable(L);
			lua_pushstring(L, "MatchID"); lua_pushstring(L, ladder[i].MatchID); lua_settable(L, -3);
			lua_pushstring(L, "Opponent"); lua_pushstring(L, ladder[i].OpponentName); lua_settable(L, -3);
			lua_pushnumber(L, ladder[i].OpponentELO); lua_setfield(L, -2, "ELO");
			lua_pushstring(L, "Song"); lua_pushstring(L, ladder[i].SongTitle); lua_settable(L, -3);
            lua_pushstring(L, "Status"); lua_pushstring(L, ladder[i].bCompleted ? ladder[i].Result.c_str() : "Pending"); lua_settable(L, -3);
			lua_rawseti(L, -2, i+1);
		}
		return 1;
	}

    static int StartMatch( T* p, lua_State *L )
    {
        p->StartMatch(SArg(1));
        return 0;
    }

    static int ReportMatchResult( T* p, lua_State *L )
    {
        p->ReportMatchResult(SArg(1), IArg(2));
        return 0;
    }

	LunaTournamentManager()
	{
		ADD_METHOD( GetLadder );
        ADD_METHOD( StartMatch );
        ADD_METHOD( ReportMatchResult );
	}
};

LUA_REGISTER_CLASS( TournamentManager )
