#include "global.h"
#include "ScreenGovernance.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "Economy/EconomyManager.h"
#include "InputEventPlus.h"
#include "GameSoundManager.h"

REGISTER_SCREEN_CLASS( ScreenGovernance );

void ScreenGovernance::Init()
{
	ScreenWithMenuElements::Init();

	// Mock Data
	m_Proposals.push_back( { 1, "Add Song: Butterfly", "License fee: 5000 Coins", 120, 10 } );
	m_Proposals.push_back( { 2, "Increase Tournament Fee", "Change from 5% to 7%", 45, 90 } );
	m_Proposals.push_back( { 3, "New Gym Feature", "Fund dev of Calorie Course", 300, 5 } );
	m_iCurrentPropIndex = 0;

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "COMPANY GOVERNANCE (DAO)" );
	m_textTitle.SetXY( 320, 40 );
	this->AddChild( &m_textTitle );

	m_textTreasury.LoadFromFont( THEME->GetPathF("Common", "normal") );
	long long balance = EconomyManager::Instance()->GetBalance("WALLET_DAO");
	m_textTreasury.SetText( ssprintf("Treasury: %lld Coins", balance) );
	m_textTreasury.SetXY( 320, 70 );
	m_textTreasury.SetDiffuse( RageColor(1,0.8f,0,1) ); // Gold
	this->AddChild( &m_textTreasury );

	// Proposal UI
	m_textPropTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textPropTitle.SetZoom( 0.8f );
	m_textPropTitle.SetXY( 320, 150 );
	this->AddChild( &m_textPropTitle );

	m_textPropDesc.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textPropDesc.SetXY( 320, 190 );
	this->AddChild( &m_textPropDesc );

	m_textVotes.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textVotes.SetXY( 320, 230 );
	this->AddChild( &m_textVotes );

	UpdateUI();
}

void ScreenGovernance::UpdateUI()
{
	if( m_Proposals.empty() ) return;

	const Proposal& p = m_Proposals[m_iCurrentPropIndex];
	m_textPropTitle.SetText( ssprintf("Prop #%d: %s", p.id, p.title.c_str()) );
	m_textPropDesc.SetText( p.desc );
	m_textVotes.SetText( ssprintf("YES: %d   NO: %d", p.yesVotes, p.noVotes) );
}

void ScreenGovernance::Vote( bool bYes )
{
	if( m_Proposals.empty() ) return;

	// Vote weight is determined by share ownership
	// Minimum 1 vote for citizenship
	int shares = EconomyManager::Instance()->GetShareCount();
	int weight = (shares > 0) ? shares : 1;

	if( bYes ) m_Proposals[m_iCurrentPropIndex].yesVotes += weight;
	else       m_Proposals[m_iCurrentPropIndex].noVotes += weight;

	SOUND->PlayOnce( THEME->GetPathS("Common", "start") );
	UpdateUI();
}

void ScreenGovernance::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	if( input.MenuI == GAME_BUTTON_LEFT )
	{
		// Previous Prop
		if( m_iCurrentPropIndex > 0 ) m_iCurrentPropIndex--;
		UpdateUI();
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
	}
	else if( input.MenuI == GAME_BUTTON_RIGHT )
	{
		// Next Prop
		if( m_iCurrentPropIndex < (int)m_Proposals.size() - 1 ) m_iCurrentPropIndex++;
		UpdateUI();
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
	}
	else if( input.MenuI == GAME_BUTTON_START )
	{
		// Vote Yes
		Vote( true );
	}
	else if( input.MenuI == GAME_BUTTON_SELECT )
	{
		// Vote No
		Vote( false );
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
	}
}
