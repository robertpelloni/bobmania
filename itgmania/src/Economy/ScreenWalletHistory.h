#ifndef SCREEN_WALLET_HISTORY_H
#define SCREEN_WALLET_HISTORY_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"
#include "EconomyManager.h"

class ScreenWalletHistory : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );

private:
	BitmapText m_textTitle;
	std::vector<BitmapText*> m_pRowTexts;
	std::vector<Transaction> m_History;
	int m_iScrollOffset;
};

#endif
