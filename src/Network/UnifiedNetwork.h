#ifndef UNIFIED_NETWORK_H
#define UNIFIED_NETWORK_H

#include "RageUtil.h"
#include "ezsockets.h"
#include "json/json.h" // Ensure JsonCpp is available
#include <map>
#include <functional>

// Use std::function for flexible callbacks (requires C++11, which SM5 supports)
typedef std::function<void(const Json::Value&)> PacketCallback;

class UnifiedNetwork
{
public:
    UnifiedNetwork();
    ~UnifiedNetwork();

    void Init();
    void Update( float fDeltaTime );

    // Connection
    bool Connect( const RString& sHost, unsigned short iPort );
    void Disconnect();
    bool IsConnected() const;

    // Messaging
    void SendJSON( const RString& sMessageID, const Json::Value& payload );

    // Callbacks
    void RegisterCallback( const RString& sMessageID, PacketCallback callback );

private:
    EzSockets* m_pSocket;
    bool m_bConnected;
    RString m_sBuffer;

    std::map<RString, PacketCallback> m_Callbacks;
};

extern UnifiedNetwork* UNIFIED_NET;

#endif
