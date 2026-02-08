#include "global.h"
#include "ActorMsdGraph.h"
#include "ThemeManager.h"
#include "RageUtil.h"

ActorMsdGraph::ActorMsdGraph()
{
	m_Background.SetWidth( 300 );
	m_Background.SetHeight( 100 );
	m_Background.SetDiffuse( RageColor(0,0,0,0.5f) );
	this->AddChild( &m_Background );

	m_Line.SetWidth( 2.0f );
	this->AddChild( &m_Line );
}

void ActorMsdGraph::LoadFromNoteData( const NoteData& nd, const float fMusicLength )
{
	// Sampling resolution (e.g. 50 points)
	int samples = 50;
	float fInterval = fMusicLength / samples;

	// Mock Calculation of density per interval
	// In reality, we'd use NoteData::GetNumTapNotesInRange

	for( int i=0; i < samples; ++i )
	{
		float fTime = i * fInterval;

		// Fake density for MVP (Sin wave)
		float fDensity = 5.0f + 5.0f * sinf( fTime * 0.1f );

		// Map to XY
		// X: -150 to 150
		// Y: 50 to -50 (Inverted Y in SM)

		float fX = SCALE( (float)i, 0.0f, (float)samples, -150.0f, 150.0f );
		float fY = SCALE( fDensity, 0.0f, 15.0f, 50.0f, -50.0f );

		m_Line.AddVertex( RageVector3(fX, fY, 0) );
	}

	m_Line.SetDiffuse( RageColor(0,1,1,1) ); // Cyan line
}
