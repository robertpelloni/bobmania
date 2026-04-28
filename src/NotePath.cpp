#include "NotePath.h"
#include "RageMath.h"
#include <algorithm>

NotePath::NotePath() : m_bActive(false) {}

NotePath::~NotePath() {}

void NotePath::Clear() {
    m_vPoints.clear();
    m_bActive = false;
}

void NotePath::AddControlPoint(float yOffset, float posX, float posY, float posZ, float rotX, float rotY, float rotZ) {
    ControlPoint cp;
    cp.yOffset = yOffset;
    cp.pos = RageVector3(posX, posY, posZ);
    cp.rot = RageVector3(rotX, rotY, rotZ);
    m_vPoints.push_back(cp);
}

bool ComparePoints(const NotePath::ControlPoint& a, const NotePath::ControlPoint& b) {
    return a.yOffset < b.yOffset;
}

void NotePath::BuildSpline() {
    if (m_vPoints.size() < 2) {
        m_bActive = false;
        return;
    }

    // Sort points by YOffset to ensure correct spline evaluation
    std::sort(m_vPoints.begin(), m_vPoints.end(), ComparePoints);
    m_bActive = true;
}

// Catmull-Rom Spline Interpolation
float NotePath::Interpolate(float t, float p0, float p1, float p2, float p3) const {
    float t2 = t * t;
    float t3 = t2 * t;
    return 0.5f * (
        (2.0f * p1) +
        (-p0 + p2) * t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
    );
}

void NotePath::Evaluate(float yOffset, RageVector3& posOut, RageVector3& rotOut) const {
    if (!m_bActive || m_vPoints.empty()) {
        posOut = RageVector3(0, yOffset, 0);
        rotOut = RageVector3(0, 0, 0);
        return;
    }

    // Edge case: before first point
    if (yOffset <= m_vPoints.front().yOffset) {
        posOut = m_vPoints.front().pos;
        rotOut = m_vPoints.front().rot;
        return;
    }

    // Edge case: after last point
    if (yOffset >= m_vPoints.back().yOffset) {
        posOut = m_vPoints.back().pos;
        rotOut = m_vPoints.back().rot;
        return;
    }

    // Find the segment we are in
    size_t idx = 0;
    for (size_t i = 0; i < m_vPoints.size() - 1; ++i) {
        if (yOffset >= m_vPoints[i].yOffset && yOffset < m_vPoints[i+1].yOffset) {
            idx = i;
            break;
        }
    }

    // Get 4 control points for Catmull-Rom
    const ControlPoint& p1 = m_vPoints[idx];
    const ControlPoint& p2 = m_vPoints[idx + 1];

    // Duplicate edge points if we are at the boundaries
    const ControlPoint& p0 = (idx > 0) ? m_vPoints[idx - 1] : p1;
    const ControlPoint& p3 = (idx + 2 < m_vPoints.size()) ? m_vPoints[idx + 2] : p2;

    // Calculate normalized t (0.0 to 1.0) inside this segment
    float range = p2.yOffset - p1.yOffset;
    float t = (range > 0.001f) ? (yOffset - p1.yOffset) / range : 0.0f;

    posOut.x = Interpolate(t, p0.pos.x, p1.pos.x, p2.pos.x, p3.pos.x);
    posOut.y = Interpolate(t, p0.pos.y, p1.pos.y, p2.pos.y, p3.pos.y);
    posOut.z = Interpolate(t, p0.pos.z, p1.pos.z, p2.pos.z, p3.pos.z);

    rotOut.x = Interpolate(t, p0.rot.x, p1.rot.x, p2.rot.x, p3.rot.x);
    rotOut.y = Interpolate(t, p0.rot.y, p1.rot.y, p2.rot.y, p3.rot.y);
    rotOut.z = Interpolate(t, p0.rot.z, p1.rot.z, p2.rot.z, p3.rot.z);
}

bool NotePath::IsActive() const {
    return m_bActive;
}

// ----------------------------------------------------------------------------
// Lua Bindings
// ----------------------------------------------------------------------------
#include "LuaManager.h"
#include "LuaBinding.h"

class LunaNotePath : public Luna<NotePath>
{
public:
    static int Clear(NotePath* p, lua_State* L) {
        p->Clear();
        return 0;
    }

    static int AddControlPoint(NotePath* p, lua_State* L) {
        float yOff = FArg(1);
        float px = FArg(2); float py = FArg(3); float pz = FArg(4);
        float rx = FArg(5); float ry = FArg(6); float rz = FArg(7);
        p->AddControlPoint(yOff, px, py, pz, rx, ry, rz);
        return 0;
    }

    static int BuildSpline(NotePath* p, lua_State* L) {
        p->BuildSpline();
        return 0;
    }

    LunaNotePath()
    {
        ADD_METHOD(Clear);
        ADD_METHOD(AddControlPoint);
        ADD_METHOD(BuildSpline);
    }
};

LUA_REGISTER_CLASS(NotePath)
