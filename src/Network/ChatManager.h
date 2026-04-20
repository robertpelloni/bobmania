#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include "RageUtil.h"
#include "json/json.h" // Needed for packet handling callback

struct lua_State;

struct ChatMessage
{
    RString User;
    RString Text;
    RString Timestamp;
};

class ChatManager
{
public:
    ChatManager();
    ~ChatManager();

    void Init();

    // API
    void SendChat( const RString& sText );
    const std::vector<ChatMessage>& GetMessages() const;
    void HandleChatMessage( const Json::Value& payload );

    // Lua
    void PushSelf( lua_State *L );

private:
    std::vector<ChatMessage> m_Messages;
    RString m_sUsername;
};

extern ChatManager* CHATMAN;

#endif
