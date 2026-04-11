<<<<<<< HEAD:itgmania/src/arch/Lights/LightsDriver_PacDrive.cpp
#include "global.h"
#include "RageLog.h"
#include "LightsDriver_PacDrive.h"
#include "GameState.h"
#include "Game.h"

REGISTER_LIGHTS_DRIVER_CLASS(PacDrive);

static Preference<RString> g_sPacDriveLightOrdering("PacDriveLightOrdering", "openitg");
int iPacDriveLightOrder = 0;

LightsDriver_PacDrive::LightsDriver_PacDrive() : dev{PACDRIVE_VID, make_pids(PACDRIVE_PID, PACDRIVE_PID_MAX), PACDRIVE_INTERFACE}
{
	prev_led_state.raw = 0;
	memset(state.raw_state, 0x00, sizeof(state.raw_state));

	RString lightOrder = g_sPacDriveLightOrdering.Get();
	if (lightOrder.CompareNoCase("lumenar") == 0 || lightOrder.CompareNoCase("openitg") == 0)
	{
		iPacDriveLightOrder = 1;
=======
// LightsDriver_PacDrive for use with a PacDrive hooked up with LEDs
// You need PacDrive32.dll in the StepMania directory to use this.

#include "global.h"
#include "LightsDriver_PacDrive.h"
#include "windows.h"
#include "RageUtil.h"
#include "Preference.h"

REGISTER_LIGHTS_DRIVER_CLASS(PacDrive);

HINSTANCE PachDLL = nullptr;

bool PacDriveConnected = false;
typedef int (WINAPI PacInitialize)(void);
PacInitialize* m_pacinit = nullptr;
typedef void (WINAPI PacShutdown)(void);
PacShutdown* m_pacdone = nullptr;
typedef bool (WINAPI PacSetLEDStates)(int, short int);
PacSetLEDStates* m_pacset = nullptr;
int iLightingOrder = 0;

//Adds new preference to allow for different light wiring setups
static Preference<RString> g_sPacDriveLightOrdering("PacDriveLightOrdering", "minimaid");


LightsDriver_PacDrive::LightsDriver_PacDrive()
{
	// init io.dll
	PachDLL = LoadLibrary("pacdrive32.dll");
	if(PachDLL == nullptr)
	{
		MessageBox(nullptr, "Could not LoadLibrary( pacdrive32.dll ).", "ERROR", MB_OK );
		return;
	}

	//Get the function pointers
	m_pacinit = (PacInitialize*)GetProcAddress(PachDLL, "PacInitialize");
	m_pacset = (PacSetLEDStates*)GetProcAddress(PachDLL, "PacSetLEDStates");
	m_pacdone = (PacShutdown*)GetProcAddress(PachDLL, "PacShutdown");

	int NumPacDrives = 0;

	if (m_pacinit)
		NumPacDrives = m_pacinit(); //initialize the pac drive

	if (NumPacDrives == 0)
	{
		PacDriveConnected = false; // set not connected
		MessageBox(nullptr, "Could not find connected PacDrive.", "ERROR", MB_OK);
		return;
	}
	else
	{
		PacDriveConnected = true; // set connected
		m_pacset(0, 0x0);  // clear all lights for device i
		RString lightOrder = g_sPacDriveLightOrdering.Get();
		if (lightOrder.CompareNoCase("lumenar") == 0 || lightOrder.CompareNoCase("openitg") == 0) {
			iLightingOrder = 1;
		}
>>>>>>> origin/unified-ui-features-13937230807013224518:src/arch/Lights/LightsDriver_PacDrive.cpp
	}
}

LightsDriver_PacDrive::~LightsDriver_PacDrive()
{
<<<<<<< HEAD:itgmania/src/arch/Lights/LightsDriver_PacDrive.cpp
=======
	if (PacDriveConnected && m_pacset)
		m_pacset(0, 0x0);  // clear all lights for device i

	if (m_pacdone)
		m_pacdone();

	FreeLibrary(PachDLL);
>>>>>>> origin/unified-ui-features-13937230807013224518:src/arch/Lights/LightsDriver_PacDrive.cpp
}

void LightsDriver_PacDrive::Set(const LightsState *ls)
{
<<<<<<< HEAD:itgmania/src/arch/Lights/LightsDriver_PacDrive.cpp
	if (!dev.FoundOnce())
		return;

	switch (iPacDriveLightOrder)
	{
	case 1:
		// Sets the cabinet light values to follow LumenAR/OpenITG wiring standards

		/*
		 * OpenITG PacDrive Order:
		 * Taken from LightsDriver_PacDrive::SetLightsMappings() in openitg.
		 * (index of 1 as the PacDrive labels them as index 1)
		 *
		 * 1: Marquee UL
		 * 2: Marquee UR
		 * 3: Marquee DL
		 * 4: Marquee DR
		 *
		 * 5: P1 Button
		 * 6: P2 Button
		 *
		 * 7: Bass Left
		 * 8: Bass Right
		 *
		 * 9,19,11,12: P1 L R U D
		 * 13,14,15,16: P2 L R U D
		 */

		state.leds.led01 = ls->m_bCabinetLights[LIGHT_MARQUEE_UP_LEFT];
		state.leds.led02 = ls->m_bCabinetLights[LIGHT_MARQUEE_UP_RIGHT];
		state.leds.led03 = ls->m_bCabinetLights[LIGHT_MARQUEE_LR_LEFT];
		state.leds.led04 = ls->m_bCabinetLights[LIGHT_MARQUEE_LR_RIGHT];

		state.leds.led05 = ls->m_bGameButtonLights[GameController_1][GAME_BUTTON_START];
		state.leds.led06 = ls->m_bGameButtonLights[GameController_2][GAME_BUTTON_START];

		state.leds.led07 = ls->m_bCabinetLights[LIGHT_BASS_LEFT] || ls->m_bCabinetLights[LIGHT_BASS_RIGHT];
		state.leds.led08 = ls->m_bCabinetLights[LIGHT_BASS_LEFT] || ls->m_bCabinetLights[LIGHT_BASS_RIGHT];

		state.leds.led09 = ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_LEFT];
		state.leds.led10 = ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_RIGHT];
		state.leds.led11 = ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_UP];
		state.leds.led12 = ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_DOWN];

		state.leds.led13 = ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_LEFT];
		state.leds.led14 = ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_RIGHT];
		state.leds.led15 = ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_UP];
		state.leds.led16 = ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_DOWN];
		break;

	case 0:
	default:
		// If all else fails, falls back to original order
		// reference page 7
		// http://www.peeweepower.com/stepmania/sm509pacdriveinfo.pdf

		state.leds.led01 = ls->m_bCabinetLights[LIGHT_MARQUEE_UP_LEFT];
		state.leds.led02 = ls->m_bCabinetLights[LIGHT_MARQUEE_UP_RIGHT];
		state.leds.led03 = ls->m_bCabinetLights[LIGHT_MARQUEE_LR_LEFT];
		state.leds.led04 = ls->m_bCabinetLights[LIGHT_MARQUEE_LR_RIGHT];

		state.leds.led05 = ls->m_bCabinetLights[LIGHT_BASS_LEFT] || ls->m_bCabinetLights[LIGHT_BASS_RIGHT];

		state.leds.led06 = ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_LEFT];
		state.leds.led07 = ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_RIGHT];
		state.leds.led08 = ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_UP];
		state.leds.led09 = ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_DOWN];
		state.leds.led10 = ls->m_bGameButtonLights[GameController_1][GAME_BUTTON_START];

		state.leds.led11 = ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_LEFT];
		state.leds.led12 = ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_RIGHT];
		state.leds.led13 = ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_UP];
		state.leds.led14 = ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_DOWN];
		state.leds.led15 = ls->m_bGameButtonLights[GameController_2][GAME_BUTTON_START];

		// led16 is not used.
		state.leds.led16 = false;
		break;
	}

	// only push on changes.
	if (state.leds.raw != prev_led_state.raw)
	{
		state.report_id = PACDRIVE_HIDREPORT_ID;
		state.pad0 = 0;
		state.pad1 = 0;

		dev.Write((unsigned char *)&state.raw_state, sizeof(state.raw_state));
		prev_led_state = state.leds;
	}
}
=======
	short int outb = 0;
	switch (iLightingOrder) {
	case 1:
		//Sets the cabinet light values to follow LumenAR/OpenITG wiring standards

		if (ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_LEFT]) outb |= BIT(0);
		if (ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_RIGHT]) outb |= BIT(1);
		if (ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_UP]) outb |= BIT(2);
		if (ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_DOWN]) outb |= BIT(3);
		if (ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_LEFT]) outb |= BIT(4);
		if (ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_RIGHT]) outb |= BIT(5);
		if (ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_UP]) outb |= BIT(6);
		if (ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_DOWN]) outb |= BIT(7);
		if (ls->m_bCabinetLights[LIGHT_MARQUEE_UP_LEFT]) outb |= BIT(8);
		if (ls->m_bCabinetLights[LIGHT_MARQUEE_UP_RIGHT]) outb |= BIT(9);
		if (ls->m_bCabinetLights[LIGHT_MARQUEE_LR_LEFT]) outb |= BIT(10);
		if (ls->m_bCabinetLights[LIGHT_MARQUEE_LR_RIGHT]) outb |= BIT(11);
		if (ls->m_bGameButtonLights[GameController_1][GAME_BUTTON_START]) outb |= BIT(12);
		if (ls->m_bGameButtonLights[GameController_2][GAME_BUTTON_START]) outb |= BIT(13);
		if (ls->m_bCabinetLights[LIGHT_BASS_LEFT] || ls->m_bCabinetLights[LIGHT_BASS_RIGHT]) outb |= BIT(14);
		break;
	case 0:
	default:
		//If all else fails, falls back to Minimaid order

		if (ls->m_bCabinetLights[LIGHT_MARQUEE_UP_LEFT]) outb |= BIT(0);
		if (ls->m_bCabinetLights[LIGHT_MARQUEE_UP_RIGHT]) outb |= BIT(1);
		if (ls->m_bCabinetLights[LIGHT_MARQUEE_LR_LEFT]) outb |= BIT(2);
		if (ls->m_bCabinetLights[LIGHT_MARQUEE_LR_RIGHT]) outb |= BIT(3);
		if (ls->m_bCabinetLights[LIGHT_BASS_LEFT] || ls->m_bCabinetLights[LIGHT_BASS_RIGHT]) outb |= BIT(4);
		if (ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_LEFT]) outb |= BIT(5);
		if (ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_RIGHT]) outb |= BIT(6);
		if (ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_UP]) outb |= BIT(7);
		if (ls->m_bGameButtonLights[GameController_1][DANCE_BUTTON_DOWN]) outb |= BIT(8);
		if (ls->m_bGameButtonLights[GameController_1][GAME_BUTTON_START]) outb |= BIT(9);
		if (ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_LEFT]) outb |= BIT(10);
		if (ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_RIGHT]) outb |= BIT(11);
		if (ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_UP]) outb |= BIT(12);
		if (ls->m_bGameButtonLights[GameController_2][DANCE_BUTTON_DOWN]) outb |= BIT(13);
		if (ls->m_bGameButtonLights[GameController_2][GAME_BUTTON_START]) outb |= BIT(14);
		break;
	}

	//ensure m_pacset function call was loaded.
	if (m_pacset)
		m_pacset(0, outb);
}

/* Modified 2015 Dave Barribeau for StepMania 5.09
* (c) 2003-2004 Chris Danford
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
>>>>>>> origin/unified-ui-features-13937230807013224518:src/arch/Lights/LightsDriver_PacDrive.cpp
