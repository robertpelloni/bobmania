#ifndef UNIFIED_NETWORK_H
#define UNIFIED_NETWORK_H

#include "global.h"
#include <string>
#include <vector>
#include <mutex>

// Thread-safe generic networking interface to completely replace EzSockets.
class UnifiedNetwork
{
public:
    UnifiedNetwork();
    ~UnifiedNetwork();

    bool Connect(const RString& ip, unsigned short port);
    void Disconnect();

    // Non-blocking send
    bool Send(const std::string& data);

    // Non-blocking receive: returns true if data was received
    bool Receive(std::string& outData);

    bool IsConnected() const;

private:
    bool m_bConnected;
    std::mutex m_Mutex;

    // Abstracting raw socket logic for the Unified implementation
    int m_iSocketFD;
};

#endif
