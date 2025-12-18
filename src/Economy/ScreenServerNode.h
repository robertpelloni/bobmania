#ifndef SCREEN_SERVER_NODE_H
#define SCREEN_SERVER_NODE_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"

class ScreenServerNode : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Update( float fDeltaTime );
	virtual void Input( const InputEventPlus &input );

private:
	BitmapText m_textStatus;
	BitmapText m_textConsole; // Matrix-style log
	BitmapText m_textEarnings;

	std::vector<std::string> m_LogLines;
	float m_fLogTimer;
};

#endif
