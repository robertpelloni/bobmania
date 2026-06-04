#include "global.h"

=======
>>>>>>> upstream/release
=======
>>>>>>> Stashed changes
#if defined(WIN32) && defined(DEBUG)
  return false;
#else
  return true;
#endif
}

static void CheckFocus() {
  if (!HOOKS->AppFocusChanged()) {
    return;
  }


  delta *= g_fUpdateRate;

  // Update SOUNDMAN early (before any RageSound::GetPosition calls), to flush
  // position data.
  SOUNDMAN->Update();

  // NOTE(Glenn): Update song beat information -before- calling update on all
	// the classes that depend on it. If you don't do this first, the classes are
	// all acting on old information and will lag. (but no longer fatally, due to
  // timestamping).
  SOUND->Update(delta);
  TEXTUREMAN->Update(delta);
  GAMESTATE->Update(delta);
  SCREENMAN->Update(delta);
  MEMCARDMAN->Update();

  // Important: Process input AFTER updating game logic, or input will be acting
	// on song beat from last frame
  HandleInputEvents(delta);

  // bandaid for low max audio sample counter
  SOUNDMAN->low_sample_count_workaround();
  LIGHTSMAN->Update(delta);
}

void GameLoop::RunGameLoop() {
  // People may want to do something else while songs are loading, so do this
	// after loading songs.
  if (ChangeAppPriority()) {
    HOOKS->BoostPriority();
  }

  while (!ArchHooks::UserQuit()) {
    if (!g_NewGame.empty()) {
      DoChangeGame();
    }
    if (!g_NewTheme.empty()) {
      DoChangeTheme();
    }

    CheckFocus();

    UpdateAllButDraw(false);

    if (INPUTMAN->DevicesChanged()) {
			// Fix "buttons stuck" once per frame if button held while unplugged.
      INPUTFILTER->Reset();
      INPUTMAN->LoadDrivers();
      RString sMessage;
      if (INPUTMAPPER->CheckForChangedInputDevicesAndRemap(sMessage)) {
        SCREENMAN->SystemMessage(sMessage);
      }
    }

    SCREENMAN->Draw();
  }

  // If we ended mid-game, finish up.
  GAMESTATE->SaveLocalData();

  if (ChangeAppPriority()) {
    HOOKS->UnBoostPriority();
  }
=======
		UpdateAllButDraw(false);
		
		CallEveryNFrames(500, CheckInputDevices);
		
		SCREENMAN->Draw();
	}

void GameLoop::UpdateAllButDraw(bool bRunningFromVBLANK) {
  // Flag to indicate whether an update has been processed during the VBLANK
  // period.
  static bool m_bUpdatedDuringVBLANK = false;

	if( ChangeAppPri() )
		HOOKS->UnBoostPriority();
>>>>>>> release
}

class ConcurrentRenderer {
 public:
  ConcurrentRenderer();
  ~ConcurrentRenderer();

  void Start();
  void Stop();

 private:
  RageThread thread_;
  RageEvent event_;
  bool shutdown_;
  void RenderThread();
  static int StartRenderThread(void* p);

=======
  // If we're running from VBLANK, and we've already updated during the VBLANK
  // period, don't update again. This is to prevent multiple updates during the
  // same VBLANK period.
  if (!bRunningFromVBLANK && m_bUpdatedDuringVBLANK) {
    m_bUpdatedDuringVBLANK = false;
    return;
  }

  // If we're running from VBLANK, indicate we've updated during the VBLANK
  // period. Otherwise, make sure the flag is cleared.
  if (bRunningFromVBLANK) {
    m_bUpdatedDuringVBLANK = true;
  } else {
    m_bUpdatedDuringVBLANK = false;
  }

  // If the constant update delta is set, use that value. Otherwise, use the
  // delta time from the gameplay timer.
  float fDeltaTime = (g_fConstantUpdateDeltaSeconds > 0)
                         ? g_fConstantUpdateDeltaSeconds
                         : g_GameplayTimer.GetDeltaTime();

  // Use a static boolean to check the preference once per game launch.
  // This is a rarely used debug feature, so we try to skip it if possible.
  static bool bLogSkips = PREFSMAN->m_bLogSkips;
  if (bLogSkips) {
    CheckGameLoopTimerSkips(fDeltaTime);
  }

  fDeltaTime *= g_fUpdateRate;

  // Update SOUNDMAN early (before any RageSound::GetPosition calls), to flush
  // position data.
  SOUNDMAN->Update();

  /* Update song beat information -before- calling update on all the classes
   * that depend on it. If you don't do this first, the classes are all acting
   * on old information and will lag. (but no longer fatally, due to
   * timestamping -glenn) */
  SOUND->Update(fDeltaTime);
  TEXTUREMAN->Update(fDeltaTime);
  GAMESTATE->Update(fDeltaTime);
  NETWORK->Update();
  SCREENMAN->Update(fDeltaTime);
  MEMCARDMAN->Update();

  /* Important: Process input AFTER updating game logic, or input will be
   * acting on song beat from last frame */
  HandleInputEvents(fDeltaTime);

  // Update the lights
  LIGHTSMAN->Update(fDeltaTime);
}

void GameLoop::RunGameLoop() {
  /* People may want to do something else while songs are loading, so do
   * this after loading songs. */
  if (ChangeAppPri()) {
    HOOKS->BoostPriority();
  }

  while (!ArchHooks::UserQuit()) {
    if (!g_NewGame.empty()) {
      DoChangeGame();
    }
    if (!g_NewTheme.empty()) {
      DoChangeTheme();
    }

    CheckFocus();

    UpdateAllButDraw(false);

    CallEveryNFrames(500, CheckInputDevices);

    SCREENMAN->Draw();
  }

  // If we ended mid-game, finish up.
  GAMESTATE->SaveLocalData();

  if (ChangeAppPri()) {
    HOOKS->UnBoostPriority();
  }
}

class ConcurrentRenderer {
 public:
  ConcurrentRenderer();
  ~ConcurrentRenderer();

  void Start();
  void Stop();

 private:
  RageThread m_Thread;
  RageEvent m_Event;
  bool m_bShutdown;
  void RenderThread();
  static int StartRenderThread(void* p);

>>>>>>> upstream/release
  enum State {
    RENDERING_IDLE,
    RENDERING_START,
    RENDERING_ACTIVE,
    RENDERING_END
  };
  State state_;
};
static ConcurrentRenderer* g_pConcurrentRenderer = nullptr;

ConcurrentRenderer::ConcurrentRenderer() : event_("ConcurrentRenderer") {
  shutdown_ = false;
  state_ = RENDERING_IDLE;

  thread_.SetName("ConcurrentRenderer");
  thread_.Create(StartRenderThread, this);
}

ConcurrentRenderer::~ConcurrentRenderer() {
  ASSERT(state_ == RENDERING_IDLE);
  shutdown_ = true;
  thread_.Wait();
}

void ConcurrentRenderer::Start() {
  DISPLAY->BeginConcurrentRenderingMainThread();

  event_.Lock();
  ASSERT(state_ == RENDERING_IDLE);
  state_ = RENDERING_START;
  event_.Signal();
  while (state_ != RENDERING_ACTIVE) {
    event_.Wait();
  }
  event_.Unlock();
}

void ConcurrentRenderer::Stop() {
  event_.Lock();
  ASSERT(state_ == RENDERING_ACTIVE);
  state_ = RENDERING_END;
  event_.Signal();
  while (state_ != RENDERING_IDLE) {
    event_.Wait();
  }
  event_.Unlock();

  DISPLAY->EndConcurrentRenderingMainThread();
}

void ConcurrentRenderer::RenderThread() {
  ASSERT(SCREENMAN != nullptr);

  while (!shutdown_) {
    event_.Lock();
    while (state_ == RENDERING_IDLE && !shutdown_) {
      event_.Wait();
    }
    event_.Unlock();

    if (state_ == RENDERING_START) {
      // We're starting to render. Set up, and then kick the event to wake up
			// the calling thread.
      DISPLAY->BeginConcurrentRendering();
      HOOKS->SetupConcurrentRenderingThread();

      LOG->Trace("ConcurrentRenderer::RenderThread start");

      event_.Lock();
      state_ = RENDERING_ACTIVE;
      event_.Signal();
      event_.Unlock();
    }

    // This is started during Update(). The next thing the game loop will do is
		// Draw, so shift operations around to put Draw at the top. This makes sure
		// updates are seamless.
    if (state_ == RENDERING_ACTIVE) {
      SCREENMAN->Draw();

      float fDeltaTime = g_GameplayTimer.GetDeltaTime();
      SCREENMAN->Update(fDeltaTime);
    }

    if (state_ == RENDERING_END) {
      LOG->Trace("ConcurrentRenderer::RenderThread done");

      DISPLAY->EndConcurrentRendering();

      event_.Lock();
      state_ = RENDERING_IDLE;
      event_.Signal();
      event_.Unlock();
    }
  }
}

int ConcurrentRenderer::StartRenderThread(void* p) {
  ((ConcurrentRenderer*)p)->RenderThread();
  return 0;
}

void GameLoop::StartConcurrentRendering() {
  if (g_pConcurrentRenderer == nullptr) {
    g_pConcurrentRenderer = new ConcurrentRenderer;
  }
  g_pConcurrentRenderer->Start();
}

void GameLoop::FinishConcurrentRendering() { g_pConcurrentRenderer->Stop(); }

/*
 * (c) 2001-2005 Chris Danford, Glenn Maynard
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
