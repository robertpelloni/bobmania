#ifndef REPLAY_MANAGER_H
#define REPLAY_MANAGER_H

#include "global.h"
#include <vector>

struct ReplayInput {
    float fTime;     // Exact timestamp of input
    int iColumn;     // Which column (0-3 usually)
    bool bPressed;   // True if pressed, false if released
};

class ReplayManager
{
public:
    ReplayManager();
    ~ReplayManager();

    void Init();
    void Shutdown();

    void StartRecording();
    void StopRecording();
    void RecordInput(float fTime, int iColumn, bool bPressed);
    bool SaveReplay(const RString& sFilePath);

    bool LoadReplay(const RString& sFilePath);
    const std::vector<ReplayInput>& GetLoadedReplay() const;
    void ClearLoadedReplay();

private:
    bool m_bRecording;
    std::vector<ReplayInput> m_vActiveRecording;
    std::vector<ReplayInput> m_vLoadedReplay;
};

extern ReplayManager* REPLAYMAN;

#endif
