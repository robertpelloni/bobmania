#ifndef SCREEN_CONTENT_NETWORK_H
#define SCREEN_CONTENT_NETWORK_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"
#include "ContentSwarmManager.h"

class ScreenContentNetwork : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Update( float fDeltaTime );
	virtual void Input( const InputEventPlus &input );

private:
	void RefreshList();

	BitmapText m_textTitle;
	BitmapText m_textEarnings;

	std::vector<BitmapText*> m_pRowTexts;
	std::vector<SwarmPack> m_DisplayPacks;
	int m_iSelection;
};

#endif
