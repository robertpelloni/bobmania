#ifndef NOTE_PATH_H
#define NOTE_PATH_H

#include "global.h"
#include "RageTypes.h"
#include <vector>

/**
 * Unified StepMania - NotITG Parity
 * NotePath encapsulates a Catmull-Rom or Cubic spline that dictates
 * exactly where an arrow should be drawn on the screen based on its
 * current YOffset (distance from the receptor).
 */
class NotePath
{
public:
    NotePath();
    ~NotePath();

    // Clears the current spline and resets to a straight vertical line
    void Clear();

    // Adds a control point to the spline.
    // yOffset: distance from receptor.
    // posX, posY, posZ: 3D coordinate offsets for the arrow.
    // rotX, rotY, rotZ: 3D rotation offsets for the arrow.
    void AddControlPoint(float yOffset, float posX, float posY, float posZ, float rotX, float rotY, float rotZ);

    // Finalizes the spline generation. Must be called after adding points.
    void BuildSpline();

    // Evaluates the spline at a specific YOffset.
    // Outputs the interpolated 3D position and rotation.
    void Evaluate(float yOffset, RageVector3& posOut, RageVector3& rotOut) const;

    // Checks if a custom path is active (otherwise fast-path straight rendering)
    bool IsActive() const;

public:
    struct ControlPoint {
        float yOffset;
        RageVector3 pos;
        RageVector3 rot;
    };

    std::vector<ControlPoint> m_vPoints;
    bool m_bActive;

    // Interpolation helper
    float Interpolate(float t, float p0, float p1, float p2, float p3) const;
};

#endif
