#ifndef WIFE_SCORING_H
#define WIFE_SCORING_H

namespace WifeScoring
{
    // Implementation of Wife3 (J4) Scoring Curve
    // fSeconds is the deviation in seconds (e.g. 0.045 for 45ms)
    // Returns a score between 0.0 and 2.0 (2.0 = Perfect)
    // Misses should be handled by the caller (typically assigning -8.0)
    float CalculateWife3( float fSeconds );
}

#endif
