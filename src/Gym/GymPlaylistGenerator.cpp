#include "global.h"
#include "GymPlaylistGenerator.h"
#include "SongManager.h"
#include "Song.h"
#include "Steps.h"
#include "GameState.h"
#include "Course.h"
#include "RageUtil.h"

std::vector<Song*> GymPlaylistGenerator::GeneratePlaylist( float fTargetDuration, int iMinMeter, int iMaxMeter )
{
    std::vector<Song*> playlist;
    float currentDuration = 0.0f;

    // Get all songs
    const std::vector<Song*> &allSongs = SONGMAN->GetAllSongs();
    if( allSongs.empty() ) return playlist;

    std::vector<Song*> candidates;
    for( Song* s : allSongs )
    {
        // Check if song has music length > 30s and < 5 mins
        if( s->m_fMusicLengthSeconds < 30.0f || s->m_fMusicLengthSeconds > 300.0f )
            continue;

        // Check intensity (Meter)
        bool bHasValidSteps = false;
        const std::vector<Steps*> &vpSteps = s->GetAllSteps();
        for( const Steps* steps : vpSteps )
        {
            if( steps->GetMeter() >= iMinMeter && steps->GetMeter() <= iMaxMeter )
            {
                bHasValidSteps = true;
                break;
            }
        }

        if( bHasValidSteps )
            candidates.push_back(s);
    }

    if( candidates.empty() ) return playlist;

    int safeguard = 0;
    while( currentDuration < fTargetDuration && safeguard < 100 )
    {
        // Pick random song
        Song* s = candidates[ RandomInt(candidates.size()) ];
        playlist.push_back(s);
        currentDuration += s->m_fMusicLengthSeconds;
        safeguard++;
    }

    return playlist;
}

bool GymPlaylistGenerator::StartPlaylistAsCourse( const std::vector<Song*>& playlist )
{
    if( playlist.empty() ) return false;

    // Create a new, temporary course
    Course *pCourse = new Course;
    pCourse->m_bIsAutogen = true;
    pCourse->m_sMainTitle = "Gym Workout";
    pCourse->m_sBannerPath = ""; // TODO: Default Gym Banner

    for( Song* s : playlist )
    {
        CourseEntry ce;
        ce.songID.FromSong( s );
        // For now, we assume standard difficulty (Medium/Hard) or use the player's preferred difficulty
        // Since we can't easily guess here, we leave it to the GameState to pick the Steps based on current difficulty.
        pCourse->m_vEntries.push_back( ce );
    }

    // Register with SONGMAN so it's managed (or at least valid for the session)
    // Actually, we can just set it in GameState if it's a one-off.
    // But StepMania expects Courses to be in SONGMAN for some lookups.
    // For this implementation, let's try setting it directly in GameState.

    GAMESTATE->m_pCurCourse.Set( pCourse );
    GAMESTATE->m_PlayMode.Set( PLAY_MODE_NONSTOP ); // Using NONSTOP as generic course mode

    // We need to ensure the Course is "Finished" loading (caches cleared etc)
    // pCourse->RegenerateNonFixedTrails(); // If we had trails.

    return true;
}
