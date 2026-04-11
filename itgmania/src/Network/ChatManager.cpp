#include "global.h"
#include "ChatManager.h"
#include "UnifiedNetwork.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "DateTime.h"

ChatManager* CHATMAN = nullptr;

ChatManager::ChatManager()
{
    m_sUsername = "Player"; // Default
}

ChatManager::~ChatManager()
{
}

void ChatManager::Init()
{
    LOG->Trace("ChatManager::Init()");
    if( UNIFIED_NET )
    {
        UNIFIED_NET->RegisterCallback("chat_message", std::bind(&ChatManager::HandleChatMessage, this, std::placeholders::_1));
    }
}

void ChatManager::SendChat( const RString& sText )
{
    if( sText.empty() ) return;

    if( UNIFIED_NET && UNIFIED_NET->IsConnected() )
    {
        Json::Value payload;
        payload["user"] = m_sUsername;
        payload["text"] = sText;
        payload["timestamp"] = DateTime::GetNowTime().GetString();
        UNIFIED_NET->SendJSON("chat_message", payload);
    }
    else
    {
        // Local echo / Offline mode
        ChatMessage msg;
        msg.User = m_sUsername;
        msg.Text = sText;
        msg.Timestamp = DateTime::GetNowTime().GetString();
        m_Messages.push_back(msg);
    }
}

void ChatManager::HandleChatMessage( const Json::Value& payload )
{
    ChatMessage msg;
    msg.User = payload["user"].asString();
    msg.Text = payload["text"].asString();
    msg.Timestamp = payload["timestamp"].asString();

    m_Messages.push_back(msg);

    // Limit history
    if( m_Messages.size() > 50 ) m_Messages.erase( m_Messages.begin() );

    LOG->Trace("Chat: [%s] %s", msg.User.c_str(), msg.Text.c_str());
    MESSAGEMAN->Broadcast("NewChatMessage");
}

const std::vector<ChatMessage>& ChatManager::GetMessages() const
{
    return m_Messages;
}

// Lua Bindings
class LunaChatManager: public Luna<ChatManager>
{
public:
    static int SendChat( T* p, lua_State *L )
    {
        p->SendChat(SArg(1));
        return 0;
    }

    static int GetMessages( T* p, lua_State *L )
    {
        const auto& msgs = p->GetMessages();
        lua_newtable(L);
        for( size_t i=0; i<msgs.size(); ++i )
        {
            lua_newtable(L);
            lua_pushstring(L, "User"); lua_pushstring(L, msgs[i].User); lua_settable(L, -3);
            lua_pushstring(L, "Text"); lua_pushstring(L, msgs[i].Text); lua_settable(L, -3);
            lua_pushstring(L, "Time"); lua_pushstring(L, msgs[i].Timestamp); lua_settable(L, -3);
            lua_rawseti(L, -2, i+1);
        }
        return 1;
    }

    LunaChatManager()
    {
        ADD_METHOD( SendChat );
        ADD_METHOD( GetMessages );
    }
};

LUA_REGISTER_CLASS( ChatManager )
