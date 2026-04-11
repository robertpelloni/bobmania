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
	void RefreshView();
	void PlaceBet( long long amount );

	BitmapText m_textTitle;
	BitmapText m_textBalance;
	BitmapText m_textInstructions;

	std::vector<long long> m_WagerOptions;
	int m_iSelection;

	std::vector<BitmapText*> m_pOptionTexts;
};

#endif
