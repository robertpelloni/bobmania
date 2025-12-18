#ifndef SCREEN_MARKETPLACE_H
#define SCREEN_MARKETPLACE_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"

struct MarketItem {
	int id;
	std::string name;
	std::string category;
	long long price;
	bool owned;
};

class ScreenMarketplace : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );

private:
	void RefreshList();
	void BuyItem( int index );

	BitmapText m_textTitle;
	BitmapText m_textBalance;
	BitmapText m_textStatus;

	std::vector<MarketItem> m_Items;
	std::vector<BitmapText*> m_pRowTexts;
	int m_iSelection;
};

#endif
