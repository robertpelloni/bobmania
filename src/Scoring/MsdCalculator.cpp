#include "global.h"
#include "MsdCalculator.h"
#include "Steps.h"
#include "NoteData.h"
#include "RageUtil.h"
#include "TimingData.h"
#include <algorithm>
#include <numeric>

// Helper struct for flattened note data
struct NoteInfo
{
	float time;
	int notes; // Number of notes in this row (1=tap, 2=jump, etc.)
	int columns; // Bitmask of columns
};

static std::vector<NoteInfo> ExtractNotes( const Steps* pSteps )
{
	std::vector<NoteInfo> notes;
	if( !pSteps ) return notes;

	NoteData nd;
	pSteps->GetNoteData( nd );
	TimingData* td = pSteps->GetTimingData();

	int iNumTracks = nd.GetNumTracks();
	int iLastRow = nd.GetLastRow();

	// Iterate all rows
	// NoteData iterator is a bit complex, let's use a simpler row loop for safety and clarity
	// optimizing for readability over raw speed for this MVP
	for( int r=0; r<=iLastRow; r++ )
	{
		if( !nd.IsRowEmpty(r) )
		{
			int count = 0;
			int mask = 0;
			for( int t=0; t<iNumTracks; t++ )
			{
				const TapNote& tn = nd.GetTapNote( t, r );
				if( tn.type == TapNoteType_Tap || tn.type == TapNoteType_HoldHead )
				{
					count++;
					mask |= (1 << t);
				}
			}

			if( count > 0 )
			{
				float time = td->GetElapsedTimeFromBeat( NoteRowToBeat(r) );
				notes.push_back( { time, count, mask } );
			}
		}
	}

	return notes;
}

float MsdCalculator::CalculateMSD( const Steps* pSteps )
{
	if( !pSteps ) return 0.0f;

	std::vector<NoteInfo> notes = ExtractNotes( pSteps );
	if( notes.empty() ) return 0.0f;

	// Window-based Strain Calculation
	// We slide a window of 'windowSize' seconds across the chart
	float windowSize = 1.0f; // 1 second windows
	float interval = 0.5f;   // Slide by 0.5 seconds
	
	std::vector<float> strains;
	float endTime = notes.back().time;

	for( float t = 0; t < endTime; t += interval )
	{
		float windowStart = t;
		float windowEnd = t + windowSize;
		
		int noteCount = 0;
		int jumpCount = 0;
		int handCount = 0;

		for( const auto& n : notes )
		{
			if( n.time >= windowStart && n.time < windowEnd )
			{
				noteCount += n.notes;
				if( n.notes == 2 ) jumpCount++;
				if( n.notes >= 3 ) handCount++;
			}
		}

		// Calculate NPS for this window
		float nps = (float)noteCount / windowSize;

		// Apply pattern weights
		// Jumps are harder than streams of same NPS
		// Hands are significantly harder
		float strain = nps;
		
		// Simple weighting:
		// If 50% of notes are jumps, boost strain
		if( noteCount > 0 )
		{
			float jumpRatio = (float)(jumpCount * 2) / noteCount;
			float handRatio = (float)(handCount * 3) / noteCount;

			strain *= (1.0f + jumpRatio * 0.5f); // Up to 1.5x for pure jumpstream
			strain *= (1.0f + handRatio * 1.0f); // Up to 2.0x for pure handstream
		}

		strains.push_back( strain );
	}

	if( strains.empty() ) return 0.0f;

	// Aggregate Strain
	// Sort descending
	std::sort( strains.begin(), strains.end(), std::greater<float>() );

	// Take top 10% average
	int topCount = max( 1, (int)(strains.size() * 0.1f) );
	float sum = 0.0f;
	for( int i=0; i<topCount; i++ )
	{
		sum += strains[i];
	}

	float avgPeakStrain = sum / topCount;

	// Scale to Etterna-ish values
	// Etterna 20 MSD is roughly 20 NPS stream or equivalent complexity
	// Our strain is roughly NPS * complexity.
	// So 1:1 mapping is a decent starting point.
	
	return avgPeakStrain;
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

	std::vector<NoteInfo> notes = ExtractNotes( pSteps );
	if( notes.empty() ) return tags;

	int totalNotes = 0;
	int jumps = 0;
	int hands = 0;
	int jacks = 0;

	// Analyze for jacks (same column < 150ms)
	for( size_t i=1; i<notes.size(); i++ )
	{
		float dt = notes[i].time - notes[i-1].time;
		if( dt < 0.15f && (notes[i].columns & notes[i-1].columns) )
		{
			jacks++;
		}
	}

	for( const auto& n : notes )
	{
		totalNotes += n.notes;
		if( n.notes == 2 ) jumps++;
		if( n.notes >= 3 ) hands++;
	}

	if( totalNotes == 0 ) return tags;

	float jumpRatio = (float)jumps / notes.size(); // Rows with jumps / Total rows
	float handRatio = (float)hands / notes.size();
	float jackRatio = (float)jacks / notes.size();

	if( handRatio > 0.05f ) tags.push_back("Handstream");
	else if( jumpRatio > 0.15f ) tags.push_back("Jumpstream");
	else if( totalNotes > 500 ) tags.push_back("Stream"); // Generic stream tag if dense enough

	if( jackRatio > 0.05f ) tags.push_back("Jack");

	// Calculate MSD for Boss tag
	float msd = CalculateMSD( pSteps );
	if( msd > 25.0f ) tags.push_back("Boss");

	return tags;
}
