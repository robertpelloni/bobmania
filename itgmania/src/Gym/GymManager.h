#ifndef GYM_MANAGER_H
#define GYM_MANAGER_H

#include "RageTypes.h"
#include <map>
#include <vector>

struct lua_State;
class XNode;

struct GymProfile {
    float Weight; // kg
    float Height; // cm
    float BMI;
    float DailyGoal; // kcal
    float TotalCaloriesBurned;
    int StreakDays;
    float CaloriesToday;
    std::string LastLoginDate;
};

struct WorkoutSession {
    RString Date;
    RString PlaylistName;
    float Duration; // minutes
    float CaloriesBurned;
};

class GymManager
{
public:
	GymManager();
	~GymManager();

	void Init();

    // Profile Management
    const GymProfile& GetProfile() const;
    void UpdateWeight( float fWeight );
    void UpdateHeight( float fHeight );
    void SetDailyGoal( float fGoal );

    // Workout Tracking
    void LogWorkout( const RString& sPlaylist, float fDuration, float fCalories );
    const std::vector<WorkoutSession>& GetHistory() const;

	// Persistence
	void LoadFromNode( const XNode *pNode );
	XNode *CreateNode() const;
	void ReadFromDisk();
	void WriteToDisk();

	// Lua
	void PushSelf( lua_State *L );

private:
    GymProfile m_Profile;
    std::vector<WorkoutSession> m_History;

    void RecalculateBMI();
    void UpdateStreak();
};

extern GymManager*	GYMMAN;	// global singleton

#endif
