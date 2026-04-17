#include "ScoreKeeperUnified.h"
#include <cmath>

ScoreKeeperUnified::ScoreKeeperUnified()
    : m_fTotalWifeScore(0.0f), m_fTotalMaxWifeScore(0.0f)
{
}

ScoreKeeperUnified::~ScoreKeeperUnified()
{
}

// Wife3 uses a custom erfc-based curve or polynomial approximation
// to smoothly scale scores based on millisecond offsets.
float ScoreKeeperUnified::CalculateWife3(float fOffset, float fJudgeScale)
{
    // Convert to milliseconds and take absolute value
    float ms = std::abs(fOffset * 1000.0f);

    // Scale the window by the Judge scale (J4 is standard 1.0)
    ms /= fJudgeScale;

    // Etterna Wife3 polynomial approximation magic constants
    // These constants form a bell curve where 0ms = 2.0 points,
    // dropping off to 0.0 points at the miss window.
    float max_points = 2.0f;
    float y = 1.0f - std::pow(2.0f, -ms * ms / 2000.0f);

    // Base score calculation
    float score = max_points - (y * 2.5f);

    // Floor it at negative penalty for terrible hits (mines/misses)
    if (score < -8.0f) score = -8.0f;

    return score;
}

void ScoreKeeperUnified::HandleTapScore(float fNoteOffset, float fJudgeScale)
{
    float points = CalculateWife3(fNoteOffset, fJudgeScale);

    m_fTotalWifeScore += points;
    m_fTotalMaxWifeScore += 2.0f; // 2.0 is flawless Wife3 score
}

void ScoreKeeperUnified::HandleHoldScore(bool bHeld)
{
    // Holds give a flat bonus or no penalty in Wife
    if (bHeld) {
        m_fTotalWifeScore += 2.0f;
    }
    m_fTotalMaxWifeScore += 2.0f;
}

float ScoreKeeperUnified::GetWifeScore() const
{
    if (m_fTotalMaxWifeScore == 0.0f) return 0.0f;
    return m_fTotalWifeScore / m_fTotalMaxWifeScore;
}

float ScoreKeeperUnified::GetMaxWifeScore() const
{
    return m_fTotalMaxWifeScore;
}
