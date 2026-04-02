#include "global.h"
#include "UnifiedNetwork.h"
#include "RageLog.h"
#include "JsonUtil.h"

UnifiedNetwork* UNIFIED_NET = nullptr;

UnifiedNetwork::UnifiedNetwork()
{
    m_pSocket = new EzSockets();
    m_bConnected = false;
}

UnifiedNetwork::~UnifiedNetwork()
{
    Disconnect();
    SAFE_DELETE( m_pSocket );
}

void UnifiedNetwork::Init()
{
    // Auto-connect to localhost for dev/testing if config exists?
    // For now, wait for explicit Connect call.
}

bool UnifiedNetwork::Connect( const RString& sHost, unsigned short iPort )
{
    if( m_bConnected ) Disconnect();

    m_pSocket->create();
    // m_pSocket->setBlocking(false); // EzSockets defaults?

    if( m_pSocket->connect( sHost, iPort ) )
    {
        m_bConnected = true;
        LOG->Trace( "UnifiedNetwork: Connected to %s:%d", sHost.c_str(), iPort );
        return true;
    }
    else
    {
        LOG->Warn( "UnifiedNetwork: Failed to connect to %s:%d", sHost.c_str(), iPort );
        return false;
    }
}

void UnifiedNetwork::Disconnect()
{
    if( m_pSocket ) m_pSocket->close();
    m_bConnected = false;
}

bool UnifiedNetwork::IsConnected() const
{
    return m_bConnected && m_pSocket && (m_pSocket->state == EzSockets::skCONNECTED);
}

void UnifiedNetwork::SendJSON( const RString& sMessageID, const Json::Value& payload )
{
    if( !IsConnected() ) return;

    Json::Value packet;
    packet["id"] = sMessageID;
    packet["payload"] = payload;

    RString sJson;
    if( JsonUtil::WriteToString(packet, sJson) )
    {
        // Append delimiter (newline)
        sJson += "\n";
        m_pSocket->SendData( sJson );
        LOG->Trace( "UnifiedNetwork: Sent %s", sJson.c_str() );
    }
}

void UnifiedNetwork::RegisterCallback( const RString& sMessageID, PacketCallback callback )
{
    m_Callbacks[sMessageID] = callback;
}

void UnifiedNetwork::Update( float fDeltaTime )
{
    if( !IsConnected() ) return;

    // Read data
    if( m_pSocket->CanRead() )
    {
        char buffer[1024];
        int bytes = m_pSocket->ReadData( buffer, 1024 );
        if( bytes > 0 )
        {
            m_sBuffer.append( buffer, bytes );

            // Process delimiter
            size_t pos;
            while( (pos = m_sBuffer.find('\n')) != RString::npos )
            {
                RString sLine = m_sBuffer.substr(0, pos);
                m_sBuffer.erase(0, pos + 1);

                // Parse JSON
                Json::Value root;
                RString sError;
                if( JsonUtil::LoadFromString(root, sLine, sError) )
                {
                    LOG->Trace( "UnifiedNetwork: Received %s", sLine.c_str() );

                    RString msgID = root["id"].asString();
                    if( m_Callbacks.find(msgID) != m_Callbacks.end() )
                    {
                        m_Callbacks[msgID]( root["payload"] );
                    }
                    else
                    {
                        LOG->Trace("UnifiedNetwork: No callback registered for %s", msgID.c_str());
                    }
                }
                else
                {
                    LOG->Warn( "UnifiedNetwork: JSON Parse Error: %s", sError.c_str() );
                }
            }
        }
        else if( bytes == 0 )
        {
            // Disconnected
            Disconnect();
        }
    }
}
