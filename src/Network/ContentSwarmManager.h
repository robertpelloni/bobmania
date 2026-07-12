#ifndef CONTENT_SWARM_MANAGER_H
#define CONTENT_SWARM_MANAGER_H

#include "RageUtil.h"
#include <vector>
#include <map>

struct lua_State;

struct ContentPack
{
    RString ID;
    RString Name;
    RString Author;
    long long SizeBytes;
    int Seeders;
};

// libtorrent fwd decl
namespace libtorrent {
    class session;
    struct torrent_handle;
}

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

    // Download state access
    float GetDownloadProgress( const RString& hash ) const;
    int GetSeedCount( const RString& hash ) const;
    int GetLeechCount( const RString& hash ) const;
    bool IsDownloadComplete( const RString& hash ) const;
    void CancelDownload( const RString& hash );

    // Lua
    void PushSelf( lua_State *L );

private:
    std::vector<ContentPack> m_AvailablePacks;
    bool m_bDiscovering;

    // std::unique_ptr<libtorrent::session> m_pSession;
    void* m_pSession; // Stubbed void* for now since libtorrent isn't included yet
    std::map<RString, void*> m_ActiveDownloads;
};

extern ContentSwarmManager* SWARMMAN;

#endif
