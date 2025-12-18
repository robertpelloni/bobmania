#include "global.h"
#include "ScreenNetworkChat.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "InputEventPlus.h"
#include "RageUtil.h"

REGISTER_SCREEN_CLASS( ScreenNetworkChat );

void ScreenNetworkChat::Init()
{
	ScreenWithMenuElements::Init();

	m_Messages.push_back("SYSTEM: Welcome to the Lobby.");
	m_Messages.push_back("DDR_Fan_99: Anyone up for a wager?");
	m_Messages.push_back("ProStep: Just hit 99% on Max 300!");

	m_textChatBox.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textChatBox.SetXY( 100, 100 );
	m_textChatBox.SetHorizAlign( align_left );
	m_textChatBox.SetZoom( 0.6f );
	this->AddChild( &m_textChatBox );

	m_textInput.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textInput.SetXY( 100, 400 );
	m_textInput.SetHorizAlign( align_left );
	m_textInput.SetDiffuse( RageColor(0,1,0,1) );
	this->AddChild( &m_textInput );

	m_sInputBuffer = "";
	m_fAutoChatTimer = 0;
}

void ScreenNetworkChat::Update( float fDeltaTime )
{
	ScreenWithMenuElements::Update( fDeltaTime );

	// Mock incoming messages
	m_fAutoChatTimer += fDeltaTime;
	if( m_fAutoChatTimer > 5.0f )
	{
		m_fAutoChatTimer = 0;
		if( RandomInt(2) == 0 )
			m_Messages.push_back("RhythmGamer: Looking for group...");
		else
			m_Messages.push_back("System: New Tournament starting in 5 mins.");
	}

	// Render Chat
	std::string sDisplay;
	int start = max(0, (int)m_Messages.size() - 10);
	for( int i=start; i<(int)m_Messages.size(); ++i )
		sDisplay += m_Messages[i] + "\n";

	m_textChatBox.SetText( sDisplay );

	// Render Input
	m_textInput.SetText( "> " + m_sInputBuffer + "_" );
}

void ScreenNetworkChat::Input( const InputEventPlus &input )
{
	if( input.type == IET_FIRST_PRESS )
	{
		// Mock Typing
		char c = input.DeviceI.ToChar();
		if( c >= 32 && c <= 126 )
		{
			m_sInputBuffer += c;
		}
		else if( input.DeviceI.button == KEY_ENTER || input.DeviceI.button == KEY_KP_ENTER )
		{
			if( !m_sInputBuffer.empty() )
			{
				m_Messages.push_back( "YOU: " + m_sInputBuffer );
				m_sInputBuffer = "";
			}
		}
		else if( input.MenuI == GAME_BUTTON_BACK )
		{
			SCREENMAN->SetNewScreen( "ScreenUnifiedDashboard" );
		}
	}
}
