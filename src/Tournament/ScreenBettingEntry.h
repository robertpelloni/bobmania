#ifndef SCREEN_BETTING_ENTRY_H
#define SCREEN_BETTING_ENTRY_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"

class ScreenBettingEntry : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );

private:
	BitmapText m_textTitle;
	BitmapText m_textInstructions;
	BitmapText m_textWagerAmount;
	BitmapText m_textWalletBalance;

	long long m_iWager;
};

#endif
