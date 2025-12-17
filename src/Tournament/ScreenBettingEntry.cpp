#include "global.h"
#include "ScreenBettingEntry.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "GameSoundManager.h"
#include "EconomyManager.h"
#include "InputEventPlus.h"

REGISTER_SCREEN_CLASS( ScreenBettingEntry );

void ScreenBettingEntry::Init()
{
	ScreenWithMenuElements::Init();

	m_iWager = 0;

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "PLACE YOUR BET" );
	m_textTitle.SetXY( 320, 60 );
	this->AddChild( &m_textTitle );

	m_textInstructions.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textInstructions.SetText( "Use Left/Right to adjust wager. Start to Confirm." );
	m_textInstructions.SetXY( 320, 100 );
	m_textInstructions.SetZoom( 0.8f );
	this->AddChild( &m_textInstructions );

	m_textWagerAmount.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textWagerAmount.SetText( "0 Coins" );
	m_textWagerAmount.SetXY( 320, 240 );
	m_textWagerAmount.SetDiffuse( RageColor(1,0.5f,0,1) ); // Orange
	this->AddChild( &m_textWagerAmount );

	m_textWalletBalance.LoadFromFont( THEME->GetPathF("Common", "normal") );
	// Initial update
	long long balance = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
	m_textWalletBalance.SetText( ssprintf("Your Balance: %lld Coins", balance) );
	m_textWalletBalance.SetXY( 320, 300 );
	this->AddChild( &m_textWalletBalance );
}

void ScreenBettingEntry::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	bool bUpdateText = false;

	if( input.MenuI == GAME_BUTTON_LEFT )
	{
		m_iWager -= 10;
		if( m_iWager < 0 ) m_iWager = 0;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
		bUpdateText = true;
	}
	else if( input.MenuI == GAME_BUTTON_RIGHT )
	{
		long long balance = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
		m_iWager += 10;
		if( m_iWager > balance ) m_iWager = balance;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
		bUpdateText = true;
	}
	else if( input.MenuI == GAME_BUTTON_START )
	{
		if( m_iWager > 0 )
		{
			EconomyManager::Instance()->StartMatchBet( m_iWager );
			SOUND->PlayOnce( THEME->GetPathS("Common", "start") );
		}
		// Proceed to gameplay regardless of bet (0 is valid)
		SCREENMAN->SetNewScreen( "ScreenSelectMusic" );
		return;
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
		return;
	}

	if (bUpdateText)
	{
		m_textWagerAmount.SetText( ssprintf("%lld Coins", m_iWager) );

		// Update balance text to show remaining
		long long balance = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
		long long remaining = balance - m_iWager;
		m_textWalletBalance.SetText( ssprintf("Remaining: %lld Coins", remaining) );
	}
}
