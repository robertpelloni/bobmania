/*
 * This can be used in two ways: as a timestamp or as a timer.
 *
 * As a timer,
 * RageTimer Timer;
 * for(;;) {
 *   printf( "Will be approximately: %f", Timer.PeekDeltaTime()) ;
 *   float fDeltaTime = Timer.GetDeltaTime();
 * }
 *
 * or as a timestamp:
 * void foo( RageTimer &timestamp ) {
 *     if( timestamp.IsZero() )
 *         printf( "The timestamp isn't set." );
 *     else
 *         printf( "The timestamp happened %f ago", timestamp.Ago() );
 *     timestamp.Touch();
 *     printf( "Near zero: %f", timestamp.Age() );
 * }
 */

#include "RageTimer.h"
#include "RageUtil.h"

#include <chrono>
using namespace std::chrono;


const RageTimer RageZeroTimer(0,0);
static auto g_iStartTime = steady_clock::now();


float RageTimer::GetTimeSinceStart()
{
	duration<float> t = steady_clock::now() - g_iStartTime;
	return t.count();
}

double RageTimer::GetTimeSinceStartDouble()
{
	duration<double> t = steady_clock::now() - g_iStartTime;
	return t.count();
}

std::uint64_t RageTimer::GetMicroseconds() const
{
	steady_clock::duration duration = timestamp.time_since_epoch();
	std::uint64_t usecs = duration_cast<microseconds>(duration).count();
	return usecs;
=======
#include <cmath>
#include <cstdint>

#include "arch/ArchHooks/ArchHooks.h"

// Intialize important variables and definitions
constexpr uint64_t ONE_SECOND_IN_MICROSECONDS_ULL = 1000000ULL;
constexpr int64_t ONE_SECOND_IN_MICROSECONDS_LL = 1000000LL;
constexpr double ONE_SECOND_IN_MICROSECONDS_DBL = 1000000.0;
const RageTimer RageZeroTimer(0, 0);
static const uint64_t g_iStartTime = ArchHooks::GetSystemTimeInMicroseconds();

static inline uint64_t GetTime() noexcept {
  return ArchHooks::GetSystemTimeInMicroseconds();
}

/* The accuracy of RageTimer::GetTimeSinceStart() is directly tied to the
 * stability of the clock sync. Maintaining precision here is crucial. Too
 * much error here will manifest as a drastic shift in the game's sync, and
 * will feel like the global offset suddenly changed. Incorrect math here will
 * manifest as a _consistent_ sync offset in game. Resolution mismatches or
 * values truncated or rounded when they shouldn't be can cause errors when
 * this is calculated and manifest as a _sudden_ drift of sync. Use caution
 * and do thorough testing if you change anything here. -sukibaby */
double RageTimer::GetTimeSinceStart() {
  const uint64_t usecs = (GetTime() - g_iStartTime);
  return static_cast<double>(usecs / ONE_SECOND_IN_MICROSECONDS_DBL);
}

int RageTimer::GetTimeSinceStartSeconds() {
  const uint64_t usecs = (GetTime() - g_iStartTime);
  return static_cast<int>(usecs / ONE_SECOND_IN_MICROSECONDS_ULL);
}

uint64_t RageTimer::GetTimeSinceStartMicroseconds()
{
	return (GetTime() - g_iStartTime);
>>>>>>> release
}

void RageTimer::Touch()
{
	timestamp = steady_clock::now();
}

void RageTimer::Touch() {
  uint64_t usecs = GetTime();

  this->m_secs = uint64_t(usecs / ONE_SECOND_IN_MICROSECONDS_ULL);
  this->m_us = uint64_t(usecs % ONE_SECOND_IN_MICROSECONDS_ULL);
>>>>>>> upstream/release
}

float RageTimer::Ago() const {
  const RageTimer Now;
  return Now - *this;
}

float RageTimer::GetDeltaTime() {
  const RageTimer Now;
  const float diff = Difference(Now, *this);
  *this = Now;
  return diff;
}

/*
 * Get a timer representing half of the time ago as this one.  This is
 * useful for averaging time.  For example,
 *
 * RageTimer tm;
 * ... do stuff ...
 * RageTimer AverageTime = tm.Half();
 * printf( "Something happened approximately %f seconds ago.\n", tm.Ago() );
 */
RageTimer RageTimer::Half() const
{
	const float fProbableDelay = Ago() / 2;
	return *this + fProbableDelay;
}

RageTimer RageTimer::operator+(float tm) const { return Sum(*this, tm); }

float RageTimer::operator-(const RageTimer& rhs) const {
  return Difference(*this, rhs);
}

bool RageTimer::operator<(const RageTimer& rhs) const {
  if (m_secs != rhs.m_secs) {
    return m_secs < rhs.m_secs;
  }
  return m_us < rhs.m_us;
}

bool RageTimer::operator<( const RageTimer &rhs ) const
{
	return timestamp < rhs.timestamp;
}

RageTimer RageTimer::Sum(const RageTimer &lhs, float tm)
{
	const steady_clock::time_point ret =
		lhs.timestamp +
		duration_cast<steady_clock::duration>(duration<float>(tm));
	return RageTimer(ret);

  // Adjust seconds and microseconds if microseconds is negative
  if (us < 0) {
    us += ONE_SECOND_IN_MICROSECONDS_LL;
    --secs;
  }

	// Calculate the sum of the seconds and microseconds
	ret.m_secs = seconds + lhs.m_secs;
	ret.m_us = us + lhs.m_us;

	// Adjust the seconds and microseconds if microseconds is greater than or equal to TIMESTAMP_RESOLUTION
	if (ret.m_us >= ONE_SECOND_IN_MICROSECONDS_ULL)
	{
		ret.m_us -= ONE_SECOND_IN_MICROSECONDS_ULL;
		++ret.m_secs;
	}

	return ret;
>>>>>>> release
}

float RageTimer::Difference(const RageTimer &lhs, const RageTimer &rhs)
{
	const duration<float> diff = lhs.timestamp - rhs.timestamp;
	return diff.count();
}

#include "LuaManager.h"
LuaFunction(GetTimeSinceStart, RageTimer::GetTimeSinceStartDouble())
=======
>>>>>>> Stashed changes

    /*
     * Copyright (c) 2001-2003 Chris Danford, Glenn Maynard
     * All rights reserved.
     *
     * Permission is hereby granted, free of charge, to any person obtaining a
     * copy of this software and associated documentation files (the
     * "Software"), to deal in the Software without restriction, including
     * without limitation the rights to use, copy, modify, merge, publish,
     * distribute, and/or sell copies of the Software, and to permit persons to
     * whom the Software is furnished to do so, provided that the above
     * copyright notice(s) and this permission notice appear in all copies of
     * the Software and that both the above copyright notice(s) and this
     * permission notice appear in supporting documentation.
     *
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
     * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
     * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
     * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
     * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
     * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
     * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
     * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
     * PERFORMANCE OF THIS SOFTWARE.
     */
