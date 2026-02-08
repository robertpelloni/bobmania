#include "global.h"
#include "TournamentManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "XmlFile.h"
#include "XmlFileUtil.h"
#include "RageFile.h"
#include "RageUtil.h"

TournamentManager* TOURNAMENTMAN = nullptr;

static const RString TOURNAMENT_DAT = "Save/Tournament.xml";

TournamentManager::TournamentManager()
{
    // Mock Ladder
    m_Ladder.push_back({ 1, "SwiftFeet", 2450, 150, 12 });
    m_Ladder.push_back({ 2, "RhythmMaster", 2410, 142, 20 });
    m_Ladder.push_back({ 3, "ArrowKing", 2380, 130, 15 });
    m_Ladder.push_back({ 4, "Dancer01", 2200, 100, 50 });
    m_Ladder.push_back({ 5, "StepPro", 2150, 95, 55 });
    m_Ladder.push_back({ 6, "Newbie", 1500, 10, 5 });

    // Mock Matches
    m_Matches.push_back({ "SwiftFeet", "RhythmMaster", "18:00 UTC", "1000 BOB" });
    m_Matches.push_back({ "ArrowKing", "Dancer01", "19:00 UTC", "500 BOB" });
}

TournamentManager::~TournamentManager()
{
	WriteToDisk();
}

void TournamentManager::Init()
{
	ReadFromDisk();
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
{
    return m_Ladder;
}

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

    LunaTournamentManager()
    {
        ADD_METHOD( GetLadder );
        ADD_METHOD( GetMatches );
    }
};

LUA_REGISTER_CLASS( TournamentManager )
