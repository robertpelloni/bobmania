#include "global.h"
#include "GymPlaylistGenerator.h"
#include "SongManager.h"
#include "MsdCalculator.h"
#include "RageUtil.h"
#include "GameState.h"
#include "GameManager.h"

Course* GymPlaylistGenerator::GenerateCircuit( float targetDurationSeconds, WorkoutIntensity intensity )
{
	Course* pCourse = new Course;
	pCourse->m_sMainTitle = "Daily Circuit";
	pCourse->m_sScripter = "AI Trainer";

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

	// Simple Greedy Algorithm for MVP
	// In production, this would be smarter (Warmup -> Peak -> Cooldown)

	int attempts = 0;
	while( currentDuration < targetDurationSeconds && attempts < 100 )
	{
		Song* pSong = allSongs[ RandomInt(allSongs.size()) ];

		// Find a step chart in our range
		// (Simplified logic: assumes we can find one)
		// Real implementation would iterate pSong->GetAllSteps()

		float fSongLen = pSong->m_fMusicLengthSeconds;

		CourseEntry entry;
		entry.pSong = pSong;
		// entry.pSteps = ... (In SM5 CourseEntry usually just holds the song and difficulty type, not exact steps pointer)

		pCourse->m_vEntries.push_back( entry );
		currentDuration += fSongLen;
		attempts++;
	}

	return pCourse;
}

float GymPlaylistGenerator::EstimateCalories( Song* pSong, float fRate )
{
	// Basic formula: Time * Intensity Factor
	if(!pSong) return 0.0f;
	return pSong->m_fMusicLengthSeconds * fRate * 0.15f; // Rough kcal approximation
}
