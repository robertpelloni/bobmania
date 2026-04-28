#ifndef SCREEN_LOGIN_QR_H
#define SCREEN_LOGIN_QR_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"

class ScreenLoginQR : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );

private:
	BitmapText m_textPrompt;
	BitmapText m_textStatus;

	std::string m_sBuffer; // Buffer for barcode scanner input
};

#endif
