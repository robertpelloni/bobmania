#include "global.h"
#include "MsdCalculator.h"
#include "Steps.h"
#include "NoteData.h"
#include "RageUtil.h"

float MsdCalculator::CalculateMSD( const Steps* pSteps )
{
	if( !pSteps ) return 0.0f;

	NoteData nd;
	pSteps->GetNoteData( nd );

	// Simulation of MSD Calculation:
	// In reality, this runs a complex algorithm analyzing stream speed, jack density, etc.
	// For this MVP, we approximate it using Total Taps / Song Length * Modifier.

	int iTotalTaps = nd.GetNumTapNotes();
	float fLengthSeconds = 120.0f; // Mock length if not available easily without Song pointer

	// Etterna MSD ranges roughly 1.0 to 35.0+
	// Let's fake it based on the meter for now, plus density.
	float fBaseMeter = (float)pSteps->GetMeter();
	float fDensity = (float)iTotalTaps / fLengthSeconds;

	float fMsd = fBaseMeter * 0.8f + (fDensity * 0.5f);

	return fMsd;
}

std::string MsdCalculator::GetMsdString( const Steps* pSteps )
{
	float fMsd = CalculateMSD( pSteps );
	return ssprintf( "MSD: %.2f", fMsd );
}

std::vector<std::string> MsdCalculator::GetTags( const Steps* pSteps )
{
	std::vector<std::string> tags;
	if( !pSteps ) return tags;

	NoteData nd;
	pSteps->GetNoteData( nd );

	int jumps = nd.GetNumJumpNotes();
	int taps = nd.GetNumTapNotes();

	if( jumps > taps / 4 ) tags.push_back("Jumpstream");
	else if( taps > 1000 ) tags.push_back("Stream");

	if( pSteps->GetMeter() > 12 ) tags.push_back("Boss");

	return tags;
}
