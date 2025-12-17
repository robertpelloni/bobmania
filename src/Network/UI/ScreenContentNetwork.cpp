#include "global.h"
#include "ScreenContentNetwork.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "RageLog.h"
#include "InputEventPlus.h"
#include "GameSoundManager.h"

REGISTER_SCREEN_CLASS( ScreenContentNetwork );

void ScreenContentNetwork::Init()
{
	ScreenWithMenuElements::Init();

	// Init Manager
	ContentSwarmManager::Instance()->Initialize();

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "CONTENT SWARM NETWORK" );
	m_textTitle.SetXY( 320, 40 );
	m_textTitle.SetDiffuse( RageColor(0,1,1,1) ); // Cyan
	this->AddChild( &m_textTitle );

	m_textEarnings.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textEarnings.SetXY( 320, 440 );
	this->AddChild( &m_textEarnings );

	m_iSelection = 0;
	RefreshList();
}

void ScreenContentNetwork::Update( float fDeltaTime )
{
	ScreenWithMenuElements::Update( fDeltaTime );
	ContentSwarmManager::Instance()->Update( fDeltaTime );

	long long earnings = ContentSwarmManager::Instance()->GetSessionBandwidthEarnings();
	m_textEarnings.SetText( ssprintf("Seeding Reward: %lld Tokens (Bandwidth Contributed)", earnings) );

	// Simple refresh to show download progress
	// In production, optimize this to only update progress text
	RefreshList();
}

void ScreenContentNetwork::RefreshList()
{
	// Clear old actors
	for( auto* p : m_pRowTexts ) {
		this->RemoveChild( p );
		delete p;
	}
	m_pRowTexts.clear();

	m_DisplayPacks = ContentSwarmManager::Instance()->GetAvailablePacks();

	float startY = 100;
	for( size_t i=0; i < m_DisplayPacks.size(); ++i )
	{
		SwarmPack& p = m_DisplayPacks[i];
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "normal") );

		std::string status = p.isLocal ? "[INSTALLED]" : "[REMOTE]";
		float progress = ContentSwarmManager::Instance()->GetDownloadProgress(p.id);
		if( progress > 0.0f && progress < 1.0f ) status = ssprintf("[DL %.0f%%]", progress*100);

		std::string sEntry = ssprintf( "%s %s (Seeds: %d)", status.c_str(), p.name.c_str(), p.seedCount );

		pText->SetText( sEntry );
		pText->SetXY( 320, startY + (i * 30) );

		if( (int)i == m_iSelection ) pText->SetDiffuse( RageColor(1,1,0,1) ); // Highlight
		else pText->SetDiffuse( RageColor(1,1,1,1) );

		this->AddChild( pText );
		m_pRowTexts.push_back( pText );
	}
}

void ScreenContentNetwork::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	if( input.MenuI == GAME_BUTTON_DOWN )
	{
		if( m_iSelection < (int)m_DisplayPacks.size() - 1 ) m_iSelection++;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
	}
	else if( input.MenuI == GAME_BUTTON_UP )
	{
		if( m_iSelection > 0 ) m_iSelection--;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
	}
	else if( input.MenuI == GAME_BUTTON_START )
	{
		if( !m_DisplayPacks.empty() )
		{
			SwarmPack& p = m_DisplayPacks[m_iSelection];
			if( !p.isLocal )
			{
				ContentSwarmManager::Instance()->StartDownload(p.id);
				SOUND->PlayOnce( THEME->GetPathS("Common", "start") );
			}
		}
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
	}
}
