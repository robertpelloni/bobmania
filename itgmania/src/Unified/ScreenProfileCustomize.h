#ifndef SCREEN_PROFILE_CUSTOMIZE_H
#define SCREEN_PROFILE_CUSTOMIZE_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"
#include "EconomyManager.h"

class ScreenProfileCustomize : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );

private:
	void RefreshList();
	void EquipItem( int index );

	BitmapText m_textTitle;
	BitmapText m_textStatus;

	std::vector<Asset> m_Inventory;
	std::vector<BitmapText*> m_pRowTexts;
	int m_iSelection;
};

#endif
