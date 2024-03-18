// Real.h

#pragma once

#include	<cmath>
#include    <cstdint>

namespace Nessie {

    inline void WASSERT (bool  a)
    {
        (void)(a); // You may want to put a conditional breakpiont here...
    }


    constexpr double    Eps = 1.E-5;
    constexpr double    SmallEps = 1.E-7;
    constexpr double    SmallerEps = 1.E-12;
    constexpr double    Pi = 3.14159265358979323846;
    constexpr double    Deg2Rad = Pi / 180.0;
    constexpr double    Ang60InRad = Pi / 3.0;

    // Templates
    template <typename T> inline bool IsPositive (const T   & a,
                                                  const T   & tolerance)
    {
        WASSERT(tolerance > 0);
        return (a > tolerance);
    }

    inline bool IsPositive (double a)
    {
        return IsPositive(a, Eps);
    }

    template <typename T> inline bool IsSmallEnough (const T& a,
                                                     const T& tolerance)
    {
        WASSERT(tolerance > 0);
        return (a < tolerance && a + tolerance > 0);
    }

    inline bool IsSmallEnough (double a)
    {
        return IsSmallEnough(a, SmallEps);
    }

    template <typename T> inline bool IsNear (const T& a,
                                              const T& b,
                                              const T& tolerance)
    {
        WASSERT(tolerance > 0);
        return (a + tolerance > b && a < b + tolerance);
    }

    template <typename T> inline bool IsNearZero (const T a,
                                                  const T tolerance)
    {
        return IsSmallEnough(a, tolerance);
    }

    inline bool IsNear (const double	a,
                        const double	b)
    {
        return IsNear(a, b, Eps);
    }

    inline bool IsNearZero (const double a)
    {
        return IsSmallEnough(a, Eps);
    }

    template <typename T> inline bool IsNotTooSmall (const T& a,
                                                     const T& tolerance)
    {
        WASSERT(tolerance > 0);
        return (a <= -tolerance || a >= tolerance);
    }

    inline bool IsNotTooSmall (const double a)
    {
        return IsNotTooSmall(a, SmallEps);
    }

    template <typename T> inline bool IsNotNear (const T& a,
                                                 const T& b,
                                                 const T& tolerance)
    {
        WASSERT(tolerance > 0);
        return (a + tolerance <= b || a >= b + tolerance);
    }

    inline bool IsNotNear (const double	a,
                           const double	b)
    {
        return IsNotNear(a, b, Eps);
    }

} // namespace Nessie
