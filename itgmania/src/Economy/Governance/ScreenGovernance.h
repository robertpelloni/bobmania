#ifndef SCREEN_GOVERNANCE_H
#define SCREEN_GOVERNANCE_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"

class ScreenGovernance : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );

private:
	void UpdateUI();
	void Vote( bool bYes );

	BitmapText m_textTitle;
	BitmapText m_textTreasury;

	// Single Active Proposal Display
	BitmapText m_textPropTitle;
	BitmapText m_textPropDesc;
	BitmapText m_textVotes;

	int m_iCurrentPropIndex;
};

#endif
