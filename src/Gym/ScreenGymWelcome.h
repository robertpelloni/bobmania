#ifndef SCREEN_GYM_WELCOME_H
#define SCREEN_GYM_WELCOME_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"
#include "ActorCalorieGraph.h"

class ScreenGymWelcome : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );

private:
	BitmapText m_textWelcome;
	BitmapText m_textInstruction;
	BitmapText m_textCalories;
	BitmapText m_textServerStatus;

	ActorCalorieGraph m_Graph;
};

#endif
