#include "global.h"
#include "ScreenGovernance.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "EconomyManager.h"
#include "InputEventPlus.h"
#include "GameSoundManager.h"

REGISTER_SCREEN_CLASS( ScreenGovernance );

void ScreenGovernance::Init()
{
	ScreenWithMenuElements::Init();

	// Proposals are now managed by EconomyManager
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
	const auto& proposals = EconomyManager::Instance()->GetProposals();
	if( proposals.empty() ) return;

	if( m_iCurrentPropIndex >= (int)proposals.size() ) m_iCurrentPropIndex = 0;

	const Proposal& p = proposals[m_iCurrentPropIndex];
	m_textPropTitle.SetText( ssprintf("Prop #%d: %s", p.id, p.title.c_str()) );
	m_textPropDesc.SetText( p.desc );
	m_textVotes.SetText( ssprintf("YES: %d   NO: %d", p.yesVotes, p.noVotes) );
}

void ScreenGovernance::Vote( bool bYes )
{
	const auto& proposals = EconomyManager::Instance()->GetProposals();
	if( proposals.empty() ) return;

	int propId = proposals[m_iCurrentPropIndex].id;
	EconomyManager::Instance()->VoteOnProposal(propId, bYes);

	SOUND->PlayOnce( THEME->GetPathS("Common", "start") );
	UpdateUI();
}

void ScreenGovernance::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	const auto& proposals = EconomyManager::Instance()->GetProposals();

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
		if( m_iCurrentPropIndex < (int)proposals.size() - 1 ) m_iCurrentPropIndex++;
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
