#include "global.h"
#include "ContentSwarmManager.h"
#include "RageLog.h"
#include "LuaBinding.h"
#include "ScreenManager.h"

// If we had libtorrent:
// #include <libtorrent/session.hpp>
// #include <libtorrent/add_torrent_params.hpp>
// #include <libtorrent/torrent_handle.hpp>
// #include <libtorrent/magnet_uri.hpp>

ContentSwarmManager* SWARMMAN = nullptr;

ContentSwarmManager::ContentSwarmManager()
{
    m_bDiscovering = false;
    m_pSession = nullptr; // would be: new libtorrent::session(...)
}

ContentSwarmManager::~ContentSwarmManager()
{
    // delete (libtorrent::session*)m_pSession;
}

void ContentSwarmManager::Init()
{
    LOG->Trace("ContentSwarmManager::Init()");

    // Stub for session init:
    // libtorrent::settings_pack p;
    // p.set_int(libtorrent::settings_pack::alert_mask, libtorrent::alert::status_notification);
    // m_pSession = new libtorrent::session(p);
}

void ContentSwarmManager::StartDiscovery()
{
    if( m_bDiscovering ) return;

    LOG->Trace("ContentSwarmManager: Starting P2P Discovery...");
    m_bDiscovering = true;

    // Mock Discovery: Populate with fake remote packs (until tracker support)
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
    LOG->Trace("ContentSwarmManager: Requesting Pack %s via BitTorrent", sPackID.c_str());

    if( m_ActiveDownloads.find(sPackID) != m_ActiveDownloads.end() )
    {
        SCREENMAN->SystemMessage("Already downloading " + sPackID);
        return;
    }

    // Mock Torrent Download Addition
    // libtorrent::add_torrent_params p;
    // p.save_path = "Packages/";
    // p.ti = std::make_shared<libtorrent::torrent_info>(sPackID + ".torrent");
    // libtorrent::torrent_handle h = ((libtorrent::session*)m_pSession)->add_torrent(p);

    // Store handle pointer stub
    m_ActiveDownloads[sPackID] = nullptr;
    SCREENMAN->SystemMessage("Started BitTorrent download for: " + sPackID);
}

void ContentSwarmManager::Update( float fDeltaTime )
{
    // Poll libtorrent alerts
    // if (!m_pSession) return;
    // std::vector<libtorrent::alert*> alerts;
    // ((libtorrent::session*)m_pSession)->pop_alerts(&alerts);
    // for (libtorrent::alert const* a : alerts) {
    //     if (auto* finished = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a)) {
    //         SCREENMAN->SystemMessage("Torrent Complete!");
    //     }
    // }
}

float ContentSwarmManager::GetDownloadProgress( const RString& hash ) const
{
    // Stub
    if (m_ActiveDownloads.find(hash) == m_ActiveDownloads.end()) return 0.0f;
    return 0.5f; // Hardcoded mock progress
}

int ContentSwarmManager::GetSeedCount( const RString& hash ) const
{
    if (m_ActiveDownloads.find(hash) == m_ActiveDownloads.end()) return 0;
    return 10;
}

int ContentSwarmManager::GetLeechCount( const RString& hash ) const
{
    if (m_ActiveDownloads.find(hash) == m_ActiveDownloads.end()) return 0;
    return 3;
}

bool ContentSwarmManager::IsDownloadComplete( const RString& hash ) const
{
    // Stub
    return false;
}

void ContentSwarmManager::CancelDownload( const RString& hash )
{
    auto it = m_ActiveDownloads.find(hash);
    if( it != m_ActiveDownloads.end() )
    {
        // ((libtorrent::session*)m_pSession)->remove_torrent(handle);
        m_ActiveDownloads.erase(it);
        SCREENMAN->SystemMessage("Canceled download for " + hash);
    }
}
