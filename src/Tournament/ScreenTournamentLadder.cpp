#include "global.h"
#include "ScreenTournamentLadder.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "RageLog.h"
#include "InputEventPlus.h"
#include "EconomyManager.h" // To show user balance potentially

// Register the screen so the theme can load it
REGISTER_SCREEN_CLASS( ScreenTournamentLadder );

void ScreenTournamentLadder::Init()
{
	ScreenWithMenuElements::Init();

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "Global Athletic League" );
	m_textTitle.SetXY( 320, 40 );
	m_textTitle.SetDiffuse( RageColor(1,1,0,1) );
	this->AddChild( &m_textTitle );

	m_textDivision.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textDivision.SetText( "Season 1 - Gold Division" );
	m_textDivision.SetXY( 320, 70 );
	this->AddChild( &m_textDivision );

	// Mock Data for the Ladder
	m_LadderEntries.push_back( { "1", "Chris Chike", "2450 LP", "PRO" } );
	m_LadderEntries.push_back( { "2", "FEFEMZ", "2410 LP", "PRO" } );
	m_LadderEntries.push_back( { "3", "IamChris4Life", "2390 LP", "PRO" } );
	m_LadderEntries.push_back( { "4", "HappyF33t", "2100 LP", "GOLD" } );
	m_LadderEntries.push_back( { "5", "DDR_Fan_99", "1950 LP", "GOLD" } );
	m_LadderEntries.push_back( { "6", "YOU", "1200 LP", "GOLD" } ); // The player
	m_LadderEntries.push_back( { "7", "RhythmGamer", "1150 LP", "GOLD" } );

	// Create rows
	float startY = 120;
	float spacing = 30;

	for( size_t i=0; i < m_LadderEntries.size(); ++i )
	{
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "normal") );

		std::string sEntry = ssprintf( "#%s  %s  (%s)",
			m_LadderEntries[i].rank.c_str(),
			m_LadderEntries[i].name.c_str(),
			m_LadderEntries[i].points.c_str()
		);

		pText->SetText( sEntry );
		pText->SetXY( 320, startY + (i * spacing) );

		if( m_LadderEntries[i].name == "YOU" )
			pText->SetDiffuse( RageColor(0,1,0,1) ); // Highlight player

		this->AddChild( pText );
		m_pRowTexts.push_back( pText );
	}

	m_iSelection = 0;
}

void ScreenTournamentLadder::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->PlayStartSound();
		SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
		return;
	}

	ScreenWithMenuElements::Input( input );
}

void ScreenTournamentLadder::HandleScreenMessage( const ScreenMessage SM )
{
	ScreenWithMenuElements::HandleScreenMessage( SM );
}
