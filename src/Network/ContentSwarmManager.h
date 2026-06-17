#ifndef CONTENT_SWARM_MANAGER_H
#define CONTENT_SWARM_MANAGER_H

#include "RageUtil.h"
#include <vector>
#include <memory>

struct lua_State;

struct ContentPack
{
    RString ID;
    RString Name;
    RString Author;
    long long SizeBytes;
    int Seeders;
};

class ContentSwarmManager
{
public:
    ContentSwarmManager();
    ~ContentSwarmManager();

    void Init();

    // API
    void StartDiscovery();
    const std::vector<ContentPack>& GetAvailablePacks() const;
    void RequestPack( const RString& sPackID );
    void AddLocalPack( const RString& sPath );

    void Update( float fDeltaTime );

    // Lua
    void PushSelf( lua_State *L );

private:
    std::vector<ContentPack> m_AvailablePacks;
    bool m_bDiscovering;
    std::unique_ptr<class FileTransfer> m_pTransfer;
};

extern ContentSwarmManager* SWARMMAN;

#endif
