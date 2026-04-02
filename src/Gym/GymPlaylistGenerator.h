#ifndef GYM_PLAYLIST_GENERATOR_H
#define GYM_PLAYLIST_GENERATOR_H

#include "Song.h"
#include <vector>

class GymPlaylistGenerator
{
public:
    static std::vector<Song*> GeneratePlaylist( float fTargetDuration, int iMinMeter, int iMaxMeter );
    static bool StartPlaylistAsCourse( const std::vector<Song*>& playlist );
};

#endif
