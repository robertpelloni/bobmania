#include "global.h"
#include "TournamentManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
<<<<<<< HEAD:itgmania/src/Tournament/TournamentManager.cpp
#include "XmlFile.h"
#include "XmlFileUtil.h"
#include "RageFile.h"
#include "RageUtil.h"

TournamentManager* TOURNAMENTMAN = nullptr;

static const RString TOURNAMENT_DAT = "Save/Tournament.xml";
=======
#include "GameState.h"
#include "ScreenManager.h"
#include "Economy/EconomyManager.h"
#include "Network/UnifiedNetwork.h"
#include "json/json.h"

TournamentManager *TOURNAMENTMAN = nullptr;
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Tournament/TournamentManager.cpp

TournamentManager::TournamentManager()
{
}

TournamentManager::~TournamentManager()
{
<<<<<<< HEAD:itgmania/src/Tournament/TournamentManager.cpp
	WriteToDisk();
=======
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Tournament/TournamentManager.cpp
}

void TournamentManager::Init()
{
<<<<<<< HEAD:itgmania/src/Tournament/TournamentManager.cpp
	ReadFromDisk();

    // If no ladder loaded, seed default
    if( m_Ladder.empty() )
    {
        m_Ladder.push_back({ 1, "SwiftFeet", 2450, 150, 12 });
        m_Ladder.push_back({ 2, "RhythmMaster", 2410, 142, 20 });
        m_Ladder.push_back({ 3, "ArrowKing", 2380, 130, 15 });
        m_Ladder.push_back({ 4, "Dancer01", 2200, 100, 50 });
        m_Ladder.push_back({ 5, "StepPro", 2150, 95, 55 });
        m_Ladder.push_back({ 6, "Newbie", 1500, 10, 5 });
    }

    // If no matches, seed default
    if( m_Matches.empty() )
    {
        m_Matches.push_back({ "SwiftFeet", "RhythmMaster", "18:00 UTC", "1000 BOB" });
        m_Matches.push_back({ "ArrowKing", "Dancer01", "19:00 UTC", "500 BOB" });
    }
}

void TournamentManager::LoadFromNode( const XNode *pNode )
{
	if( pNode->GetName() != "Tournament" ) return;

    const XNode *pLadder = pNode->GetChild( "Ladder" );
    if( pLadder )
    {
        m_Ladder.clear();
        FOREACH_CONST_Child( pLadder, entry )
        {
            LadderEntry e;
            entry->GetAttrValue("Rank", e.Rank);
            entry->GetAttrValue("Name", e.Name);
            entry->GetAttrValue("ELO", e.ELO);
            entry->GetAttrValue("Wins", e.Wins);
            entry->GetAttrValue("Losses", e.Losses);
            m_Ladder.push_back(e);
        }
    }
}

XNode* TournamentManager::CreateNode() const
{
	XNode *xml = new XNode( "Tournament" );
    XNode *pLadder = xml->AppendChild( "Ladder" );
    for( const auto& e : m_Ladder )
    {
        XNode *entry = pLadder->AppendChild( "Entry" );
        entry->AppendAttr("Rank", e.Rank);
        entry->AppendAttr("Name", e.Name);
        entry->AppendAttr("ELO", e.ELO);
        entry->AppendAttr("Wins", e.Wins);
        entry->AppendAttr("Losses", e.Losses);
    }
	return xml;
}

void TournamentManager::ReadFromDisk()
{
	if( !IsAFile(TOURNAMENT_DAT) ) return;
	XNode xml;
	if( !XmlFileUtil::LoadFromFileShowErrors(xml, TOURNAMENT_DAT) ) return;
	LoadFromNode( &xml );
}

void TournamentManager::WriteToDisk()
{
	RageFile f;
	if( !f.Open(TOURNAMENT_DAT, RageFile::WRITE|RageFile::SLOW_FLUSH) ) return;
	std::unique_ptr<XNode> xml( CreateNode() );
	XmlFileUtil::SaveToFile( xml.get(), f );
}

const std::vector<LadderEntry>& TournamentManager::GetLadder() const
=======
	LOG->Trace( "TournamentManager::Init()" );

    if( UNIFIED_NET )
    {
        UNIFIED_NET->RegisterCallback("match_started", std::bind(&TournamentManager::HandleMatchStarted, this, std::placeholders::_1));
    }

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
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Tournament/TournamentManager.cpp
{
    return m_Ladder;
}

<<<<<<< HEAD:itgmania/src/Tournament/TournamentManager.cpp
const std::vector<MatchInfo>& TournamentManager::GetUpcomingMatches() const
{
    return m_Matches;
}

void TournamentManager::UpdateELO( const RString& sPlayer, int iChange )
{
    // Simplified ELO update
    for( auto& e : m_Ladder )
    {
        if( e.Name == sPlayer )
        {
            e.ELO += iChange;
            if( iChange > 0 ) e.Wins++; else e.Losses++;
            return; // Found
        }
    }
}

bool TournamentManager::StartMatch( const RString& sOpponentName, int iSongID )
{
    // 1. Validate Opponent
    // 2. Load Song (Simulated via ID)
    // 3. Set GameState to Versus Mode
    // 4. Set Opponent Name for UI

    // For MVP, we'll just log and return true.
    LOG->Trace("Starting match against %s on Song %d", sOpponentName.c_str(), iSongID);
    return true;
}

void TournamentManager::ReportMatchResult( const RString& sWinner )
{
    LOG->Trace("Match Finished. Winner: %s", sWinner.c_str());

    if( sWinner == "Player" )
    {
        UpdateELO("Newbie", 25); // Assume player is "Newbie"
        UpdateELO("RhythmMaster", -25); // Mock opponent
    }
    else
    {
        UpdateELO("Newbie", -20);
        UpdateELO("RhythmMaster", 20);
    }
    WriteToDisk();
}

// Lua
class LunaTournamentManager: public Luna<TournamentManager>
{
public:
    static int GetLadder( T* p, lua_State *L )
    {
        const auto& ladder = p->GetLadder();
        lua_newtable(L);
        for( size_t i=0; i<ladder.size(); ++i )
        {
            lua_newtable(L);
            lua_pushstring(L, "Rank"); lua_pushnumber(L, ladder[i].Rank); lua_settable(L, -3);
            lua_pushstring(L, "Name"); lua_pushstring(L, ladder[i].Name); lua_settable(L, -3);
            lua_pushstring(L, "ELO"); lua_pushnumber(L, ladder[i].ELO); lua_settable(L, -3);
            lua_pushstring(L, "Wins"); lua_pushnumber(L, ladder[i].Wins); lua_settable(L, -3);
            lua_pushstring(L, "Losses"); lua_pushnumber(L, ladder[i].Losses); lua_settable(L, -3);
            lua_rawseti(L, -2, i+1);
        }
        return 1;
    }

    static int GetMatches( T* p, lua_State *L )
    {
        const auto& matches = p->GetUpcomingMatches();
        lua_newtable(L);
        for( size_t i=0; i<matches.size(); ++i )
        {
            lua_newtable(L);
            lua_pushstring(L, "P1"); lua_pushstring(L, matches[i].P1); lua_settable(L, -3);
            lua_pushstring(L, "P2"); lua_pushstring(L, matches[i].P2); lua_settable(L, -3);
            lua_pushstring(L, "Time"); lua_pushstring(L, matches[i].Time); lua_settable(L, -3);
            lua_pushstring(L, "Prize"); lua_pushstring(L, matches[i].Prize); lua_settable(L, -3);
            lua_rawseti(L, -2, i+1);
        }
        return 1;
    }

    static int StartMatch( T* p, lua_State *L )
    {
        RString opp = SArg(1);
        int song = IArg(2);
        lua_pushboolean(L, p->StartMatch(opp, song));
        return 1;
=======
void TournamentManager::StartMatch(const RString& sMatchID)
{
    m_CurrentMatchID = sMatchID;
    LOG->Trace("TournamentManager: Starting Match %s", sMatchID.c_str());

    // Network Request
    if( UNIFIED_NET && UNIFIED_NET->IsConnected() )
    {
        Json::Value payload;
        payload["match_id"] = sMatchID;
        UNIFIED_NET->SendJSON("match_start_request", payload);
    }

    // Find match details (Mock)
    // In a real implementation, we would set GAMESTATE->m_pCurSong here based on SongTitle
    // For now, assume the frontend handles the screen transition
}

void TournamentManager::HandleMatchStarted(const Json::Value& payload)
{
    RString matchID = payload["match_id"].asString();
    RString status = payload["status"].asString();
    RString opponent = payload["opponent"].asString();

    LOG->Trace("TournamentManager: Match Started Packet! ID: %s, Opponent: %s", matchID.c_str(), opponent.c_str());

    // Here we would sync the GAMESTATE if we had full control
    SCREENMAN->SystemMessage("Opponent Connected: " + opponent);
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
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Tournament/TournamentManager.cpp
    }

    static int ReportMatchResult( T* p, lua_State *L )
    {
<<<<<<< HEAD:itgmania/src/Tournament/TournamentManager.cpp
        RString winner = SArg(1);
        p->ReportMatchResult(winner);
        return 0;
    }

    LunaTournamentManager()
    {
        ADD_METHOD( GetLadder );
        ADD_METHOD( GetMatches );
        ADD_METHOD( StartMatch );
        ADD_METHOD( ReportMatchResult );
    }
=======
        p->ReportMatchResult(SArg(1), IArg(2));
        return 0;
    }

	LunaTournamentManager()
	{
		ADD_METHOD( GetLadder );
        ADD_METHOD( StartMatch );
        ADD_METHOD( ReportMatchResult );
	}
>>>>>>> origin/unified-ui-features-13937230807013224518:src/Tournament/TournamentManager.cpp
};

LUA_REGISTER_CLASS( TournamentManager )
