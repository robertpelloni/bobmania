#include "global.h"
#include "WifeScoring.h"
#include <cmath>

namespace WifeScoring
{
    // J4 scale constants
    // J4 typically has a Marvelous window around 22.5ms.
    // Using a Gaussian approximation: Score = 2 * exp( - (dev^2) / (2 * sigma^2) )
    // If sigma = 25ms (0.025s), then 2*sigma^2 = 2 * 0.000625 = 0.00125

    const float WIFE3_SIGMA_SQ_X2 = 0.00125f;

    float CalculateWife3( float fSeconds )
    {
        float dev = std::abs(fSeconds);

        // Ensure we don't return garbage for massive deviations (though the formula handles it well)
        // exp(-large) -> 0

        return 2.0f * std::exp( -(dev*dev) / WIFE3_SIGMA_SQ_X2 );
    }
}
