#include "global.h"

#include "CubicSpline.h"

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <list>
#include <vector>

#include "RageLog.h"
#include "RageUtil.h"

// Spline solving optimization:
// The tridiagonal part of the system of equations for a spline of size n is
//   the same for all splines of size n.  It's not affected by the positions
//   of the points.
// So spline solving can be split into two parts.  Part 1 solves the
//   tridiagonal and stores the result.  Part 2 takes the solved tridiagonal
//   and applies it to the positions to find the coefficients.
// Part 1 only needs to be done when the number of points changes.  So this
//   could cut solve time for the same number of points substantially.
// Further optimization is to cache the part 1 results for the last 16 spline
//   sizes solved, to reduce the cost of using lots of splines with a small
//   number of sizes.

}

void SplineSolutionCache::add_to_cache(
    std::list<Entry>& cache, std::vector<float>& outd,
    std::vector<float>& outm) {
  if (cache.size() >= solution_cache_limit) {
    cache.pop_back();
  }
  cache.push_front(Entry());
  cache.front().diagonals = outd;
  cache.front().multiples = outm;
}

=======
>>>>>>> Stashed changes
}

void SplineSolutionCache::solve_diagonals_straight(
    std::vector<float>& diagonals, std::vector<float>& multiples) {
  if (find_in_cache(straight_diagonals, diagonals, multiples)) {
    return;
  }

  // Solution steps:
  // Two stages:  First, work downwards, zeroing the 1s below each diagonal.
  // | 2 1 0 0 | -> | 2 1 0 0 | -> | 2 1 0 0 | -> | 2 1 0 0 |
  // | 1 4 1 0 | -> | 0 a 1 0 | -> | 0 d 1 0 | -> | 0 a 1 0 |
  // | 0 1 4 1 | -> | 0 1 4 1 | -> | 0 0 b 1 | -> | 0 0 b 1 |
  // | 0 0 1 2 | -> | 0 0 1 2 | -> | 0 0 1 2 | -> | 0 0 0 c |
  // Second stage:  Work upwards, zeroing the 1s above each diagonal.
  // V
  // | 2 1 0 0 | -> | 2 1 0 0 | -> | 2 0 0 0 |
  // | 0 a 1 0 | -> | 0 a 0 0 | -> | 0 a 0 0 |
  // | 0 0 b 0 | -> | 0 0 b 0 | -> | 0 0 b 0 |
  // | 0 0 0 c | -> | 0 0 0 c | -> | 0 0 0 c |

=======
>>>>>>> Stashed changes
  // Solving finished.
  add_to_cache(straight_diagonals, diagonals, multiples);
}

void SplineSolutionCache::solve_diagonals_looped(
    std::vector<float>& diagonals, std::vector<float>& multiples) {
  if (find_in_cache(looped_diagonals, diagonals, multiples)) {
    return;
  }

  // The steps to solve the system of equations look like this:
  // Stage one:  Zero the 1s below the diagonals.
  // | 4 1 0 0 1 | -> | 4 1 0 0 1 | -> | 4 1 0 0 1 | -> | 4 1 0 0 1 |
  // | 1 4 1 0 0 | -> | 0 a 1 0 u | -> | 0 a 1 0 u | -> | 0 a 1 0 u |
  // | 0 1 4 1 0 | -> | 0 1 4 1 0 | -> | 0 0 b 1 v | -> | 0 0 b 1 v |
  // | 0 0 1 4 1 | -> | 0 0 1 4 1 | -> | 0 0 1 4 1 | -> | 0 0 0 c w |
  // | 1 0 0 1 4 | -> | 1 0 0 1 4 | -> | 1 0 0 1 4 | -> | 1 0 0 1 4 |
  // V
  // | 4 1 0 0 1 |
  // | 0 a 1 0 u |
  // | 0 0 b 1 v |
  // | 0 0 0 c w |
  // | 1 0 0 0 d |
  // The top of the right column is left unzeroed because it will be changed
  // by stage two, nullifying the effect of zeroing it.
  // V Stage two:  Zero the 1s above the diagonals, starting with the second
  //   to last row to avoid carrying effects across the left column.
  // | 4 1 0 0 1 | -> | 4 1 0 0 1 | -> | 4 0 0 0 z | -> | 4 0 0 0 z |
  // | 0 a 1 0 u | -> | 0 a 0 0 y | -> | 0 a 0 0 y | -> | 0 a 0 0 y |
  // | 0 0 b 0 x | -> | 0 0 b 0 x | -> | 0 0 b 0 x | -> | 0 0 b 0 x |
  // | 0 0 0 c w | -> | 0 0 0 c w | -> | 0 0 0 c w | -> | 0 0 0 c w |
  // | 1 0 0 0 d | -> | 1 0 0 0 d | -> | 1 0 0 0 d | -> | 0 0 0 0 f |
  // V Stage three:  Zero the right column.
  // | 4 0 0 0 0 | -> | 4 0 0 0 0 | -> | 4 0 0 0 0 | -> | 4 0 0 0 0 |
  // | 0 a 0 0 y | -> | 0 a 0 0 0 | -> | 0 a 0 0 0 | -> | 0 a 0 0 0 |
  // | 0 0 b 0 x | -> | 0 0 b 0 x | -> | 0 0 b 0 0 | -> | 0 0 b 0 0 |
  // | 0 0 0 c w | -> | 0 0 0 c w | -> | 0 0 0 c w | -> | 0 0 0 c 0 |
  // | 0 0 0 0 f | -> | 0 0 0 0 f | -> | 0 0 0 0 f | -> | 0 0 0 0 f |

void CubicSpline::solve_looped() {
  if (check_minimum_size()) {
    return;
  }
  std::size_t last = m_points.size();
  std::vector<float> results(m_points.size());
  std::vector<float> diagonals(m_points.size());
  std::vector<float> multiples;
  solution_cache.solve_diagonals_looped(diagonals, multiples);
  results[0] = 3 * loop_space_difference(
                       m_points[1].a, m_points[last - 1].a, m_spatial_extent);
  prep_inner(last, results);
  results[last - 1] =
      3 * loop_space_difference(
              m_points[0].a, m_points[last - 2].a, m_spatial_extent);

  // Steps explained in detail in SplineSolutionCache.
  // Only the operations on the results column are performed here.
  // Stage one.
  // SplineSolutionCache's Stage one loop ends at last-2 because it has to
  // handle right_column.  This does not handle right_column, so the loop
  // goes to last-1.
  for (std::size_t i = 0; i < last - 1; ++i) {
    // Operation: Add row[i] * -multiples[i] to row[i+1].
    results[i + 1] -= results[i] * multiples[i];
  }
  std::size_t next_mult = last - 1;
  // Stage two.
  for (std::size_t i = last - 2; i > 0; --i) {
    // Operation: Add row[i] * -multiples[nm] to row[i-1].
    results[i - 1] -= results[i] * multiples[next_mult];
    ++next_mult;
  }
  // Last step of stage two.
  // Operation: Add row[0] * -multiples[nm] to row[l-1].
  results[last - 1] -= results[0] * multiples[next_mult];
  ++next_mult;
  // Stage three.
  const std::size_t end = last - 1;
  for (std::size_t i = 0; i < end; ++i) {
    // Operation: Add row[e] * -multiples[nm] to row[i].
    results[i] -= results[end] * multiples[next_mult];
    ++next_mult;
  }
  // Solving finished.
  set_results(last, diagonals, results);
}

void CubicSpline::solve_straight() {
  if (check_minimum_size()) {
    return;
  }
  std::size_t last = m_points.size();
  std::vector<float> results(m_points.size());
  std::vector<float> diagonals(m_points.size());
  std::vector<float> multiples;
  solution_cache.solve_diagonals_straight(diagonals, multiples);
  results[0] = 3 * (m_points[1].a - m_points[0].a);
  prep_inner(last, results);
  results[last - 1] =
      3 * loop_space_difference(
              m_points[last - 1].a, m_points[last - 2].a, m_spatial_extent);

  // Steps explained in detail in SplineSolutionCache.
  // Only the operations on the results column are performed here.
  // Stage one.
  for (std::size_t i = 0; i < last - 1; ++i) {
    // Operation: Add row[i] * -multiples[i] to row[i+1].
    results[i + 1] -= results[i] * multiples[i];
  }
  std::size_t next_mult = last - 1;
  // Stage two.
  for (std::size_t i = last - 1; i > 0; --i) {
    // Operation: Add row[i] * -multiples[nm] to row [i-1].
    results[i - 1] -= results[i] * multiples[next_mult];
    ++next_mult;
  }
  // Solving finished.
  set_results(last, diagonals, results);
}

void CubicSpline::solve_polygonal() {
  if (check_minimum_size()) {
    return;
  }
  std::size_t last = m_points.size() - 1;
  for (std::size_t i = 0; i < last; ++i) {
    m_points[i].b = loop_space_difference(
        m_points[i + 1].a, m_points[i].a, m_spatial_extent);
  }
  m_points[last].b =
      loop_space_difference(m_points[0].a, m_points[last].a, m_spatial_extent);
}

bool CubicSpline::check_minimum_size() {
  std::size_t last = m_points.size();
  if (last < 2) {
    m_points[0].b = m_points[0].c = m_points[0].d = 0.0f;
    return true;
  }
  if (last == 2) {
    m_points[0].b =
        loop_space_difference(m_points[1].a, m_points[0].a, m_spatial_extent);
    m_points[0].c = m_points[0].d = 0.0f;
    // These will be used in the looping case.
    m_points[1].b =
        loop_space_difference(m_points[0].a, m_points[1].a, m_spatial_extent);
    m_points[1].c = m_points[1].d = 0.0f;
    return true;
  }
  float a = m_points[0].a;
  bool all_points_identical = true;
  for (std::size_t i = 0; i < m_points.size(); ++i) {
    m_points[i].b = m_points[i].c = m_points[i].d = 0.0f;
    if (m_points[i].a != a) {
      all_points_identical = false;
    }
  }
  return all_points_identical;
}

void CubicSpline::prep_inner(std::size_t last, std::vector<float>& results) {
  for (std::size_t i = 1; i < last - 1; ++i) {
    results[i] =
        3 * loop_space_difference(
                m_points[i + 1].a, m_points[i - 1].a, m_spatial_extent);
  }
}

void CubicSpline::set_results(
    std::size_t last, std::vector<float>& diagonals,
    std::vector<float>& results) {
  // No more operations left, everything not a diagonal should be zero now.
  for (std::size_t i = 0; i < last; ++i) {
    results[i] /= diagonals[i];
  }
  // Now we can go through and set the b, c, d values of each point.
  // b, c, d values of the last point are not set because they are unused.
  for (std::size_t i = 0; i < last; ++i) {
    std::size_t next = (i + 1) % last;
    float diff = loop_space_difference(
        m_points[next].a, m_points[i].a, m_spatial_extent);
    m_points[i].b = results[i];
    m_points[i].c = (3 * diff) - (2 * results[i]) - results[next];
    m_points[i].d = (2 * -diff) + results[i] + results[next];
#define UNNAN(n) \
  if (n != n) {  \
    n = 0.0f;    \
  }
    UNNAN(m_points[i].b);
    UNNAN(m_points[i].c);
    UNNAN(m_points[i].d);

  // Steps explained in detail in SplineSolutionCache.
  // Only the operations on the results column are performed here.
  // Stage one.
  // SplineSolutionCache's Stage one loop ends at last-2 because it has to
  // handle right_column.  This does not handle right_column, so the loop
  // goes to last-1.
  for (size_t i = 0; i < last - 1; ++i) {
    // Operation: Add row[i] * -multiples[i] to row[i+1].
    results[i + 1] -= results[i] * multiples[i];
  }
  size_t next_mult = last - 1;
  // Stage two.
  for (size_t i = last - 2; i > 0; --i) {
    // Operation: Add row[i] * -multiples[nm] to row[i-1].
    results[i - 1] -= results[i] * multiples[next_mult];
    ++next_mult;
  }
  // Last step of stage two.
  // Operation: Add row[0] * -multiples[nm] to row[l-1].
  results[last - 1] -= results[0] * multiples[next_mult];
  ++next_mult;
  // Stage three.
  const size_t end = last - 1;
  for (size_t i = 0; i < end; ++i) {
    // Operation: Add row[e] * -multiples[nm] to row[i].
    results[i] -= results[end] * multiples[next_mult];
    ++next_mult;
  }
  // Solving finished.
  set_results(last, diagonals, results);
}

void CubicSpline::solve_straight() {
  if (check_minimum_size()) {
    return;
  }
  size_t last = m_points.size();
  std::vector<float> results(m_points.size());
  std::vector<float> diagonals(m_points.size());
  std::vector<float> multiples;
  solution_cache.solve_diagonals_straight(diagonals, multiples);
  results[0] = 3 * (m_points[1].a - m_points[0].a);
  prep_inner(last, results);
  results[last - 1] =
      3 * loop_space_difference(
              m_points[last - 1].a, m_points[last - 2].a, m_spatial_extent);

  // Steps explained in detail in SplineSolutionCache.
  // Only the operations on the results column are performed here.
  // Stage one.
  for (size_t i = 0; i < last - 1; ++i) {
    // Operation: Add row[i] * -multiples[i] to row[i+1].
    results[i + 1] -= results[i] * multiples[i];
  }
  size_t next_mult = last - 1;
  // Stage two.
  for (size_t i = last - 1; i > 0; --i) {
    // Operation: Add row[i] * -multiples[nm] to row [i-1].
    results[i - 1] -= results[i] * multiples[next_mult];
    ++next_mult;
  }
  // Solving finished.
  set_results(last, diagonals, results);
}

void CubicSpline::solve_polygonal() {
  if (check_minimum_size()) {
    return;
  }
  size_t last = m_points.size() - 1;
  for (size_t i = 0; i < last; ++i) {
    m_points[i].b = loop_space_difference(
        m_points[i + 1].a, m_points[i].a, m_spatial_extent);
  }
  m_points[last].b =
      loop_space_difference(m_points[0].a, m_points[last].a, m_spatial_extent);
}

bool CubicSpline::check_minimum_size() {
  size_t last = m_points.size();
  if (last < 2) {
    m_points[0].b = m_points[0].c = m_points[0].d = 0.0f;
    return true;
  }
  if (last == 2) {
    m_points[0].b =
        loop_space_difference(m_points[1].a, m_points[0].a, m_spatial_extent);
    m_points[0].c = m_points[0].d = 0.0f;
    // These will be used in the looping case.
    m_points[1].b =
        loop_space_difference(m_points[0].a, m_points[1].a, m_spatial_extent);
    m_points[1].c = m_points[1].d = 0.0f;
    return true;
  }
  float a = m_points[0].a;
  bool all_points_identical = true;
  for (size_t i = 0; i < m_points.size(); ++i) {
    m_points[i].b = m_points[i].c = m_points[i].d = 0.0f;
    if (m_points[i].a != a) {
      all_points_identical = false;
    }
  }
  return all_points_identical;
}

void CubicSpline::prep_inner(size_t last, std::vector<float>& results) {
  for (size_t i = 1; i < last - 1; ++i) {
    results[i] =
        3 * loop_space_difference(
                m_points[i + 1].a, m_points[i - 1].a, m_spatial_extent);
  }
}

void CubicSpline::set_results(size_t last, std::vector<float>& diagonals, std::vector<float>& results)
{
	// No more operations left, everything not a diagonal should be zero now.
	for(size_t i= 0; i < last; ++i)
	{
		results[i]/= diagonals[i];
	}
	// Now we can go through and set the b, c, d values of each point.
	// b, c, d values of the last point are not set because they are unused.
	for(size_t i= 0; i < last; ++i)
	{
		size_t next= (i+1) % last;
		float diff= loop_space_difference(
			m_points[next].a, m_points[i].a, m_spatial_extent);
		m_points[i].b= results[i];
		m_points[i].c= (3 * diff) - (2 * results[i]) - results[next];
		m_points[i].d= (2 * -diff) + results[i] + results[next];
#define UNNAN(n) if(n != n) { n = 0.0f; }
		UNNAN(m_points[i].b);
		UNNAN(m_points[i].c);
		UNNAN(m_points[i].d);
>>>>>>> release
  if (loop) {
    float max_t = static_cast<float>(m_points.size());
    t = std::fmod(t, max_t);
    if (t < 0.0f) {
      t += max_t;
    }
    p = static_cast<std::size_t>(t);
    tfrac = t - static_cast<float>(p);
  } else {
    int flort = static_cast<int>(t);
    if (flort < 0) {
      p = 0;
      tfrac = 0;
    } else if (static_cast<std::size_t>(flort) >= m_points.size() - 1) {
      p = m_points.size() - 1;
      tfrac = 0;
    } else {
      p = static_cast<std::size_t>(flort);
      tfrac = t - static_cast<float>(p);
    }
  }
=======
    } else if (static_cast<size_t>(flort) >= m_points.size() - 1) {
      p = m_points.size() - 1;
      tfrac = 0;
    } else {
      p = static_cast<size_t>(flort);
      tfrac = t - static_cast<float>(p);
    }
  }
>>>>>>> upstream/release
=======
  std::size_t p = 0;        \
  float tfrac = 0.0f;       \
  p_and_tfrac_from_t(t, loop, p, tfrac);
>>>>>>> Stashed changes

float CubicSpline::evaluate(float t, bool loop) const {
  RETURN_IF_EMPTY;
  DECLARE_P_AND_TFRAC;
  float tsq = tfrac * tfrac;
  float tcub = tsq * tfrac;
  return m_points[p].a + (m_points[p].b * tfrac) + (m_points[p].c * tsq) +
         (m_points[p].d * tcub);
}

float CubicSpline::evaluate_derivative(float t, bool loop) const {
  RETURN_IF_EMPTY;
  DECLARE_P_AND_TFRAC;
  float tsq = tfrac * tfrac;
  return m_points[p].b + (2.0f * m_points[p].c * tfrac) +
         (3.0f * m_points[p].d * tsq);
}

float CubicSpline::evaluate_second_derivative(float t, bool loop) const {
  RETURN_IF_EMPTY;
  DECLARE_P_AND_TFRAC;
  return (2.0f * m_points[p].c) + (6.0f * m_points[p].d * tfrac);
}

float CubicSpline::evaluate_third_derivative(float t, bool loop) const {
  RETURN_IF_EMPTY;
  DECLARE_P_AND_TFRAC;
  return 6.0f * m_points[p].d;
}

#undef RETURN_IF_EMPTY
#undef DECLARE_P_AND_TFRAC

=======
  // Behavior for splines of different sizes:  Use a size between the two.
  // Points that exist in both will be averaged.
  // Points that only exist in one will come only from that one.
  const size_t from_size = from.size();
  const size_t to_size = to.size();
  size_t out_size = to_size;
  size_t limit = to_size;
  if (from_size < to_size) {
    out_size =
        from_size +
        static_cast<size_t>(static_cast<float>(to_size - from_size) * between);
  } else if (to_size < from_size) {
    limit = from_size;
    out_size = to_size + static_cast<size_t>(
                             static_cast<float>(from_size - to_size) * between);
  }
  rage_clamp(out_size, 0, limit);
  out.resize(out_size);

  for (size_t spli = 0; spli < out.m_splines.size(); ++spli) {
    for (size_t p = 0; p < out_size; ++p) {
      float fc[4] = {0.0f, 0.0f, 0.0f, 0.0f};
      float tc[4] = {0.0f, 0.0f, 0.0f, 0.0f};
      if (p < from_size) {
        from.m_splines[spli].get_point_and_coefficients(
            p, fc[0], fc[1], fc[2], fc[3]);
      }
      if (p < to_size) {
        to.m_splines[spli].get_point_and_coefficients(
            p, tc[0], tc[1], tc[2], tc[3]);
      } else {
        for (int i = 0; i < 4; ++i) {
          tc[i] = fc[i];
        }
      }
      if (p >= from_size) {
        for (int i = 0; i < 4; ++i) {
          fc[i] = tc[i];
        }
      }
      float oc[4] = {0.0f, 0.0f, 0.0f, 0.0f};
      for (int i = 0; i < 4; ++i) {
        oc[i] = lerp(between, fc[i], tc[i]);
      }
      out.m_splines[spli].set_point_and_coefficients(
          p, oc[0], oc[1], oc[2], oc[3]);
    }
  }
  // The spline is not solved after averaging because my testing showed that
  // it is unnecessary.
  // My testing method was this:
  // Spline A is generated by lerping all points and coefficients.
  // Spline B is generated by lerping all points then solving.
  // The coefficients for Spline A and Spline B are identical to 5 to 9
  // significant digits.  Thus, solving is unnecessary.
  // Additionally, solving would require a mechanism to disable solving for
  // the people that wish to set their own coefficients instead of solving.
  // -Kyz
>>>>>>> upstream/release
=======
>>>>>>> Stashed changes
}

void CubicSplineN::solve() {
  if (!m_dirty) {
    return;
  }
#define SOLVE_LOOP(solvent)                                \
  for (spline_cont_t::iterator spline = m_splines.begin(); \
       spline != m_splines.end(); ++spline) {              \
    spline->solvent();                                     \
  }
  if (m_polygonal) {
    SOLVE_LOOP(solve_polygonal);
  } else {
    if (m_loop) {
      SOLVE_LOOP(solve_looped);
    } else {
      SOLVE_LOOP(solve_straight);
    }
  }
#undef SOLVE_LOOP
  m_dirty = false;
}

#define CSN_EVAL_SOMETHING(something)                                  \
  void CubicSplineN::something(float t, std::vector<float>& v) const { \
    for (spline_cont_t::const_iterator spline = m_splines.begin();     \
         spline != m_splines.end(); ++spline) {                        \
      v.push_back(spline->something(t, m_loop));                       \
    }                                                                  \
  }

CSN_EVAL_SOMETHING(evaluate);
CSN_EVAL_SOMETHING(evaluate_derivative);
CSN_EVAL_SOMETHING(evaluate_second_derivative);
CSN_EVAL_SOMETHING(evaluate_third_derivative);

#undef CSN_EVAL_SOMETHING

#define CSN_EVAL_RV_SOMETHING(something)                        \
  void CubicSplineN::something(float t, RageVector3& v) const { \
    ASSERT(m_splines.size() == 3);                              \
    v.x = m_splines[0].something(t, m_loop);                    \
    v.y = m_splines[1].something(t, m_loop);                    \
    v.z = m_splines[2].something(t, m_loop);                    \
  }

CSN_EVAL_RV_SOMETHING(evaluate);
CSN_EVAL_RV_SOMETHING(evaluate_derivative);

#undef CSN_EVAL_RV_SOMETHING

=======
void CubicSplineN::redimension(size_t d) {
  m_splines.resize(d);
  m_dirty = true;
}

size_t CubicSplineN::dimension() const { return m_splines.size(); }
>>>>>>> upstream/release
=======
>>>>>>> Stashed changes

// m_dirty is set before the member so that the set_dirty that is created
// can actually be used to set the dirty flag. -Kyz
#define SET_GET_MEM(member, name)         \
  void CubicSplineN::set_##name(bool b) { \
    m_dirty = true;                       \
    member = b;                           \
  }                                       \
  bool CubicSplineN::get_##name() const { return member; }

SET_GET_MEM(m_loop, loop);
SET_GET_MEM(m_polygonal, polygonal);
SET_GET_MEM(m_dirty, dirty);

#undef SET_GET_MEM

#include "LuaBinding.h"

=======
>>>>>>> Stashed changes
};
LUA_REGISTER_CLASS(CubicSplineN);

int LuaFunc_create_spline(lua_State* L);
int LuaFunc_create_spline(lua_State* L) {
  CubicSplineN* spline = new CubicSplineN;
  spline->PushSelf(L);
  return 1;
}
LUAFUNC_REGISTER_COMMON(create_spline);

// Side note:  Actually written between 2014/12/26 and 2014/12/28
/*
 * Copyright (c) 2014-2015 Eric Reese
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
