#ifndef SCREEN_EVALUATION_BETTING_H
#define SCREEN_EVALUATION_BETTING_H

#include "ScreenEvaluation.h"
#include "BitmapText.h"

class ScreenEvaluationBetting : public ScreenEvaluation
{
public:
	virtual void Init();
	// Inherit Input and HandleScreenMessage from ScreenEvaluation

private:
	BitmapText m_textBetResult;
};

#endif
