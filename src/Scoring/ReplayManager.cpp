#include "ReplayManager.h"
#include "RageLog.h"
#include <fstream>
#include <sstream>

ReplayManager* REPLAYMAN = nullptr;

ReplayManager::ReplayManager() : m_bRecording(false) {}

ReplayManager::~ReplayManager() {}

void ReplayManager::Init() {
    LOG->Trace("ReplayManager::Init");
}

void ReplayManager::Shutdown() {
    LOG->Trace("ReplayManager::Shutdown");
}

void ReplayManager::StartRecording() {
    m_bRecording = true;
    m_vActiveRecording.clear();
    LOG->Trace("ReplayManager started recording.");
}

void ReplayManager::StopRecording() {
    m_bRecording = false;
    LOG->Trace("ReplayManager stopped recording. Recorded %d inputs.", (int)m_vActiveRecording.size());
}

void ReplayManager::RecordInput(float fTime, int iColumn, bool bPressed) {
    if (!m_bRecording) return;
    ReplayInput in;
    in.fTime = fTime;
    in.iColumn = iColumn;
    in.bPressed = bPressed;
    m_vActiveRecording.push_back(in);
}

bool ReplayManager::SaveReplay(const RString& sFilePath) {
    if (m_vActiveRecording.empty()) return false;

    // Use standard fstream for mock, should use RageFile in real engine.
    std::ofstream out(sFilePath.c_str());
    if (!out.is_open()) return false;

    for (size_t i = 0; i < m_vActiveRecording.size(); ++i) {
        out << m_vActiveRecording[i].fTime << ","
            << m_vActiveRecording[i].iColumn << ","
            << (m_vActiveRecording[i].bPressed ? 1 : 0) << "\n";
    }
    out.close();
    LOG->Trace("ReplayManager saved replay to %s", sFilePath.c_str());
    return true;
}

bool ReplayManager::LoadReplay(const RString& sFilePath) {
    m_vLoadedReplay.clear();
    std::ifstream in(sFilePath.c_str());
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string token;
        ReplayInput rin;

        std::getline(ss, token, ',');
        rin.fTime = std::stof(token);

        std::getline(ss, token, ',');
        rin.iColumn = std::stoi(token);

        std::getline(ss, token, ',');
        rin.bPressed = (std::stoi(token) == 1);

        m_vLoadedReplay.push_back(rin);
    }
    in.close();
    LOG->Trace("ReplayManager loaded replay with %d inputs.", (int)m_vLoadedReplay.size());
    return true;
}

const std::vector<ReplayInput>& ReplayManager::GetLoadedReplay() const {
    return m_vLoadedReplay;
}

void ReplayManager::ClearLoadedReplay() {
    m_vLoadedReplay.clear();
}

// ----------------------------------------------------------------------------
// Lua Bindings
// ----------------------------------------------------------------------------
#include "LuaManager.h"
#include "LuaBinding.h"

class LunaReplayManager : public Luna<ReplayManager>
{
public:
    static int LoadReplay(T* p, lua_State* L) {
        RString sPath = SArg(1);
        bool success = p->LoadReplay(sPath);
        lua_pushboolean(L, success);
        return 1;
    }

    static int ClearReplay(T* p, lua_State* L) {
        p->ClearLoadedReplay();
        return 0;
    }

    LunaReplayManager()
    {
        ADD_METHOD(LoadReplay);
        ADD_METHOD(ClearReplay);
    }
};

LUA_REGISTER_CLASS(ReplayManager)
