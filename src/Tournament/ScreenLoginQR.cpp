#include "global.h"
#include "ScreenLoginQR.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "RageLog.h"
#include "InputEventPlus.h"
#include "ProfileManager.h"
#include "EconomyManager.h"
#include "GameSoundManager.h"

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

		// Barcode scanners often behave like keyboards sending characters followed by a Return/Enter.
		// We capture printable characters into a buffer and flush on Enter.

		char c = input.DeviceI.ToChar();
		if( c >= 32 && c <= 126 ) // Printable ASCII
		{
			m_sBuffer += c;
			m_textStatus.SetText( "Reading: " + std::string(m_sBuffer.length(), '*') ); // Masked for privacy
		}
		else if( input.DeviceI.button == KEY_ENTER || input.DeviceI.button == KEY_KP_ENTER )
		{
			// End of scan sequence
			LOG->Trace("QR Login Scanned: %s", m_sBuffer.c_str());

			bool bValid = false;

			// Check for valid prefixes (SMX, DDR) or any non-empty string for testing
			if ( m_sBuffer.find("SMX:") == 0 || m_sBuffer.find("DDR:") == 0 )
			{
				bValid = true;
			}
			else if ( !m_sBuffer.empty() )
			{
				// Allow generic IDs for flexibility in this test phase
				bValid = true;
			}

			if( bValid )
			{
				m_textStatus.SetText( "Authenticating..." );

				// 1. Load Profile (Simulated)
				// PROFILEMAN->LoadProfileFromID( m_sBuffer );

				// 2. Register/Login to Economy
				EconomyManager::Instance()->RegisterUser( m_sBuffer );

				SOUND->PlayOnce( THEME->GetPathS("Common", "start") );
				SCREENMAN->SetNewScreen( "ScreenUnifiedDashboard" ); // Jump to Dashboard
			}
			else
			{
				m_textStatus.SetText( "Invalid Code. Try Again." );
				SOUND->PlayOnce( THEME->GetPathS("Common", "cancel") );
				m_sBuffer = "";
			}
		}
	}
}
