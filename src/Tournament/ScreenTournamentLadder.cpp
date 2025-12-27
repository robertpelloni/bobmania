#include "global.h"
#include "ScreenTournamentLadder.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "RageLog.h"
#include "InputEventPlus.h"
#include "Economy/EconomyManager.h" // To show user balance potentially
#include <algorithm>

// Register the screen so the theme can load it
REGISTER_SCREEN_CLASS( ScreenTournamentLadder );

void ScreenTournamentLadder::Init()
{
	ScreenWithMenuElements::Init();
	m_bShowBracket = false;

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "Global Athletic League" );
	m_textTitle.SetXY( 320, 40 );
	m_textTitle.SetDiffuse( RageColor(1,1,0,1) );
	this->AddChild( &m_textTitle );

	m_textDivision.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textDivision.SetText( "Season 1 - Gold Division" );
	m_textDivision.SetXY( 320, 70 );
	this->AddChild( &m_textDivision );

	m_textViewMode.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textViewMode.SetText( "Press SELECT to view Bracket" );
	m_textViewMode.SetZoom( 0.6f );
	m_textViewMode.SetXY( 320, 440 );
	this->AddChild( &m_textViewMode );

	// Mock Data for the Ladder
	m_LadderEntries.push_back( { "1", "Chris Chike", 2450, "PRO" } );
	m_LadderEntries.push_back( { "2", "FEFEMZ", 2410, "PRO" } );
	m_LadderEntries.push_back( { "3", "IamChris4Life", 2390, "PRO" } );
	m_LadderEntries.push_back( { "4", "HappyF33t", 2100, "GOLD" } );
	m_LadderEntries.push_back( { "5", "DDR_Fan_99", 1950, "GOLD" } );

	// Add real player stats
	int myElo = EconomyManager::Instance()->GetPlayerElo();
	std::string myTitle = EconomyManager::Instance()->GetEquippedAsset("Title");
	if (myTitle.empty()) myTitle = "Novice";
	std::string name = "YOU [" + myTitle + "]";

	m_LadderEntries.push_back( { "?", name, myElo, "GOLD" } );

	m_LadderEntries.push_back( { "7", "RhythmGamer", 1150, "GOLD" } );
	m_LadderEntries.push_back( { "8", "NoobMaster69", 1000, "SILVER" } );

	// Sort by Points
	std::sort(m_LadderEntries.begin(), m_LadderEntries.end(),
		[](const LadderEntry& a, const LadderEntry& b) {
			return a.points > b.points;
		});

	// Re-assign ranks
	std::vector<std::string> names;
	for(size_t i=0; i<m_LadderEntries.size(); ++i) {
		m_LadderEntries[i].rank = std::to_string(i+1);
		names.push_back(m_LadderEntries[i].name);
	}

	// Generate Bracket
	m_Bracket.Generate(names);

	// Create reusable rows (max 10)
	for( int i=0; i < 10; ++i )
	{
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "normal") );
		pText->SetXY( 320, 120 + (i * 30) );
		this->AddChild( pText );
		m_pRowTexts.push_back( pText );
	}

	RefreshView();
	m_iSelection = 0;
}

void ScreenTournamentLadder::RefreshView()
{
	for(auto* p : m_pRowTexts) p->SetText(""); // Clear

	if( m_bShowBracket )
	{
		m_textViewMode.SetText( "Press SELECT to view Ladder" );
		const auto& matches = m_Bracket.GetMatches();

		for( size_t i=0; i < matches.size() && i < m_pRowTexts.size(); ++i )
		{
			const auto& m = matches[i];
			std::string sMatch = ssprintf( "M%d: %s vs %s -> %s",
				m.id, m.p1.c_str(), m.p2.c_str(),
				m.winner.empty() ? "?" : m.winner.c_str() );
			m_pRowTexts[i]->SetText( sMatch );
		}
	}
	else
	{
		m_textViewMode.SetText( "Press SELECT to view Bracket" );
		for( size_t i=0; i < m_LadderEntries.size() && i < m_pRowTexts.size(); ++i )
		{
			std::string sEntry = ssprintf( "#%s  %s  (%d LP)",
				m_LadderEntries[i].rank.c_str(),
				m_LadderEntries[i].name.c_str(),
				m_LadderEntries[i].points
			);
			m_pRowTexts[i]->SetText( sEntry );
			if( m_LadderEntries[i].name == "YOU" )
				m_pRowTexts[i]->SetDiffuse( RageColor(0,1,0,1) );
			else
				m_pRowTexts[i]->SetDiffuse( RageColor(1,1,1,1) );
		}
	}
}

void ScreenTournamentLadder::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	if( input.MenuI == GAME_BUTTON_SELECT )
	{
		m_bShowBracket = !m_bShowBracket;
		RefreshView();
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
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
