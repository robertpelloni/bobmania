#ifndef SCREEN_UNIFIED_DASHBOARD_H
#define SCREEN_UNIFIED_DASHBOARD_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"
#include "Sprite.h"

class ScreenUnifiedDashboard : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Update( float fDeltaTime );
	virtual void Input( const InputEventPlus &input );

private:
	// Top Bar
	BitmapText m_textHeader;
	BitmapText m_textClock;

	// Widgets
	BitmapText m_textWalletWidget;
	BitmapText m_textRankWidget;
	BitmapText m_textSwarmWidget;

	// Navigation
	int m_iSelection;
	std::vector<std::string> m_Options;
	std::vector<BitmapText*> m_pOptionTexts;

	Sprite m_sprCursor;
};

#endif
