#include "global.h"
#include "ContentSwarmManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "FileDownload.h"
#include "ScreenManager.h"

ContentSwarmManager* SWARMMAN = nullptr;

ContentSwarmManager::ContentSwarmManager()
{
    m_bDiscovering = false;
    m_pTransfer = nullptr;
}

ContentSwarmManager::~ContentSwarmManager()
{
}

void ContentSwarmManager::Init()
{
    LOG->Trace("ContentSwarmManager::Init()");
}

void ContentSwarmManager::StartDiscovery()
{
    if( m_bDiscovering ) return;

    LOG->Trace("ContentSwarmManager: Starting P2P Discovery...");
    m_bDiscovering = true;

    // Mock Discovery: Populate with fake remote packs
    m_AvailablePacks.clear();
    m_AvailablePacks.push_back({ "p001", "Community Pack 1", "StepArtist_A", 104857600, 5 });
    m_AvailablePacks.push_back({ "p002", "Hardcore Rave", "DJ_Speed", 209715200, 12 });
    m_AvailablePacks.push_back({ "p003", "Chillout Mix", "SlowJamz", 52428800, 2 });
}

const std::vector<ContentPack>& ContentSwarmManager::GetAvailablePacks() const
{
    return m_AvailablePacks;
}

void ContentSwarmManager::RequestPack( const RString& sPackID )
{
    LOG->Trace("ContentSwarmManager: Requesting Pack %s", sPackID.c_str());

    // Check if a transfer is already running
    if( m_pTransfer != nullptr && !m_pTransfer->IsFinished() )
    {
        SCREENMAN->SystemMessage("A download is already in progress.");
        return;
    }

    m_pTransfer = std::make_unique<FileTransfer>();

    // For the Content Swarm, we communicate with the local Supernode (Node.js/webtorrent)
    // In the future, this will be a direct magnet link resolution.
    RString sURL = "http://127.0.0.1:8081/add-torrent"; // P2P Supernode Port
    RString sDest = "Packages/" + sPackID + ".smzip";

    // Simulate requesting the supernode to join the swarm
    m_pTransfer->StartDownload( sURL, sDest );
    SCREENMAN->SystemMessage("ContentSwarm: Joining swarm for " + sPackID + "...");
}

void ContentSwarmManager::AddLocalPack( const RString& sPath )
{
    LOG->Trace("ContentSwarmManager: Adding local pack to swarm: %s", sPath.c_str());
    // In a real scenario, we'd POST the file to the local Supernode /upload endpoint
}

void ContentSwarmManager::Update( float fDeltaTime )
{
    if( m_pTransfer )
    {
        m_pTransfer->Update( fDeltaTime );
        if( m_pTransfer->IsFinished() )
        {
            if( m_pTransfer->GetResponseCode() >= 200 && m_pTransfer->GetResponseCode() < 300 )
            {
                SCREENMAN->SystemMessage("Download Complete!");
                // Trigger reload if we wanted to auto-mount, but StepMania auto-mounts Packages/
            }
            else
            {
                SCREENMAN->SystemMessage("Download Failed: " + m_pTransfer->GetStatus());
            }
            m_pTransfer.reset();
        }
    }
}

// Lua Bindings
class LunaContentSwarmManager: public Luna<ContentSwarmManager>
{
public:
    static int StartDiscovery( T* p, lua_State *L )
    {
        p->StartDiscovery();
        return 0;
    }

    static int GetAvailablePacks( T* p, lua_State *L )
    {
        const auto& packs = p->GetAvailablePacks();
        lua_newtable(L);
        for( size_t i=0; i<packs.size(); ++i )
        {
            lua_newtable(L);
            lua_pushstring(L, "ID"); lua_pushstring(L, packs[i].ID); lua_settable(L, -3);
            lua_pushstring(L, "Name"); lua_pushstring(L, packs[i].Name); lua_settable(L, -3);
            lua_pushstring(L, "Author"); lua_pushstring(L, packs[i].Author); lua_settable(L, -3);
            lua_pushnumber(L, packs[i].Seeders); lua_setfield(L, -2, "Seeders");
            lua_rawseti(L, -2, i+1);
        }
        return 1;
    }

    static int RequestPack( T* p, lua_State *L )
    {
        p->RequestPack(SArg(1));
        return 0;
    }

    static int AddLocalPack( T* p, lua_State *L )
    {
        p->AddLocalPack(SArg(1));
        return 0;
    }

    LunaContentSwarmManager()
    {
        ADD_METHOD( StartDiscovery );
        ADD_METHOD( GetAvailablePacks );
        ADD_METHOD( RequestPack );
        ADD_METHOD( AddLocalPack );
    }
};

LUA_REGISTER_CLASS( ContentSwarmManager )
