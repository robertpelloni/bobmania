#include "HeartRateDriver_Mock.h"
#include "GameState.h"
#include "Song.h"
#include <math.h>

HeartRateDriver_Mock::HeartRateDriver_Mock() : m_fTimeElapsed(0.0f) {}

HeartRateDriver_Mock::~HeartRateDriver_Mock() {}

bool HeartRateDriver_Mock::Init() {
    return true; // Always succeeds
}

void HeartRateDriver_Mock::Shutdown() {
    // Nothing to clean up
}

int HeartRateDriver_Mock::GetHeartRate() {
    // Return a sine wave simulating 80-140 BPM, but influenced by song BPM
    m_fTimeElapsed += 0.016f; // Rough approximation of 60fps

    int baseBPM = 100;
    int amplitude = 20;

    if (GAMESTATE && GAMESTATE->m_pCurSong) {
        float songBPM = GAMESTATE->m_pCurSong->GetBPMAtBeat(GAMESTATE->m_Position.m_fSongBeat);
        if (songBPM > 160) {
            baseBPM = 130;
            amplitude = 40;
        } else if (songBPM > 120) {
            baseBPM = 115;
            amplitude = 30;
        }
    }

    return baseBPM + (int)(sin(m_fTimeElapsed * 0.5f) * amplitude);
}

bool HeartRateDriver_Mock::IsConnected() {
    return true;
}
