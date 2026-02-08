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
        if( s->m_fMusicLengthSeconds > 30.0f && s->m_fMusicLengthSeconds < 300.0f )
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
    // Stub implementation
    return true;
}
