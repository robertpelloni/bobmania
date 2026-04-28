#include "global.h"
#include "ActorOffsetBar.h"
#include "ThemeManager.h"
#include "RageUtil.h"

ActorOffsetBar::ActorOffsetBar()
{
	m_iTickIndex = 0;

	// Create background
	m_bg.SetWidth( 300 );
	m_bg.SetHeight( 20 );
	m_bg.SetDiffuse( RageColor(0,0,0,0.5f) );
	this->AddChild( &m_bg );

	// Center line (Perfect)
	m_centerLine.SetWidth( 2 );
	m_centerLine.SetHeight( 24 );
	m_centerLine.SetDiffuse( RageColor(1,1,1,1) );
	this->AddChild( &m_centerLine );

	// Create pool of tick marks
	for( int i=0; i<30; ++i )
	{
		Quad* q = new Quad;
		q->SetWidth( 4 );
		q->SetHeight( 16 );
		q->SetVisible( false );
		this->AddChild( q );
		m_ticks.push_back( q );
	}
}

void ActorOffsetBar::Load( const std::string &sMetricsGroup )
{
	// Load metrics if needed
}

void ActorOffsetBar::AddJudgment( float fOffsetSeconds, bool bMiss )
{
	if( bMiss ) return; // Don't show misses on bar typically

	Quad* q = m_ticks[m_iTickIndex];
	m_iTickIndex = (m_iTickIndex + 1) % m_ticks.size();

	// Scale offset to pixels. Assume Window is +/- 0.150s
	// Width 300px -> +/- 150px
	// 1ms = 1px roughly
	float fX = fOffsetSeconds * 1000.0f;
	CLAMP( fX, -145.0f, 145.0f );

	q->SetXY( fX, 0 );
	q->SetVisible( true );
	q->FinishTweening();
	q->SetZoom( 1.5f );
	q->SetDiffuse( RageColor( fOffsetSeconds < 0 ? 0 : 1, 0.5f, fOffsetSeconds > 0 ? 0 : 1, 1 ) ); // Blue=Early, Red=Late (Simple)

	// Tween out
	q->BeginTweening( 0.5f );
	q->SetZoom( 1.0f );
	q->SetDiffuseAlpha( 0 );
}
