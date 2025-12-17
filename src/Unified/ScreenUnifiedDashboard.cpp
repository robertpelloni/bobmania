#include "global.h"
#include "ScreenUnifiedDashboard.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "InputEventPlus.h"
#include "EconomyManager.h"
#include "ContentSwarmManager.h"
#include "RageUtil.h"
#include "GameSoundManager.h"

REGISTER_SCREEN_CLASS( ScreenUnifiedDashboard );

void ScreenUnifiedDashboard::Init()
{
	ScreenWithMenuElements::Init();

	// Setup Options
	m_Options.push_back( "ARCADE MODE" );
	m_Options.push_back( "GYM CIRCUIT" );
	m_Options.push_back( "TOURNAMENT LADDER" );
	m_Options.push_back( "CONTENT SWARM" );
	m_Options.push_back( "DAO GOVERNANCE" );
	m_Options.push_back( "SERVER NODE (IDLE)" );
	m_iSelection = 0;

	// Header
	m_textHeader.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textHeader.SetText( "STEPMANIA NETWORK OS" );
	m_textHeader.SetXY( 320, 30 );
	m_textHeader.SetDiffuse( RageColor(0.8f, 0.9f, 1.0f, 1) );
	this->AddChild( &m_textHeader );

	// Widgets Area
	float widgetY = 80;

	m_textWalletWidget.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textWalletWidget.SetXY( 100, widgetY );
	m_textWalletWidget.SetZoom( 0.6f );
	m_textWalletWidget.SetHorizAlign( align_left );
	this->AddChild( &m_textWalletWidget );

	m_textRankWidget.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textRankWidget.SetXY( 300, widgetY );
	m_textRankWidget.SetZoom( 0.6f );
	m_textRankWidget.SetHorizAlign( align_left );
	this->AddChild( &m_textRankWidget );

	m_textSwarmWidget.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textSwarmWidget.SetXY( 500, widgetY );
	m_textSwarmWidget.SetZoom( 0.6f );
	m_textSwarmWidget.SetHorizAlign( align_left );
	this->AddChild( &m_textSwarmWidget );

	// Menu Options
	float menuY = 160;
	for( size_t i=0; i < m_Options.size(); ++i )
	{
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "header") );
		pText->SetText( m_Options[i] );
		pText->SetXY( 320, menuY + (i * 45) );
		pText->SetZoom( 0.8f );
		this->AddChild( pText );
		m_pOptionTexts.push_back( pText );
	}
}

void ScreenUnifiedDashboard::Update( float fDeltaTime )
{
	ScreenWithMenuElements::Update( fDeltaTime );

	// Refresh Widgets
	long long balance = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
	m_textWalletWidget.SetText( ssprintf("WALLET\n%lld Coins", balance) );

	m_textRankWidget.SetText( "RANK\nGold Div" ); // Mock

	int packs = ContentSwarmManager::Instance()->GetAvailablePacks().size();
	long long earnings = EconomyManager::Instance()->GetMiningReward();
	m_textSwarmWidget.SetText( ssprintf("NETWORK\n%d Peers\n+%lld Earned", packs, earnings) );

	// Selection Highlight
	for( size_t i=0; i < m_pOptionTexts.size(); ++i )
	{
		if( (int)i == m_iSelection )
		{
			m_pOptionTexts[i]->SetDiffuse( RageColor(1,1,0,1) ); // Yellow
			m_pOptionTexts[i]->SetZoom( 0.9f );
		}
		else
		{
			m_pOptionTexts[i]->SetDiffuse( RageColor(1,1,1,0.5f) ); // Dim
			m_pOptionTexts[i]->SetZoom( 0.8f );
		}
	}
}

void ScreenUnifiedDashboard::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	if( input.MenuI == GAME_BUTTON_UP )
	{
		if( m_iSelection > 0 ) m_iSelection--;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
	}
	else if( input.MenuI == GAME_BUTTON_DOWN )
	{
		if( m_iSelection < (int)m_Options.size() - 1 ) m_iSelection++;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
	}
	else if( input.MenuI == GAME_BUTTON_START )
	{
		SOUND->PlayOnce( THEME->GetPathS("Common", "start") );

		std::string sChoice = m_Options[m_iSelection];
		if( sChoice == "ARCADE MODE" ) SCREENMAN->SetNewScreen( "ScreenSelectMusic" );
		else if( sChoice == "GYM CIRCUIT" ) SCREENMAN->SetNewScreen( "ScreenGymWelcome" );
		else if( sChoice == "TOURNAMENT LADDER" ) SCREENMAN->SetNewScreen( "ScreenTournamentLadder" );
		else if( sChoice == "CONTENT SWARM" ) SCREENMAN->SetNewScreen( "ScreenContentNetwork" );
		else if( sChoice == "DAO GOVERNANCE" ) SCREENMAN->SetNewScreen( "ScreenGovernance" );
		else if( sChoice == "SERVER NODE (IDLE)" ) SCREENMAN->SetNewScreen( "ScreenServerNode" );
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
	}
}
