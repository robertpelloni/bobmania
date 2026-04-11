#include "global.h"
#include "ScreenBettingEntry.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "EconomyManager.h"
#include "InputEventPlus.h"
#include "GameSoundManager.h"

REGISTER_SCREEN_CLASS( ScreenBettingEntry );

void ScreenBettingEntry::Init()
{
	ScreenWithMenuElements::Init();

	// Options
	m_WagerOptions = { 10, 50, 100, 500, 1000, 5000 };
	m_iSelection = 1; // Default to 50

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "WAGER BATTLE SETUP" );
	m_textTitle.SetXY( 320, 40 );
	m_textTitle.SetDiffuse( RageColor(1,0,0,1) ); // Red
	this->AddChild( &m_textTitle );

	m_textBalance.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textBalance.SetXY( 320, 70 );
	m_textBalance.SetDiffuse( RageColor(0,1,0,1) );
	this->AddChild( &m_textBalance );

	m_textInstructions.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textInstructions.SetText( "Select Wager Amount\n(Beat Grade 'A' to Win 2x)" );
	m_textInstructions.SetXY( 320, 400 );
	m_textInstructions.SetZoom( 0.8f );
	this->AddChild( &m_textInstructions );

	// Generate Rows
	float startY = 150;
	for( size_t i=0; i < m_WagerOptions.size(); ++i )
	{
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "header") );
		pText->SetText( ssprintf("%lld Coins", m_WagerOptions[i]) );
		pText->SetXY( 320, startY + (i * 40) );
		this->AddChild( pText );
		m_pOptionTexts.push_back( pText );
	}

	RefreshView();
}

void ScreenBettingEntry::RefreshView()
{
	long long balance = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
	m_textBalance.SetText( ssprintf("Your Balance: %lld", balance) );

	for( size_t i=0; i < m_pOptionTexts.size(); ++i )
	{
		if( (int)i == m_iSelection )
		{
			m_pOptionTexts[i]->SetDiffuse( RageColor(1,1,0,1) );
			m_pOptionTexts[i]->SetZoom( 1.0f );
		}
		else
		{
			m_pOptionTexts[i]->SetDiffuse( RageColor(1,1,1,0.5f) );
			m_pOptionTexts[i]->SetZoom( 0.8f );
		}
	}
}

void ScreenBettingEntry::PlaceBet( long long amount )
{
	long long balance = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
	if( balance < amount )
	{
		SOUND->PlayOnce( THEME->GetPathS("Common", "cancel") );
		m_textInstructions.SetText( "Insufficient Funds!" );
		m_textInstructions.SetDiffuse( RageColor(1,0,0,1) );
		return;
	}

	EconomyManager::Instance()->StartMatchBet( amount );
	SOUND->PlayOnce( THEME->GetPathS("Common", "start") );

	// Proceed to Music Select
	SCREENMAN->SetNewScreen( "ScreenSelectMusic" );
}

void ScreenBettingEntry::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	if( input.MenuI == GAME_BUTTON_UP )
	{
		if( m_iSelection > 0 ) m_iSelection--;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
		RefreshView();
	}
	else if( input.MenuI == GAME_BUTTON_DOWN )
	{
		if( m_iSelection < (int)m_WagerOptions.size() - 1 ) m_iSelection++;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
		RefreshView();
	}
	else if( input.MenuI == GAME_BUTTON_START )
	{
		PlaceBet( m_WagerOptions[m_iSelection] );
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenUnifiedDashboard" );
	}
}
