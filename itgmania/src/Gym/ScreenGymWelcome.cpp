#include "global.h"
#include "ScreenGymWelcome.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "Economy/EconomyManager.h"
#include "InputEventPlus.h"
#include "RageUtil.h"
#include "GymPlaylistGenerator.h"
#include "GymCourseWriter.h"

REGISTER_SCREEN_CLASS( ScreenGymWelcome );

void ScreenGymWelcome::Init()
{
	ScreenWithMenuElements::Init();

	m_textWelcome.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textWelcome.SetText( "GYM MODE" );
	m_textWelcome.SetXY( 320, 100 );
	m_textWelcome.SetDiffuse( RageColor(0.2f, 1.0f, 0.4f, 1.0f) ); // Greenish
	this->AddChild( &m_textWelcome );

	m_textInstruction.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textInstruction.SetText( "Press START to begin your Quick Workout" );
	m_textInstruction.SetXY( 320, 200 );
	this->AddChild( &m_textInstruction );

	m_textCalories.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textCalories.SetText( "Today's Goal: 300 kcal" );
	m_textCalories.SetXY( 320, 240 );
	this->AddChild( &m_textCalories );

	// Intensity Selection
	m_Options = { INTENSITY_LIGHT, INTENSITY_MODERATE, INTENSITY_VIGOROUS, INTENSITY_PRO_ATHLETE };
	m_iSelection = 1; // Default to Moderate

	m_textIntensity.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textIntensity.SetXY( 320, 280 );
	this->AddChild( &m_textIntensity );
	UpdateSelection();

	// Show Server Status (Simulated "Working" Machine)
	m_textServerStatus.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textServerStatus.SetZoom( 0.6f );
	m_textServerStatus.SetXY( 320, 400 );
	this->AddChild( &m_textServerStatus );

	// Stats Graph
	m_Graph.SetXY( 320, 350 );
	std::vector<float> mockHistory = { 150, 200, 320, 100, 450, 300, 300 }; // Mock
	m_Graph.LoadData( mockHistory );
	this->AddChild( &m_Graph );

	// Initial update of text
	long long earned = EconomyManager::Instance()->GetMiningReward();
	m_textServerStatus.SetText( ssprintf("Server Node Active... Mined: %lld Coins", earned) );
}

void ScreenGymWelcome::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	if( input.MenuI == GAME_BUTTON_MENULEFT )
	{
		m_iSelection--;
		if( m_iSelection < 0 ) m_iSelection = m_Options.size() - 1;
		UpdateSelection();
	}
	else if( input.MenuI == GAME_BUTTON_MENURIGHT )
	{
		m_iSelection++;
		if( m_iSelection >= (int)m_Options.size() ) m_iSelection = 0;
		UpdateSelection();
	}
	else if( input.MenuI == GAME_BUTTON_START )
	{
		// Generate a workout playlist
		// In a real flow, we would set GAMESTATE->m_pCurCourse to this new course
		// and then jump to ScreenGameplay directly or ScreenSelectCourse.

		Course* pWorkout = GymPlaylistGenerator::GenerateCircuit( 1200.0f, m_Options[m_iSelection] );
		if (pWorkout) {
		   // Use a user-writable path (e.g., Save/ directory) if possible, but standard is Songs/
		   // For simulation, we assume write permissions.
		   GymCourseWriter::WriteCourse(pWorkout, "Save/LocalProfiles/MyWorkout.crs");
		   
		   // Set GameState for Course Play
		   GAMESTATE->m_PlayMode.Set( PLAY_MODE_NONSTOP );
		   GAMESTATE->m_pCurCourse.Set( pWorkout );
		}

		// Jump to Stage Information to start the course
		SCREENMAN->SetNewScreen( "ScreenStageInformation" );
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
	}
}

void ScreenGymWelcome::UpdateSelection()
{
	RString sIntensity = "Unknown";
	switch( m_Options[m_iSelection] )
	{
	case INTENSITY_LIGHT:      sIntensity = "Light (Meter 1-3)"; break;
	case INTENSITY_MODERATE:   sIntensity = "Moderate (Meter 4-7)"; break;
	case INTENSITY_VIGOROUS:   sIntensity = "Vigorous (Meter 8-11)"; break;
	case INTENSITY_PRO_ATHLETE: sIntensity = "Pro Athlete (Meter 12-16)"; break;
	}
	m_textIntensity.SetText( "< " + sIntensity + " >" );
}
