#ifndef GYM_PLAYLIST_GENERATOR_H
#define GYM_PLAYLIST_GENERATOR_H

#include "RageTypes.h"
#include <vector>

class Song;

namespace GymPlaylistGenerator
{
    // Generates a random playlist of songs matching the target duration (in seconds)
    // and difficulty range (e.g., 3-5 meter).
    std::vector<Song*> GeneratePlaylist( float fTargetDuration, int iMinMeter, int iMaxMeter );

    // Helper to start the playlist as a Course (replaces current game mode)
    bool StartPlaylistAsCourse( const std::vector<Song*>& playlist );
}

#endif
