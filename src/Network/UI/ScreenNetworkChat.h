#ifndef SCREEN_NETWORK_CHAT_H
#define SCREEN_NETWORK_CHAT_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"

class ScreenNetworkChat : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Update( float fDeltaTime );
	virtual bool Input( const InputEventPlus &input );

private:
	BitmapText m_textChatBox;
	BitmapText m_textInput;

	std::vector<std::string> m_Messages;
	std::string m_sInputBuffer;

	float m_fAutoChatTimer;
};

#endif
