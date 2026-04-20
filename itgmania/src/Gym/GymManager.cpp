#include "global.h"
#include "GymManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "XmlFile.h"
#include "XmlFileUtil.h"
#include "RageFile.h"
#include "RageUtil.h"
#include "DateTime.h"
#include "GymPlaylistGenerator.h"
#include "Song.h"

GymManager*	GYMMAN = nullptr;

static const RString GYM_DAT = "Save/Gym.xml";

GymManager::GymManager()
{
    // Defaults
    m_Profile.Weight = 70.0f;
    m_Profile.Height = 175.0f;
    m_Profile.BMI = 22.8f;
    m_Profile.DailyGoal = 500.0f;
    m_Profile.TotalCaloriesBurned = 0.0f;
    m_Profile.StreakDays = 0;
    m_Profile.CaloriesToday = 0.0f;
    m_Profile.LastLoginDate = "";
}

GymManager::~GymManager()
{
	WriteToDisk();
}

void GymManager::Init()
{
	ReadFromDisk();
    UpdateStreak();
}

void GymManager::LoadFromNode( const XNode *pNode )
{
	if( pNode->GetName() != "Gym" ) return;

    pNode->GetChildValue( "Weight", m_Profile.Weight );
    pNode->GetChildValue( "Height", m_Profile.Height );
    pNode->GetChildValue( "DailyGoal", m_Profile.DailyGoal );
    pNode->GetChildValue( "TotalCalories", m_Profile.TotalCaloriesBurned );
    pNode->GetChildValue( "Streak", m_Profile.StreakDays );

    RString sLastLogin;
    pNode->GetChildValue( "LastLogin", sLastLogin );
    m_Profile.LastLoginDate = sLastLogin;

    const XNode *pHistory = pNode->GetChild( "History" );
    if( pHistory )
    {
        FOREACH_CONST_Child( pHistory, txn )
        {
            WorkoutSession s;
            txn->GetAttrValue( "Date", s.Date );
            txn->GetAttrValue( "Playlist", s.PlaylistName );
            txn->GetAttrValue( "Duration", s.Duration );
            txn->GetAttrValue( "Calories", s.CaloriesBurned );
            m_History.push_back( s );
        }
    }

    RecalculateBMI();
}

XNode* GymManager::CreateNode() const
{
	XNode *xml = new XNode( "Gym" );
    xml->AppendChild( "Weight", m_Profile.Weight );
    xml->AppendChild( "Height", m_Profile.Height );
    xml->AppendChild( "DailyGoal", m_Profile.DailyGoal );
    xml->AppendChild( "TotalCalories", m_Profile.TotalCaloriesBurned );
    xml->AppendChild( "Streak", m_Profile.StreakDays );
    xml->AppendChild( "LastLogin", m_Profile.LastLoginDate );

    XNode *pHistory = xml->AppendChild( "History" );
    for( const auto& s : m_History )
    {
        XNode *txn = pHistory->AppendChild( "Session" );
        txn->AppendAttr( "Date", s.Date );
        txn->AppendAttr( "Playlist", s.PlaylistName );
        txn->AppendAttr( "Duration", s.Duration );
        txn->AppendAttr( "Calories", s.CaloriesBurned );
    }
	return xml;
}

void GymManager::ReadFromDisk()
{
	if( !IsAFile(GYM_DAT) ) return;
	XNode xml;
	if( !XmlFileUtil::LoadFromFileShowErrors(xml, GYM_DAT) ) return;
	LoadFromNode( &xml );
}

void GymManager::WriteToDisk()
{
	RageFile f;
	if( !f.Open(GYM_DAT, RageFile::WRITE|RageFile::SLOW_FLUSH) ) return;
	std::unique_ptr<XNode> xml( CreateNode() );
	XmlFileUtil::SaveToFile( xml.get(), f );
}

void GymManager::RecalculateBMI()
{
    float h_m = m_Profile.Height / 100.0f;
    if( h_m > 0 )
        m_Profile.BMI = m_Profile.Weight / (h_m * h_m);
}

void GymManager::UpdateStreak()
{
    RString today = DateTime::GetNowDate().GetString();
    if( m_Profile.LastLoginDate != today )
    {
        // Simple streak logic: check if last login was yesterday?
        // For simplicity, just increment if played.
        // Reset daily calories
        m_Profile.CaloriesToday = 0;
        m_Profile.LastLoginDate = today;
    }
}

const GymProfile& GymManager::GetProfile() const
{
    return m_Profile;
}

void GymManager::UpdateWeight( float fWeight )
{
    m_Profile.Weight = fWeight;
    RecalculateBMI();
}

void GymManager::UpdateHeight( float fHeight )
{
    m_Profile.Height = fHeight;
    RecalculateBMI();
}

void GymManager::SetDailyGoal( float fGoal )
{
    m_Profile.DailyGoal = fGoal;
}

void GymManager::LogWorkout( const RString& sPlaylist, float fDuration, float fCalories )
{
    WorkoutSession s;
    s.Date = DateTime::GetNowDate().GetString();
    s.PlaylistName = sPlaylist;
    s.Duration = fDuration;
    s.CaloriesBurned = fCalories;
    m_History.push_back(s);

    m_Profile.TotalCaloriesBurned += fCalories;
    m_Profile.CaloriesToday += fCalories;
    m_Profile.StreakDays++; // Increment streak on workout
}

const std::vector<WorkoutSession>& GymManager::GetHistory() const
{
    return m_History;
}

// Lua
class LunaGymManager: public Luna<GymManager>
{
public:
    static int GetProfile( T* p, lua_State *L )
    {
        const GymProfile& prof = p->GetProfile();
        lua_newtable(L);
        lua_pushstring(L, "Weight"); lua_pushnumber(L, prof.Weight); lua_settable(L, -3);
        lua_pushstring(L, "Height"); lua_pushnumber(L, prof.Height); lua_settable(L, -3);
        lua_pushstring(L, "BMI"); lua_pushnumber(L, prof.BMI); lua_settable(L, -3);
        lua_pushstring(L, "DailyGoal"); lua_pushnumber(L, prof.DailyGoal); lua_settable(L, -3);
        lua_pushstring(L, "TotalCaloriesBurned"); lua_pushnumber(L, prof.TotalCaloriesBurned); lua_settable(L, -3);
        lua_pushstring(L, "StreakDays"); lua_pushnumber(L, prof.StreakDays); lua_settable(L, -3);
        lua_pushstring(L, "TodayCalories"); lua_pushnumber(L, prof.CaloriesToday); lua_settable(L, -3);
        return 1;
    }

    static int UpdateWeight( T* p, lua_State *L )
    {
        p->UpdateWeight( FArg(1) );
        return 0;
    }

    static int LogWorkout( T* p, lua_State *L )
    {
        RString pl = SArg(1);
        float dur = FArg(2);
        float cal = FArg(3);
        p->LogWorkout(pl, dur, cal);
        return 0;
    }

    static int GeneratePlaylist( T* p, lua_State *L )
    {
        float dur = FArg(1);
        int minM = IArg(2);
        int maxM = IArg(3);
        std::vector<Song*> playlist = GymPlaylistGenerator::GeneratePlaylist( dur, minM, maxM );

        bool success = GymPlaylistGenerator::StartPlaylistAsCourse( playlist );

        lua_pushboolean(L, success);
        return 1;
    }

    LunaGymManager()
    {
        ADD_METHOD( GetProfile );
        ADD_METHOD( UpdateWeight );
        ADD_METHOD( LogWorkout );
        ADD_METHOD( GeneratePlaylist );
    }
};

LUA_REGISTER_CLASS( GymManager )
