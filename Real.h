#pragma once
// Real.h

#pragma once

// just for fun ...{
#if defined (_MSC_VER)
#include <vcruntime.h>
#endif
constexpr long Cplusplus85 = 000001L; // pc |:)
constexpr long Cplusplus98 = 199711L;
constexpr long Cplusplus11 = 201103L;
constexpr long Cplusplus14 = 201402L;
constexpr long Cplusplus17 = 201703L;
constexpr long Cplusplus20 = 202002L;
constexpr long Cplusplus23 = 202100L;
#if defined (_MSVC_LANG) && _MSVC_LANG > __cplusplus
#if defined (_HAS_CXX23) && _HAS_CXX23
constexpr long CplusplusVer = Cplusplus23;
#elif defined (_HAS_CXX20) && _HAS_CXX20
constexpr long CplusplusVer = Cplusplus20;
#elif defined (_HAS_CXX17) && _HAS_CXX17
constexpr long CplusplusVer = Cplusplus17;
#else
constexpr long CplusplusVer = _MSVC_LANG;
#endif
#else
constexpr long CplusplusVer = __cplusplus;
#endif

#if (CplusplusVer >= Cplusplus20)
#include    <numbers>
#endif
// }... end of just for fun

#include	<cmath>
#include    <cstdint>

namespace Nessie {

    constexpr double    Eps = 1.E-5;
    constexpr double    SmallEps = 1.E-7;
    constexpr double    SmallerEps = 1.E-12;

#if (CplusplusVer >= Cplusplus20)
    constexpr double    Pi = std::numbers::pi; // 3.141592653589793
#else
    constexpr double    Pi = 3.14159265358979323846;
#endif

    constexpr double    Deg2Rad = Pi / 180.0;
    constexpr double    Ang60InRad = Pi / 3.0;

    // Templates
    template <typename T> inline bool IsPositive (const T   & a,
                                                  const T   & tolerance)
    {
        return (a > tolerance);
    }

    inline bool IsPositive (double a)
    {
        return IsPositive(a, Eps);
    }

    template <typename T> inline bool IsSmallEnough (const T& a,
                                                     const T& tolerance)
    {
        return (a < tolerance && a > -tolerance);
    }

    inline bool IsSmallEnough (double a)
    {
        return IsSmallEnough(a, SmallEps);
    }

    template <typename T> inline bool IsNear (const T& a,
                                              const T& b,
                                              const T& tolerance)
    {
        return (a + tolerance > b && a < b + tolerance);	/* tolerance should be > 0 */
    }

    template <typename T> inline bool IsNearZero (const T a,
                                                  const T tolerance)
    {
        return (a + tolerance > 0 && a < tolerance);
    }

    inline bool IsNear (const double	a,
                        const double	b)
    {
        return IsNear(a, b, Eps);
    }

    inline bool IsNearZero (const double a)
    {
        return IsNearZero(a, Eps);
    }

    template <typename T> inline bool IsNotTooSmall (const T& a,
                                                     const T& tolerance)
    {
        return (a <= -tolerance || a >= tolerance);	/* tolerance should be > 0 */
    }

    inline bool IsNotTooSmall (const double a)
    {
        return IsNotTooSmall(a, SmallEps);
    }

    template <typename T> inline bool IsNotNear (const T& a,
                                                 const T& b,
                                                 const T& tolerance)
    {
        return (a + tolerance <= b || a >= b + tolerance);	/* c should be > 0 */
    }

    inline bool IsNotNear (const double	a,
                           const double	b)
    {
        return IsNotNear(a, b, Eps);
    }

} // namespace Nessie
