#include "global.h"
#include "MissionManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "XmlFile.h"
#include "XmlFileUtil.h"
#include "RageFile.h"
#include "RageUtil.h"
#include "Economy/EconomyManager.h" // To grant rewards eventually

MissionManager* MISSIONMAN = nullptr;
static const RString MISSION_DAT = "Save/Missions.xml";

MissionManager::MissionManager()
{
    // Initialize Default Missions
    m_Missions.push_back({ "m_intro_1", "First Steps", "Clear 3 Songs", "SongsPassed", 3.0f, 0.0f, false, false, "50 BOB" });
    m_Missions.push_back({ "m_intro_2", "Warm Up", "Burn 100 Calories", "Calories", 100.0f, 0.0f, false, false, "100 BOB" });
    m_Missions.push_back({ "m_combo_1", "Combo Master", "Hit 500 Notes", "NotesHit", 500.0f, 0.0f, false, false, "200 BOB" });
}

MissionManager::~MissionManager()
{
	WriteToDisk();
}

void MissionManager::Init()
{
	ReadFromDisk();
}

int MissionManager::GetMissionIndex( const RString& sID )
{
    for( size_t i=0; i<m_Missions.size(); ++i )
        if( m_Missions[i].ID == sID ) return i;
    return -1;
}

const std::vector<Mission>& MissionManager::GetMissions() const
{
    return m_Missions;
}

void MissionManager::ReportMetric( const RString& sType, float fValue )
{
    bool bUpdated = false;
    for( auto& m : m_Missions )
    {
        if( !m.Completed && m.GoalType == sType )
        {
            m.CurrentProgress += fValue;
            if( m.CurrentProgress >= m.TargetValue )
            {
                m.CurrentProgress = m.TargetValue;
                m.Completed = true;
                // Optional: Broadcast "MissionComplete" message
            }
            bUpdated = true;
        }
    }
    if( bUpdated ) WriteToDisk();
}

bool MissionManager::ClaimReward( const RString& sMissionID )
{
    int idx = GetMissionIndex(sMissionID);
    if( idx == -1 ) return false;

    Mission& m = m_Missions[idx];
    if( m.Completed && !m.Claimed )
    {
        m.Claimed = true;
        // Parse reward string (simple hack)
        int amount = 0;
        if( sscanf( m.Reward.c_str(), "%d BOB", &amount ) == 1 )
        {
            if( ECONOMYMAN ) ECONOMYMAN->Deposit( (long long)amount, "Mission Reward: " + m.Title );
        }
        WriteToDisk();
        return true;
    }
    return false;
}

void MissionManager::LoadFromNode( const XNode *pNode )
{
	if( pNode->GetName() != "Missions" ) return;
    FOREACH_CONST_Child( pNode, mNode )
    {
        RString id;
        if( mNode->GetAttrValue("ID", id) )
        {
            int idx = GetMissionIndex(id);
            if( idx != -1 )
            {
                mNode->GetAttrValue("Progress", m_Missions[idx].CurrentProgress);
                mNode->GetAttrValue("Completed", m_Missions[idx].Completed);
                mNode->GetAttrValue("Claimed", m_Missions[idx].Claimed);
            }
        }
    }
}

XNode* MissionManager::CreateNode() const
{
	XNode *xml = new XNode( "Missions" );
    for( const auto& m : m_Missions )
    {
        XNode *mNode = xml->AppendChild("Mission");
        mNode->AppendAttr("ID", m.ID);
        mNode->AppendAttr("Progress", m.CurrentProgress);
        mNode->AppendAttr("Completed", m.Completed);
        mNode->AppendAttr("Claimed", m.Claimed);
    }
	return xml;
}

void MissionManager::ReadFromDisk()
{
	if( !IsAFile(MISSION_DAT) ) return;
	XNode xml;
	if( !XmlFileUtil::LoadFromFileShowErrors(xml, MISSION_DAT) ) return;
	LoadFromNode( &xml );
}

void MissionManager::WriteToDisk()
{
	RageFile f;
	if( !f.Open(MISSION_DAT, RageFile::WRITE|RageFile::SLOW_FLUSH) ) return;
	std::unique_ptr<XNode> xml( CreateNode() );
	XmlFileUtil::SaveToFile( xml.get(), f );
}

// Lua
class LunaMissionManager: public Luna<MissionManager>
{
public:
    static int GetMissions( T* p, lua_State *L )
    {
        const auto& missions = p->GetMissions();
        lua_newtable(L);
        for( size_t i=0; i<missions.size(); ++i )
        {
            lua_newtable(L);
            lua_pushstring(L, "ID"); lua_pushstring(L, missions[i].ID); lua_settable(L, -3);
            lua_pushstring(L, "Title"); lua_pushstring(L, missions[i].Title); lua_settable(L, -3);
            lua_pushstring(L, "Desc"); lua_pushstring(L, missions[i].Description); lua_settable(L, -3);
            lua_pushstring(L, "Reward"); lua_pushstring(L, missions[i].Reward); lua_settable(L, -3);
            lua_pushstring(L, "Status");
            if( missions[i].Claimed ) lua_pushstring(L, "Claimed");
            else if( missions[i].Completed ) lua_pushstring(L, "Complete");
            else lua_pushstring(L, "Active");
            lua_settable(L, -3);

            lua_pushstring(L, "Progress"); lua_pushnumber(L, missions[i].CurrentProgress); lua_settable(L, -3);
            lua_pushstring(L, "Target"); lua_pushnumber(L, missions[i].TargetValue); lua_settable(L, -3);

            lua_rawseti(L, -2, i+1);
        }
        return 1;
    }

    static int ClaimReward( T* p, lua_State *L )
    {
        RString id = SArg(1);
        lua_pushboolean(L, p->ClaimReward(id));
        return 1;
    }

    LunaMissionManager()
    {
        ADD_METHOD( GetMissions );
        ADD_METHOD( ClaimReward );
    }
};

LUA_REGISTER_CLASS( MissionManager )
