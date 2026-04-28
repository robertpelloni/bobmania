#ifndef REPLAY_MANAGER_H
#define REPLAY_MANAGER_H

#include "GameInput.h"
#include "RageTimer.h"
#include <vector>

struct lua_State;

struct ReplayInput
{
    float fTime;
    GameInput Input;
    bool bDown; // True if pressed, false if released
};

class ReplayManager
{
public:
    ReplayManager();
    ~ReplayManager();

    void Init();

    // Recording
    void StartRecording();
    void StopRecording();
    void RecordInput( float fTime, const GameInput& input, bool bDown );
    bool IsRecording() const;
    void SaveReplay( const RString& sPath );

    // Playback
    void LoadReplay( const RString& sPath );
    void StartPlayback();
    void StopPlayback();
    bool IsPlaying() const;
    void Update( float fDeltaTime ); // Used to inject inputs during playback

    // Lua
    void PushSelf( lua_State *L );

private:
    bool m_bRecording;
    bool m_bPlaying;
    RageTimer m_RecordTimer;
    float m_fPlaybackTime;
    unsigned int m_iPlaybackIndex;

    std::vector<ReplayInput> m_ReplayData;
};

extern ReplayManager* REPLAYMAN;

#endif
