#include "global.h"
#include "GymPlaylistGenerator.h"
#include "SongManager.h"
#include "GameState.h"
#include "Course.h"
#include "CourseLoaderCRS.h"
#include "RageLog.h"
#include "Steps.h"

std::vector<Song*> GymPlaylistGenerator::GeneratePlaylist( float fTargetDuration, int iMinMeter, int iMaxMeter )
{
    std::vector<Song*> playlist;
    std::vector<Song*> allSongs = SONGMAN->GetAllSongs();

    // Safety check: if no songs loaded, return empty
    if( allSongs.empty() )
    {
        LOG->Warn("GymPlaylistGenerator: No songs loaded!");
        return playlist;
    }

    float currentDuration = 0.0f;
    int attempts = 0;

    // Seed RNG if needed (though usually done in Global)
    // srand(time(NULL));

    while( currentDuration < fTargetDuration && attempts < 1000 )
    {
        attempts++;

        // Random Pick
        int idx = rand() % allSongs.size();
        Song* pSong = allSongs[idx];

        // Basic Filter: Check if song has steps in range
        bool bSuitable = false;
        const std::vector<Steps*>& steps = pSong->GetAllSteps();
        for( const auto* step : steps )
        {
            int meter = step->GetMeter();
            if( meter >= iMinMeter && meter <= iMaxMeter )
            {
                bSuitable = true;
                break; // Found a chart that fits
            }
        }

        if( bSuitable )
        {
            // Avoid duplicates? For now, allow them if playlist is long.
            playlist.push_back(pSong);
            currentDuration += pSong->m_fMusicLengthSeconds;
        }
    }

    LOG->Trace("GymPlaylistGenerator: Generated playlist with %d songs, duration %.2f", (int)playlist.size(), currentDuration);
    return playlist;
}

bool GymPlaylistGenerator::StartPlaylistAsCourse( const std::vector<Song*>& playlist )
{
    if( playlist.empty() ) return false;

    // Create a temporary course
    // Note: Course memory management in SM5 is tricky. Usually Courses are owned by SONGMAN.
    // For dynamic courses, we might need a dedicated cleanup or use "Autogen" flag.
    Course* pCourse = new Course;
    pCourse->m_sMainTitle = "Gym Workout";
    pCourse->m_bIsAutogen = true; // Signals that it's generated
    pCourse->m_bRepeat = false;

    for( Song* pSong : playlist )
    {
        CourseEntry ce;
        ce.songID.FromSong( pSong );
        pCourse->m_vEntries.push_back( ce );
    }

    // We need to register this course so the engine doesn't leak it,
    // or just pass it to GAMESTATE and hope ScreenGameplay handles it.
    // GAMESTATE->m_pCurCourse takes a raw pointer.

    GAMESTATE->m_pCurCourse.Set( pCourse );

    // Set PlayMode to Course (In standard SM5, this might be PLAY_MODE_REGULAR with IsCourse set, or similar.
    // Assuming PLAY_MODE_NONSTOP or ONI for course-like behavior if COURSE doesn't exist)
    GAMESTATE->m_PlayMode.Set( PLAY_MODE_NONSTOP );

    // Set first song (engine might do this automatically when starting course, but safe to set)
    GAMESTATE->m_pCurSong.Set( playlist[0] );

    return true;
}
