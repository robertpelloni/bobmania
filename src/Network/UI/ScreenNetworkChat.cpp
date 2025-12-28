#include "global.h"
#include "ScreenNetworkChat.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "InputEventPlus.h"
#include "RageUtil.h"
#include "RageInput.h"
#include "Network/GameClient.h"

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

	// Connect Client
	GameClient::Instance()->Connect("simulated-server", 9000);
	GameClient::Instance()->SetMessageCallback( [this](const std::string& msg) {
		this->m_Messages.push_back(msg);
	});
}

void ScreenNetworkChat::Update( float fDeltaTime )
{
	ScreenWithMenuElements::Update( fDeltaTime );
	GameClient::Instance()->Update( fDeltaTime );

	// Render Chat
	std::string sDisplay;
	int start = max(0, (int)m_Messages.size() - 10);
	for( int i=start; i<(int)m_Messages.size(); ++i )
		sDisplay += m_Messages[i] + "\n";

	m_textChatBox.SetText( sDisplay );

	// Render Input
	m_textInput.SetText( "> " + m_sInputBuffer + "_" );
}

bool ScreenNetworkChat::Input( const InputEventPlus &input )
{
	if( input.type == IET_FIRST_PRESS )
	{
		// Mock Typing
		wchar_t c = INPUTMAN->DeviceInputToChar(input.DeviceI, true);
		if( c >= 32 && c <= 126 )
		{
			m_sInputBuffer += (char)c;
		}
		else if( input.DeviceI.button == KEY_ENTER || input.DeviceI.button == KEY_KP_ENTER )
		{
			if( !m_sInputBuffer.empty() )
			{
				GameClient::Instance()->SendChat( m_sInputBuffer );
				m_sInputBuffer = "";
			}
		}
		else if( input.MenuI == GAME_BUTTON_BACK )
		{
			SCREENMAN->SetNewScreen( "ScreenUnifiedDashboard" );
		}
	}
	return true;
}
