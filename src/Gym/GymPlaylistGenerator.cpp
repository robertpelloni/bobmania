#include "global.h"
#include "GymPlaylistGenerator.h"
#include "SongManager.h"
#include "MsdCalculator.h"
#include "RageUtil.h"
#include "GameState.h"
#include "GameManager.h"
#include "Steps.h"
#include "Style.h"
#include "Song.h"
#include "LuaManager.h"
#include "EnumHelper.h"

Course* GymPlaylistGenerator::GenerateCircuit( float targetDurationSeconds, WorkoutIntensity intensity )
{
	Course* pCourse = new Course;
	pCourse->m_sMainTitle = "Daily Circuit";
	pCourse->m_sScripter = "AI Trainer";
	pCourse->m_bIsAutogen = true;

	// Determine target meter range based on intensity
	int minMeter = 1;
	int maxMeter = 3;

	switch( intensity )
	{
	case INTENSITY_LIGHT:      minMeter = 1; maxMeter = 3; break;
	case INTENSITY_MODERATE:   minMeter = 4; maxMeter = 7; break;
	case INTENSITY_VIGOROUS:   minMeter = 8; maxMeter = 11; break;
	case INTENSITY_PRO_ATHLETE: minMeter = 12; maxMeter = 16; break;
	}

	float currentDuration = 0.0f;
	const std::vector<Song*> &allSongs = SONGMAN->GetAllSongs();

	if( allSongs.empty() ) return pCourse;

	// Filter songs that have at least one stepchart in the range for the current style
	struct Candidate {
		Song* pSong;
		Steps* pSteps;
	};
	std::vector<Candidate> candidates;

	const Style* pStyle = GAMESTATE->GetCurrentStyle(PLAYER_INVALID);
	StepsType st = pStyle ? pStyle->m_StepsType : StepsType_dance_single;

	for( Song* pSong : allSongs )
	{
		const std::vector<Steps*>& vpSteps = pSong->GetAllSteps();
		for( Steps* pSteps : vpSteps )
		{
			if( pSteps->m_StepsType == st && 
				pSteps->GetMeter() >= minMeter && 
				pSteps->GetMeter() <= maxMeter )
			{
				candidates.push_back( { pSong, pSteps } );
			}
		}
	}

	if( candidates.empty() )
	{
		// Fallback: Just pick random songs and don't enforce steps
		for( int i=0; i<10; ++i ) {
			Song* pSong = allSongs[ RandomInt(allSongs.size()) ];
			CourseEntry entry;
			entry.songID.FromSong( pSong );
			pCourse->m_vEntries.push_back( entry );
			currentDuration += pSong->m_fMusicLengthSeconds;
			if( currentDuration >= targetDurationSeconds ) break;
		}
		return pCourse;
	}

	int attempts = 0;
	while( currentDuration < targetDurationSeconds && attempts < 200 )
	{
		const Candidate& cand = candidates[ RandomInt(candidates.size()) ];

		CourseEntry entry;
		entry.songID.FromSong( cand.pSong );
		entry.stepsCriteria.m_difficulty = cand.pSteps->GetDifficulty();
		entry.bNoDifficult = true; // Force this difficulty

		pCourse->m_vEntries.push_back( entry );
		currentDuration += cand.pSong->m_fMusicLengthSeconds;
		attempts++;
	}

	return pCourse;
}

float GymPlaylistGenerator::EstimateCalories( Song* pSong, float fRate )
{
	if(!pSong) return 0.0f;
	// Rough approximation: Length * Rate * Factor
	return pSong->m_fMusicLengthSeconds * fRate * 0.20f; 
}

// Lua bindings for GymPlaylistGenerator

static const char *WorkoutIntensityNames[] = {
	"Light",
	"Moderate",
	"Vigorous",
	"ProAthlete",
};
XBOX360_NAMED_ENUM( WorkoutIntensity, WorkoutIntensityNames );

class LunaGymPlaylistGenerator: public Luna<GymPlaylistGenerator>
{
public:
	static int GenerateCircuit( T* p, lua_State *L )
	{
		float duration = FArg(1);
		WorkoutIntensity intensity = Enum::Check<WorkoutIntensity>(L, 2);
		
		Course* pCourse = GymPlaylistGenerator::GenerateCircuit( duration, intensity );
		if( pCourse )
			pCourse->PushSelf(L);
		else
			lua_pushnil(L);
		
		return 1;
	}

	static int EstimateCalories( T* p, lua_State *L )
	{
		Song* pSong = Luna<Song>::check(L, 1);
		float rate = FArg(2);
		
		float calories = GymPlaylistGenerator::EstimateCalories( pSong, rate );
		lua_pushnumber(L, calories);
		return 1;
	}

	LunaGymPlaylistGenerator()
	{
		ADD_METHOD( GenerateCircuit );
		ADD_METHOD( EstimateCalories );
	}
};

LUA_REGISTER_CLASS( GymPlaylistGenerator )

