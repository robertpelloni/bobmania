#include "global.h"
#include "ScreenGymWelcome.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "EconomyManager.h"
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

	if( input.MenuI == GAME_BUTTON_START )
	{
		// Generate a workout playlist
		// In a real flow, we would set GAMESTATE->m_pCurCourse to this new course
		// and then jump to ScreenGameplay directly or ScreenSelectCourse.

		// Course* pWorkout = GymPlaylistGenerator::GenerateCircuit( 1200.0f, INTENSITY_MODERATE );
		// if (pWorkout) {
		//    GymCourseWriter::WriteCourse(pWorkout, "Songs/MyWorkout.crs");
		//    GAMESTATE->m_pCurCourse.Set( pWorkout );
		// }

		// For MVP, just jump to music select
		SCREENMAN->SetNewScreen( "ScreenSelectMusic" );
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenTitleMenu" );
	}
}
