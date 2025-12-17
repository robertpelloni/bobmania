#include "global.h"
#include "ScreenLoginQR.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "RageLog.h"
#include "InputEventPlus.h"
#include "ProfileManager.h"
#include "EconomyManager.h"

REGISTER_SCREEN_CLASS( ScreenLoginQR );

void ScreenLoginQR::Init()
{
	ScreenWithMenuElements::Init();

	m_textPrompt.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textPrompt.SetText( "Scan QR Code or Card" );
	m_textPrompt.SetXY( 320, 200 );
	this->AddChild( &m_textPrompt );

	m_textStatus.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textStatus.SetText( "Waiting for input..." );
	m_textStatus.SetXY( 320, 300 );
	m_textStatus.SetDiffuse( RageColor(0.5f, 0.5f, 0.5f, 1) );
	this->AddChild( &m_textStatus );

	m_sBuffer = "";
}

void ScreenLoginQR::Input( const InputEventPlus &input )
{
	// Simulate Barcode Scanner Input (which usually sends Keystrokes ending in Enter)
	if( input.type == IET_FIRST_PRESS )
	{
		if( input.MenuI == GAME_BUTTON_START || input.MenuI == GAME_BUTTON_BACK )
		{
			// Allow standard navigation if user aborts
			ScreenWithMenuElements::Input( input );
			return;
		}

		// In a real scanner implementation, we would read input.DeviceI.button character mapping
		// For this MVP, we simulate that typing "S" triggers a simulated login for testing convenience
		// or if we had a keyboard attached, we'd accumulate chars.

		// Mock logic: If we detect a specific "Scan" signal (mapped to a key for now)
		// Let's assume the user is typing on a keyboard for the simulation.

		char c = input.DeviceI.ToChar();
		if( c >= 32 && c <= 126 ) // Printable char
		{
			m_sBuffer += c;
			m_textStatus.SetText( "Reading: " + std::string(m_sBuffer.length(), '*') ); // Masked
		}
		else if( input.DeviceI.button == KEY_ENTER || input.DeviceI.button == KEY_KP_ENTER )
		{
			// Submit
			LOG->Trace("QR Login Scanned: %s", m_sBuffer.c_str());

			// Validate
			if( m_sBuffer.find("SMX:") == 0 || m_sBuffer.find("DDR:") == 0 || m_sBuffer.length() > 0 )
			{
				m_textStatus.SetText( "Authenticating..." );

				// Simulate successful login
				// PROFILEMAN->LoadProfileFromID( ... ) logic would go here

				// Register with Economy
				EconomyManager::Instance()->RegisterUser( m_sBuffer );

				SCREENMAN->SetNewScreen( "ScreenSelectProfile" ); // Or skip directly to Title
			}
			else
			{
				m_textStatus.SetText( "Invalid Code. Try Again." );
				m_sBuffer = "";
			}
		}
	}
}
