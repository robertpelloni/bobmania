#ifndef SCORE_KEEPER_UNIFIED_H
#define SCORE_KEEPER_UNIFIED_H

#include "global.h"

// Note: In the real engine, this inherits from ScoreKeeper.
// Here we stub the inheritance for independent compilation.
class ScoreKeeperUnified
{
public:
    ScoreKeeperUnified();
    ~ScoreKeeperUnified();

    // Replaces ScoreKeeperNormal::HandleTapScore
    void HandleTapScore(float fNoteOffset, float fJudgeScale);

    // Replaces ScoreKeeperNormal::HandleHoldScore
    void HandleHoldScore(bool bHeld);

    // Returns the calculated percentage (0.0 to 1.0)
    float GetWifeScore() const;

    // Returns total max possible points for current notes seen
    float GetMaxWifeScore() const;

private:
    float m_fTotalWifeScore;
    float m_fTotalMaxWifeScore;

    // The core Wife3 mathematical approximation
    float CalculateWife3(float fOffset, float fJudgeScale);
};

#endif
