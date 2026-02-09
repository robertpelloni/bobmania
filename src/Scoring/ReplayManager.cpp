#include "global.h"
#include "ReplayManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "InputFilter.h"
#include "ScreenManager.h"
#include "RageFile.h"
#include "RageInputDevice.h"

ReplayManager* REPLAYMAN = nullptr;

ReplayManager::ReplayManager()
{
    m_bRecording = false;
    m_bPlaying = false;
    m_fPlaybackTime = 0;
    m_iPlaybackIndex = 0;
}

ReplayManager::~ReplayManager()
{
}

void ReplayManager::Init()
{
}

void ReplayManager::StartRecording()
{
    m_bRecording = true;
    m_ReplayData.clear();
    m_RecordTimer.Touch();
    LOG->Trace("ReplayManager: Recording Started");
}

void ReplayManager::StopRecording()
{
    m_bRecording = false;
    LOG->Trace("ReplayManager: Recording Stopped. %d events captured.", (int)m_ReplayData.size());
}

void ReplayManager::RecordInput( float fTime, const GameInput& input, bool bDown )
{
    if( !m_bRecording ) return;

    // Normalize time to start of recording
    // Actually, ScreenGameplay passes the song time usually, or we use our timer.
    // For MVP, we'll trust the caller provided fTime or use our timer if fTime is -1.

    float t = fTime;
    if( t < 0 ) t = m_RecordTimer.Ago();

    ReplayInput ri;
    ri.fTime = t;
    ri.Input = input;
    ri.bDown = bDown;
    m_ReplayData.push_back(ri);
}

bool ReplayManager::IsRecording() const
{
    return m_bRecording;
}

void ReplayManager::SaveReplay( const RString& sPath )
{
    // Simple CSV format for MVP
    RageFile f;
    if( !f.Open(sPath, RageFile::WRITE) )
    {
        LOG->Warn("Failed to save replay to %s", sPath.c_str());
        return;
    }

    for( const auto& ri : m_ReplayData )
    {
        f.PutLine( ssprintf("%f,%d,%d,%d", ri.fTime, ri.Input.controller, ri.Input.button, ri.bDown ? 1 : 0) );
    }
    f.Close();
}

void ReplayManager::LoadReplay( const RString& sPath )
{
    m_ReplayData.clear();

    RageFile f;
    if( !f.Open(sPath, RageFile::READ) )
    {
        LOG->Warn("Failed to load replay from %s", sPath.c_str());
        return;
    }

    RString line;
    while( f.GetLine(line) > 0 )
    {
        float t;
        int c, b, d;
        if( sscanf(line.c_str(), "%f,%d,%d,%d", &t, &c, &b, &d) == 4 )
        {
            ReplayInput ri;
            ri.fTime = t;
            ri.Input.controller = (GameController)c;
            ri.Input.button = (GameButton)b;
            ri.bDown = (d != 0);
            m_ReplayData.push_back(ri);
        }
    }
}

void ReplayManager::StartPlayback()
{
    if( m_ReplayData.empty() ) return;
    m_bPlaying = true;
    m_iPlaybackIndex = 0;
    m_fPlaybackTime = 0;
    LOG->Trace("ReplayManager: Playback Started");
}

void ReplayManager::StopPlayback()
{
    m_bPlaying = false;
}

bool ReplayManager::IsPlaying() const
{
    return m_bPlaying;
}

void ReplayManager::Update( float fDeltaTime )
{
    if( !m_bPlaying ) return;

    m_fPlaybackTime += fDeltaTime;

    while( m_iPlaybackIndex < m_ReplayData.size() )
    {
        const ReplayInput& ri = m_ReplayData[m_iPlaybackIndex];
        if( ri.fTime <= m_fPlaybackTime )
        {
            // Inject Input
            INPUTFILTER->ButtonPressed( DeviceInput(DEVICE_KEYBOARD, KEY_F12) ); // Mock device
            // Ideally we inject into InputFilter directly or GameInput logic.
            // For MVP, we can't easily inject "real" inputs without deep engine access.
            // But we can broadcast messages or use a hook in ScreenGameplay.

            m_iPlaybackIndex++;
        }
        else
        {
            break;
        }
    }

    if( m_iPlaybackIndex >= m_ReplayData.size() )
    {
        StopPlayback();
    }
}

// Lua
class LunaReplayManager: public Luna<ReplayManager>
{
public:
    static int StartRecording( T* p, lua_State *L ) { p->StartRecording(); return 0; }
    static int StopRecording( T* p, lua_State *L ) { p->StopRecording(); return 0; }
    static int SaveReplay( T* p, lua_State *L ) { p->SaveReplay(SArg(1)); return 0; }
    static int LoadReplay( T* p, lua_State *L ) { p->LoadReplay(SArg(1)); return 0; }
    static int StartPlayback( T* p, lua_State *L ) { p->StartPlayback(); return 0; }

    LunaReplayManager()
    {
        ADD_METHOD( StartRecording );
        ADD_METHOD( StopRecording );
        ADD_METHOD( SaveReplay );
        ADD_METHOD( LoadReplay );
        ADD_METHOD( StartPlayback );
    }
};

LUA_REGISTER_CLASS( ReplayManager )
