#ifndef MISSION_MANAGER_H
#define MISSION_MANAGER_H

#include "RageTypes.h"
#include <map>
#include <vector>

struct lua_State;
class XNode;

struct Mission {
    RString ID;
    RString Title;
    RString Description;
    RString GoalType; // "SongsPassed", "Calories", "NotesHit"
    float TargetValue;
    float CurrentProgress;
    bool Completed;
    bool Claimed;
    RString Reward; // e.g. "500 XP"
};

class MissionManager
{
public:
	MissionManager();
	~MissionManager();

	void Init();

    // Mission Handling
    const std::vector<Mission>& GetMissions() const;
    void ReportMetric( const RString& sType, float fValue ); // Updates progress
    bool ClaimReward( const RString& sMissionID );

	// Persistence
	void LoadFromNode( const XNode *pNode );
	XNode *CreateNode() const;
	void ReadFromDisk();
	void WriteToDisk();

	// Lua
	void PushSelf( lua_State *L );

private:
    std::vector<Mission> m_Missions;

    // Helper to find index
    int GetMissionIndex( const RString& sID );
};

extern MissionManager* MISSIONMAN;

#endif
