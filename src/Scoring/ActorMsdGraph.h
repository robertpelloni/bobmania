#ifndef ACTOR_MSD_GRAPH_H
#define ACTOR_MSD_GRAPH_H

#include "ActorFrame.h"
#include "LineStrip.h"
#include "NoteData.h"

// Visualization of Note Density (MSD/NPS) over time
class ActorMsdGraph : public ActorFrame
{
public:
	ActorMsdGraph();

	// Pre-calculate the graph points based on the chart
	void LoadFromNoteData( const NoteData& nd, const float fMusicLength );

private:
	LineStrip m_Line;
	Quad m_Background;
};

#endif
