#ifndef ACTOR_CALORIE_GRAPH_H
#define ACTOR_CALORIE_GRAPH_H

#include "ActorFrame.h"
#include "Quad.h"

class ActorCalorieGraph : public ActorFrame
{
public:
	ActorCalorieGraph();
	void LoadData( const std::vector<float>& history ); // Last 7 days calories

private:
	std::vector<Quad*> m_Bars;
};

#endif
