#ifndef ARCH_HOOKS_VR_H
#define ARCH_HOOKS_VR_H

#include "global.h"

// Mocks the OpenVR or Meta Quest SDK interfaces
class ArchHooks_VR
{
public:
    ArchHooks_VR();
    ~ArchHooks_VR();

    static void Init();
    static void Shutdown();

    // Stereoscopic Matrix retrieval (Returns 4x4 float array)
    static void GetEyeMatrix(int iEye, float* pMatrixOut);

    // Hooks for the render loop
    static void BeginFrame();
    static void EndFrame();

    static bool IsVRAvailable();
};

#endif
