#include "global.h"

#include "GameSoundManager.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "AnnouncerManager.h"
#include "GameState.h"
#include "LightsManager.h"
#include "LuaManager.h"
#include "NoteData.h"
#include "NotesLoaderSM.h"
#include "NotesLoaderSSC.h"
#include "PrefsManager.h"
#include "RageDisplay.h"
#include "RageLog.h"
#include "RageSound.h"
#include "RageSoundManager.h"
#include "RageUtil.h"
#include "Song.h"
#include "SongUtil.h"
#include "Steps.h"
#include "TimingData.h"
#include "arch/Sound/RageSoundDriver.h"

GameSoundManager* SOUND = nullptr;

// When playing music, automatically search for an SM file for timing data.  If
// one is found, automatically handle GAMESTATE->m_fSongBeat, etc.
//
// std::modf(GAMESTATE->m_fSongBeat) should always be continuously moving from 0
// to 1.  To do this, wait before starting a sound until the fractional portion
// of the beat will be the same.
//
// If PlayMusic(length_seconds) is set, peek at the beat, and extend the length
// so we'll be on the same fractional beat when we loop.

// Lock this before touching g_UpdatingTimer or g_Playing.
static RageEvent* g_Mutex;
static bool g_UpdatingTimer;
static bool g_Shutdown;
static bool g_bFlushing = false;

enum FadeState { FADE_NONE, FADE_OUT, FADE_WAIT, FADE_IN };
static FadeState g_FadeState = FADE_NONE;
static float g_fDimVolume = 1.0f;
static float g_fOriginalVolume = 1.0f;
static float g_fDimDurationRemaining = 0.0f;
static bool g_bWasPlayingOnLastUpdate = false;

struct MusicPlaying {
  bool timing_delayed;
  bool has_timing;
  bool apply_music_rate;
  // The timing data that we're currently using.
  TimingData timing;
  NoteData lights;

  // If timing_delayed is true, this will be the timing data for the song that's
  // starting. We'll copy it to timing once sound is heard.
  TimingData new_timing;
  RageSound* music;
  MusicPlaying(RageSound* Music) {
    timing.AddSegment(BPMSegment(0, 120));
    new_timing.AddSegment(BPMSegment(0, 120));
    has_timing = false;
    timing_delayed = false;
    apply_music_rate = false;
    music = Music;
  }

  ~MusicPlaying() { delete music; }
};

static MusicPlaying* g_Playing;

static RageThread MusicThread;

std::vector<std::string> g_SoundsToPlayOnce;
std::vector<std::string> g_SoundsToPlayOnceFromDir;
std::vector<std::string> g_SoundsToPlayOnceFromAnnouncer;
// This should get updated to unordered_map when once C++11 is supported
std::map<std::string, std::vector<int>> g_DirSoundOrder;

struct MusicToPlay {
  RString file;
  RString timing_file;
  bool has_timing;
  TimingData timing_data;
  NoteData lights_data;
  bool force_loop;
  float start_second;
  float length_seconds;
  float fade_in_length_seconds;
  float fade_out_length_seconds;
  bool align_beat;
  float apply_music_rate;
  MusicToPlay() { has_timing = false; }
};
std::vector<MusicToPlay> g_MusicsToPlay;
static GameSoundManager::PlayMusicParams g_FallbackMusicParams;


    RageSoundParams rage_sound_params = g_Playing->music->GetParams();
    if (rage_sound_params.m_Volume != volume) {
      rage_sound_params.m_Volume = volume;
      g_Playing->music->SetParams(rage_sound_params);
    }
  }

  if (!g_UpdatingTimer) {
    return;
  }

  const float adjust = GetFrameTimingAdjustment(delta);
  if (!g_Playing->music->IsPlaying()) {
    // There's no song playing. Fake it.
    CHECKPOINT_M(
        ssprintf("%f, delta %f", GAMESTATE->position_.m_fMusicSeconds, delta));
    GAMESTATE->UpdateSongPosition(
        GAMESTATE->position_.m_fMusicSeconds +
            delta * g_Playing->music->GetPlaybackRate(),
        g_Playing->timing);
    return;
  }

  // There's a delay between us calling Play() and the sound actually playing.
  // During this time, approximate will be true.  Keep using the previous
  // timing data until we get a non-approximate time, indicating that the sound
  // has actually started playing.
  bool approximate;
  RageTimer timer;
  const float seconds =
      g_Playing->music->GetPositionSeconds(&approximate, &timer);

  // Check for song timing skips.
  if (PREFSMAN->m_bLogSkips && !g_Playing->timing_delayed) {
    const float expected_time_passed =
        (timer - GAMESTATE->position_.m_LastBeatUpdate) *
        g_Playing->music->GetPlaybackRate();
    const float sound_time_passed =
        seconds - GAMESTATE->position_.m_fMusicSeconds;
    const float difference = expected_time_passed - sound_time_passed;

    static RString last_file = "";
    const RString this_file = g_Playing->music->GetLoadedFilePath();

    // If sound_time_passed < 0, the sound has probably looped.
    if (last_file == this_file && sound_time_passed >= 0 &&
        std::abs(difference) > 0.003f) {
      LOG->Trace(
          "Song position skip in %s: expected %.3f, got %.3f (cur %f, prev %f) "
          "(%.3f difference)",
          Basename(this_file).c_str(), expected_time_passed, sound_time_passed,
          seconds, GAMESTATE->position_.m_fMusicSeconds, difference);
    }
    last_file = this_file;
  }

  // If g_Playing->timing_delayed, we're waiting for the new music to actually
  // start playing.
  if (g_Playing->timing_delayed && !approximate) {
    // Load up the new timing data.
    g_Playing->timing = g_Playing->new_timing;
    g_Playing->timing_delayed = false;
  }

  if (g_Playing->timing_delayed) {
    // We're still waiting for the new sound to start playing, so keep using the
    // old timing data and fake the time.
    GAMESTATE->UpdateSongPosition(
        GAMESTATE->position_.m_fMusicSeconds + delta, g_Playing->timing);
  } else {
    GAMESTATE->UpdateSongPosition(
        seconds + adjust, g_Playing->timing, timer + adjust);
  }

  // Send crossed messages
  if (GAMESTATE->cur_song_) {
    static int beat_last_crossed = 0;

    float song_beat = GAMESTATE->position_.m_fSongBeat;

    int row_now = BeatToNoteRowNotRounded(song_beat);
    row_now = std::max(0, row_now);

    int beat_now = row_now / ROWS_PER_BEAT;

    for (int beat = beat_last_crossed + 1; beat <= beat_now; ++beat) {
      Message msg("CrossedBeat");
      msg.SetParam("Beat", beat);
      MESSAGEMAN->Broadcast(msg);
    }

    beat_last_crossed = beat_now;
  }

  // Update lights
  NoteData& lights = g_Playing->lights;
  // Lights data was loaded
  if (lights.GetNumTracks() > 0) {
    const float song_beat = GAMESTATE->position_.m_fLightSongBeat;
    const int song_row = BeatToNoteRowNotRounded(song_beat);

    static int row_last_crossed = 0;

    FOREACH_CabinetLight(cl) {
      // Are we "holding" the light?
      if (lights.IsHoldNoteAtRow(cl, song_row)) {
        LIGHTSMAN->BlinkCabinetLight(cl);
        continue;
      }

      // Otherwise, for each index we crossed since the last update:
      FOREACH_NONEMPTY_ROW_IN_TRACK_RANGE(
          lights, cl, r, row_last_crossed + 1, song_row + 1) {
        if (lights.GetTapNote(cl, r).type != TapNoteType_Empty) {
          LIGHTSMAN->BlinkCabinetLight(cl);
          break;
        }
      }
    }

    row_last_crossed = song_row;
  }
}

RString GameSoundManager::GetMusicPath() const {
  LockMut(*g_Mutex);
  return g_Playing->music->GetLoadedFilePath();
}

void GameSoundManager::PlayMusic(
    RString file, const TimingData* timing, bool force_loop, float start_second,
    float length_seconds, float fade_in_length_seconds,
    float fade_out_length_seconds, bool align_beat, bool apply_music_rate) {
  PlayMusicParams params;
  params.file = file;
  params.timing = timing;
  params.force_loop = force_loop;
  params.start_second = start_second;
  params.length_seconds = length_seconds;
  params.fade_in_length_seconds = fade_in_length_seconds;
  params.fade_out_length_seconds = fade_out_length_seconds;
  params.align_beat = align_beat;
  params.apply_music_rate = apply_music_rate;
  PlayMusic(params);
}

void GameSoundManager::PlayMusic(
    PlayMusicParams params, PlayMusicParams fallback_music_params) {
  g_FallbackMusicParams = fallback_music_params;

  MusicToPlay to_play;

  to_play.file = params.file;
  if (params.timing) {
    to_play.has_timing = true;
    to_play.timing_data = *params.timing;
  } else {
    // If no timing data was provided, look for it in the same place as the
    // music file.
    // TODO(aj): allow loading .ssc files as well
    to_play.timing_file = SetExtension(params.file, "sm");
  }

  to_play.force_loop = params.force_loop;
  to_play.start_second = params.start_second;
  to_play.length_seconds = params.length_seconds;
  to_play.fade_in_length_seconds = params.fade_in_length_seconds;
  to_play.fade_out_length_seconds = params.fade_out_length_seconds;
  to_play.align_beat = params.align_beat;
  to_play.apply_music_rate = params.apply_music_rate;

  // Add the MusicToPlay to the g_MusicsToPlay queue.
  g_Mutex->Lock();
  g_MusicsToPlay.push_back(to_play);
  g_Mutex->Broadcast();
  g_Mutex->Unlock();
}

void GameSoundManager::DimMusic(float volume, float duration_seconds) {
  LockMut(*g_Mutex);

  if (g_FadeState == FADE_NONE) {
    g_fOriginalVolume = g_Playing->music->GetParams().m_Volume;
  }
  // otherwise, g_fOriginalVolume is already set and m_Volume will be the
  // current state, not the original state

  g_fDimDurationRemaining = duration_seconds;
  g_fDimVolume = volume;
  g_FadeState = FADE_OUT;
}

void GameSoundManager::HandleSongTimer(bool on) {
  LockMut(*g_Mutex);
  g_UpdatingTimer = on;
}

void GameSoundManager::PlayOnce(RString path) {
  // Add the sound to the g_SoundsToPlayOnce queue.
  g_Mutex->Lock();
  g_SoundsToPlayOnce.push_back(path);
  g_Mutex->Broadcast();
  g_Mutex->Unlock();
}

void GameSoundManager::PlayOnceFromDir(RString path) {
  // Add the path to the g_SoundsToPlayOnceFromDir queue.
  g_Mutex->Lock();
  g_SoundsToPlayOnceFromDir.push_back(path);
  g_Mutex->Broadcast();
  g_Mutex->Unlock();
}

void GameSoundManager::PlayOnceFromAnnouncer(RString path) {
  // Add the path to the g_SoundsToPlayOnceFromAnnouncer queue.
  g_Mutex->Lock();
  g_SoundsToPlayOnceFromAnnouncer.push_back(path);
  g_Mutex->Broadcast();
  g_Mutex->Unlock();
}

float GameSoundManager::GetPlayerBalance(PlayerNumber pn) {
  // If two players are active, play sounds on each players' side.
  if (GAMESTATE->GetNumPlayersEnabled() == 2) {
    return (pn == PLAYER_1) ? -1.0f : 1.0f;
  } else {
    return 0;
  }
}

#include "LuaBinding.h"

// Allow Lua to have access to the GameSoundManager.
class LunaGameSoundManager : public Luna<GameSoundManager> {
 public:
  static int DimMusic(T* p, lua_State* L) {
    float volume = FArg(1);
    float duration_seconds = FArg(2);
    p->DimMusic(volume, duration_seconds);
    COMMON_RETURN_SELF;
  }
  static int PlayOnce(T* p, lua_State* L) {
    RString path = SArg(1);
    if (lua_toboolean(L, 2) && PREFSMAN->m_MuteActions) {
      COMMON_RETURN_SELF;
    }
    p->PlayOnce(path);
    COMMON_RETURN_SELF;
  }
  static int PlayAnnouncer(T* p, lua_State* L) {
    RString path = SArg(1);
    p->PlayOnceFromAnnouncer(path);
    COMMON_RETURN_SELF;
  }
  static int GetPlayerBalance(T* p, lua_State* L) {
    PlayerNumber pn = Enum::Check<PlayerNumber>(L, 1);
    lua_pushnumber(L, p->GetPlayerBalance(pn));
    return 1;
  }
  static int PlayMusicPart(T* p, lua_State* L) {
    RString music_path = SArg(1);
    float music_start = FArg(2);
    float music_length = FArg(3);
    float fade_in = 0;
    float fade_out = 0;
    bool loop = false;
    bool apply_rate = false;
    bool align_beat = true;
    if (!lua_isnoneornil(L, 4)) {
      fade_in = FArg(4);
    }
    if (!lua_isnoneornil(L, 5)) {
      fade_out = FArg(5);
    }
    if (!lua_isnoneornil(L, 6)) {
      loop = BArg(6);
    }
    if (!lua_isnoneornil(L, 7)) {
      apply_rate = BArg(7);
    }
    if (!lua_isnoneornil(L, 8)) {
      align_beat = BArg(8);
    }
    p->PlayMusic(
        music_path, nullptr, loop, music_start, music_length, fade_in, fade_out,
        align_beat, apply_rate);
    COMMON_RETURN_SELF;
  }

  static int StopMusic(T* p, lua_State* L) {
    p->StopMusic();
    COMMON_RETURN_SELF;
  }
  static int IsTimingDelayed(T* p, lua_State* L) {
    lua_pushboolean(L, g_Playing->timing_delayed);
    return 1;
  }

  LunaGameSoundManager() {
    ADD_METHOD(DimMusic);
    ADD_METHOD(PlayOnce);
    ADD_METHOD(PlayAnnouncer);
    ADD_METHOD(GetPlayerBalance);
    ADD_METHOD(PlayMusicPart);
    ADD_METHOD(StopMusic);
    ADD_METHOD(IsTimingDelayed);
  }
};

LUA_REGISTER_CLASS(GameSoundManager);

int LuaFunc_get_sound_driver_list(lua_State* L);
=======
>>>>>>> Stashed changes
}
LUAFUNC_REGISTER_COMMON(get_sound_driver_list);

/*
 * Copyright (c) 2003-2005 Glenn Maynard
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
