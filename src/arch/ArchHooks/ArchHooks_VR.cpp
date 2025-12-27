#include "global.h"
#include "ArchHooks_VR.h"
#include "PrefsManager.h"
#include "RageLog.h"

ArchHooks_VR::ArchHooks_VR()
{
	Init();
}

ArchHooks_VR::~ArchHooks_VR()
{
}

void ArchHooks_VR::Init()
{
	// Initialize VR subsystem (stub)
	LOG->Trace("ArchHooks_VR::Init()");
}

void ArchHooks_VR::DumpDebugInfo()
{
	ArchHooks::DumpDebugInfo();
	LOG->Info("VR Mode: %s", IsVRModeEnabled() ? "Enabled" : "Disabled");
}

bool ArchHooks_VR::IsVRModeEnabled() const
{
	return PREFSMAN->m_bVRMode;
}

void ArchHooks_VR::EnterVRMode()
{
	LOG->Trace("Entering VR Mode (Stub)");
}

void ArchHooks_VR::ExitVRMode()
{
	LOG->Trace("Exiting VR Mode (Stub)");
}
