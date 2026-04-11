#include "global.h"
#include "ActorCalorieGraph.h"
#include "RageUtil.h"

ActorCalorieGraph::ActorCalorieGraph()
{
	for( int i=0; i<7; ++i )
	{
		Quad* q = new Quad;
		q->SetWidth( 20 );
		q->SetHeight( 0 );
		q->SetXY( (i - 3) * 30.0f, 50.0f ); // Bottom aligned logic needed, simplified for now
		q->SetValign( 1.0f ); // Anchor bottom
		q->SetDiffuse( RageColor(1, 0.5f, 0, 1) );
		this->AddChild( q );
		m_Bars.push_back( q );
	}
}

void ActorCalorieGraph::LoadData( const std::vector<float>& history )
{
	for( size_t i=0; i<m_Bars.size() && i<history.size(); ++i )
	{
		float val = history[i];
		// Scale: Max 500kcal = 100px height
		float h = SCALE( val, 0.0f, 500.0f, 0.0f, 100.0f );
		m_Bars[i]->SetHeight( h );
	}
}
