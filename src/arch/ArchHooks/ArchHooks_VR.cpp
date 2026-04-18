#include "ArchHooks_VR.h"
#include "RageLog.h"

static bool g_bVRAvailable = false;

ArchHooks_VR::ArchHooks_VR() {}
ArchHooks_VR::~ArchHooks_VR() {}

void ArchHooks_VR::Init() {
    LOG->Info("ArchHooks_VR::Init - Probing for OpenVR/Oculus runtimes...");
    // Mocking success
    g_bVRAvailable = true;
    LOG->Info("ArchHooks_VR::Init - VR Headset detected.");
}

void ArchHooks_VR::Shutdown() {
    if (g_bVRAvailable) {
        LOG->Info("ArchHooks_VR::Shutdown - Releasing VR runtime.");
        g_bVRAvailable = false;
    }
}

void ArchHooks_VR::GetEyeMatrix(int iEye, float* pMatrixOut) {
    // Identity matrix stub. In reality, OpenVR gives us the translation/rotation
    // of the physical headset to offset the in-game camera.
    for(int i=0; i<16; ++i) pMatrixOut[i] = 0.0f;
    pMatrixOut[0] = 1.0f; pMatrixOut[5] = 1.0f; pMatrixOut[10] = 1.0f; pMatrixOut[15] = 1.0f;

    // Offset X based on eye (0=Left, 1=Right) to simulate IPD (Inter-pupillary distance)
    if (iEye == 0) pMatrixOut[12] = -0.03f;
    else pMatrixOut[12] = 0.03f;
}

void ArchHooks_VR::BeginFrame() {
    // Tell the VR compositor we are starting a new frame rendering
}

void ArchHooks_VR::EndFrame() {
    // Submit the left/right eye textures to the VR compositor
}

bool ArchHooks_VR::IsVRAvailable() {
    return g_bVRAvailable;
}
