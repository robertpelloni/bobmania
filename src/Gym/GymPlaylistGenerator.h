#ifndef GYM_PLAYLIST_GENERATOR_H
#define GYM_PLAYLIST_GENERATOR_H

#include <vector>
#include "Song.h"
#include "Course.h"

enum WorkoutIntensity {
	INTENSITY_LIGHT,
	INTENSITY_MODERATE,
	INTENSITY_VIGOROUS,
	INTENSITY_PRO_ATHLETE
};

class GymPlaylistGenerator
{
public:
	// Generates a dynamic course based on target duration and intensity
	static Course* GenerateCircuit( float targetDurationSeconds, WorkoutIntensity intensity );

	// Helper to estimate calories for a song at a given rate
	static float EstimateCalories( Song* pSong, float fRate );
};

#endif
