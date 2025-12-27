#ifndef SCREEN_GYM_WELCOME_H
#define SCREEN_GYM_WELCOME_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"
#include "ActorCalorieGraph.h"
#include "GymPlaylistGenerator.h" // For WorkoutIntensity enum

class ScreenGymWelcome : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );

private:
	void UpdateSelection();

	BitmapText m_textWelcome;
	BitmapText m_textInstruction;
	BitmapText m_textCalories;
	BitmapText m_textServerStatus;
	BitmapText m_textIntensity;

	ActorCalorieGraph m_Graph;

	int m_iSelection;
	std::vector<WorkoutIntensity> m_Options;
};

#endif
