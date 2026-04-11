#ifndef ARCH_HOOKS_VR_H
#define ARCH_HOOKS_VR_H

#include "ArchHooks.h"

class ArchHooks_VR : public ArchHooks
{
public:
	ArchHooks_VR();
	virtual ~ArchHooks_VR();

	void Init();
	void DumpDebugInfo();

	// VR Specific
	bool IsVRModeEnabled() const;
	void EnterVRMode();
	void ExitVRMode();

	// Render Loop
	void BeginFrame();
	void EndFrame();

	// Matrices (Mock Matrix4x4 for now, usually RageMatrix)
	void GetViewMatrix( int eye, float* outMatrix );
	void GetProjectionMatrix( int eye, float* outMatrix );

	// Input
	void UpdateHMD();
};

#endif
