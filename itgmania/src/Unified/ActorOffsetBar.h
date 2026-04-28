#ifndef ACTOR_OFFSET_BAR_H
#define ACTOR_OFFSET_BAR_H

#include "ActorFrame.h"
#include "Quad.h"
#include "BitmapText.h"

// Visualizes timing error (Early/Late) like in Etterna
class ActorOffsetBar : public ActorFrame
{
public:
	ActorOffsetBar();
	virtual void Load( const std::string &sMetricsGroup );

	// Called by ScreenGameplay when a tap occurs
	void AddJudgment( float fOffsetSeconds, bool bMiss );

private:
	Quad m_bg;
	Quad m_centerLine;
	std::vector<Quad*> m_ticks; // Pool of tick marks
	int m_iTickIndex;
};

#endif
