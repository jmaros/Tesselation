#pragma once
// P3Vector.h

#pragma once

#include	<cmath>
#include    <cstdint>

namespace Nessie {

    constexpr double	Eps = 1.E-5;
    constexpr double	SmallEps = 1.E-7;
    constexpr double	SmallerEps = 1.E-12;
    constexpr double	Pi = 3.14159265358979323846;
    constexpr double	Deg2Rad = Pi / 180.0;
    constexpr double	Ang60InRad = Pi / 3.0;

    inline void WASSERT (bool  a)
    {
        (void)(a); // You may want to put a conditional breakpiont here...
    }

    inline bool IsPositive (double v, double tolerance = Eps)
    {
        return (v > tolerance);
    }

    inline double SmallFilter (double	a)
    {
#define DEBUG_SMALL
#if defined (DEBUG_SMALL)
        if (fabs (a) < SmallerEps) {
            a = 0.0;
        }
#endif
        return a;
    }

    // Templates
    template <typename T> inline bool IsSmallEnough (const T& a)
    {
        return (a < SmallEps && a > -SmallEps);
    }

    template <typename T> inline bool IsSmallEnough (const T& a,
                                                     const T& b)
    {
        return (a < b && a > (-b));
    }

    inline bool IsNear (const double	a,
                        const double	b)
    {
        return (a + Eps > b && a < b + Eps);
    }

    inline bool IsNearZero (const double a,
                            const double tolerance = Eps)
    {
        return (a + tolerance > 0 && a < tolerance);
    }

    template <typename T> inline bool IsNear (const T& a,
                                              const T& b,
                                              const T& c)
    {
        return (a + c > b && a < b + c);	/* c should be > 0 */
    }

    template <typename T> inline bool IsNotTooSmall (const T& a)
    {
        return (a <= (-SmallEps) || a >= SmallEps);
    }

    template <typename T> inline bool IsNotTooSmall (const T& a,
                                                     const T& c)
    {
        return (a <= (-c) || a >= c);	/* c should be > 0 */
    }

    inline bool IsNotNear (const double	a,
                           const double	b)
    {
        return (a + Eps <= b || a >= b + Eps);
    }

    template <typename T> inline bool IsNotNear (const T& a,
                                                 const T& b,
                                                 const T& c)
    {
        return (a + c <= b || a >= b + c);	/* c should be > 0 */
    }


    /****************************************************************************/
    /*						P3Vector free operators								*/
    /****************************************************************************/

    /****************************************************************************/
    /*								P3Vector									*/
    /****************************************************************************/

    class P3Vector {
        double	x;
        double	y;
        double	z;

    public:
    // constructors
        P3Vector ();
        P3Vector (double x0,
                  double y0,
                  double z0 = 0.0);
        P3Vector (const P3Vector& v);

        ~P3Vector ();

    // modifiers
        inline void X (double x0);
        inline void Y (double y0);
        inline void Z (double z0);

        inline void XYZ (double x0,
                         double y0,
                         double z0 = 0.0);

        inline void SetToZero ();

        inline P3Vector& operator= (const P3Vector& v);
        inline P3Vector& operator+= (const P3Vector& v);
        inline P3Vector& operator-= (const P3Vector& v);

        P3Vector& operator^= (const P3Vector& v);

        inline P3Vector& operator*= (double s);

    // accessors
        P3Vector operator/ (const double& s) const;
        P3Vector UnitVector () const;
        P3Vector UnitVector (double eps) const;

        P3Vector Rotate2D (const double& angle) const;

        double Angle (const P3Vector& v) const;
        double Angle2D (const P3Vector& v) const;

        double PlaneAngle (const P3Vector& v,
                           const P3Vector& vNormal) const;

        inline double X () const;
        inline double Y () const;
        inline double Z () const;
        inline double Length () const;
        inline double TaxiDistance () const;
        inline bool IsZeroLength (double	eps = SmallEps) const;
        inline bool IsNonZeroLength (double	eps = SmallEps) const;

        inline bool IsEqual (const P3Vector& v,
                             double				eps = SmallEps) const;

        inline bool IsNotEqual (const P3Vector& v,
                                double			eps = SmallEps) const;

        inline bool IsParalell		(const P3Vector& v) const;

        inline bool IsPerpendicular	(const P3Vector& v) const;

    // static
        struct CV {							// Constant Vectors
            static const P3Vector O_NULL;	// O Null Vector
            static const P3Vector X_UNIT;	// X Unit Vector
            static const P3Vector Y_UNIT;	// Y Unit Vector
            static const P3Vector Z_UNIT;	// Z Unit Vector
        };
    };

    /****************************************************************************/
    /*					P3Vector Global Scope Operators							*/
    /****************************************************************************/

    P3Vector operator^ (const P3Vector& vLeft,
                        const P3Vector& vRight);

    double operator* (const P3Vector& vLeft,
                      const P3Vector& vRight);



    /****************************************************************************/
    /*								P3Vector in line							*/
    /****************************************************************************/
    inline P3Vector& P3Vector::operator= (const P3Vector& v)
    {
        if (this == &v) {
            return *this;
        }
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    inline P3Vector& P3Vector::operator+= (const P3Vector& v)
    {
        X (x + v.x);
        Y (y + v.y);
        Z (z + v.z);
        return *this;
    }

    inline P3Vector& P3Vector::operator-= (const P3Vector& v)
    {
        X (x - v.x);
        Y (y - v.y);
        Z (z - v.z);
        return *this;
    }

    inline P3Vector& P3Vector::operator*= (double s)
    {
        X (x * s);
        Y (y * s);
        Z (z * s);
        return *this;
    }

    //Get

    inline double P3Vector::X () const
    {
        return SmallFilter(x);
    }

    inline double P3Vector::Y () const
    {
        return SmallFilter(y);
    }

    inline double P3Vector::Z () const
    {
        return SmallFilter(z);
    }

    inline double P3Vector::Length () const
    {
        return SmallFilter (sqrt(x * x + y * y + z * z));
    }

    inline double P3Vector::TaxiDistance () const
    {
        return SmallFilter (fabs(x) + fabs(y) + fabs(z));
    }

    inline bool P3Vector::IsZeroLength (double	eps /* = SmallEps */) const
    {
        return (fabs(x) + fabs(y) + fabs(z) <= eps);
    }

    inline bool P3Vector::IsNonZeroLength (double	eps /* = SmallEps */) const
    {
        return (fabs(x) + fabs(y) + fabs(z) > eps);
    }

    inline bool P3Vector::IsEqual (const P3Vector& v,
                                   double			eps /* = SmallEps */) const
    {
        return fabs (x - v.x) < eps && fabs (y - v.y) < eps && fabs (z - v.z) < eps;
    }

    inline bool P3Vector::IsNotEqual (const P3Vector& v,
                                      double			eps /* = SmallEps */) const
    {
        return fabs (x - v.x) > eps || fabs (y - v.y) > eps || fabs (z - v.z) > eps;
    }


    inline bool P3Vector::IsParalell (const P3Vector& v) const
    {
        return IsNearZero ((*this ^ v).Length ());
    }


    inline bool P3Vector::IsPerpendicular (const P3Vector& v) const
    {
        return (this->IsZeroLength () || v.IsZeroLength () || IsNearZero ((*this) * v));
    }


    //Set
    inline void P3Vector::X (double xi)
    {
        x = SmallFilter (xi);
    }

    inline void P3Vector::Y (double yi)
    {
        y = SmallFilter (yi);
    }

    inline void P3Vector::Z (double zi)
    {
        z = SmallFilter (zi);
    }

    inline void P3Vector::XYZ (double xi,
                               double yi,
                               double zi /* = 0.0 */)
    {
        x = SmallFilter (xi);
        y = SmallFilter (yi);
        z = SmallFilter (zi);
    }

    inline void P3Vector::SetToZero ()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    inline double operator* (const P3Vector& vLeft,
                             const P3Vector& vRight)
    {
        double s;
        s = SmallFilter (vLeft.X () * vRight.X () +
                         vLeft.Y () * vRight.Y () +
                         vLeft.Z () * vRight.Z ());
        return s;
    }

    inline P3Vector operator+ (const P3Vector& vLeft,
                               const P3Vector& vRight)
    {
        P3Vector s (vLeft.X () + vRight.X (),
                    vLeft.Y () + vRight.Y (),
                    vLeft.Z () + vRight.Z ());
        return s;
    }

    inline P3Vector operator- (const P3Vector& vLeft,
                               const P3Vector& vRight)
    {
        P3Vector s (vLeft.X () - vRight.X (),
                    vLeft.Y () - vRight.Y (),
                    vLeft.Z () - vRight.Z ());
        return s;
    }

    inline P3Vector operator* (const double& vLeft,
                               const P3Vector& vRight)
    {
        P3Vector s (vLeft * vRight.X (), vLeft * vRight.Y (), vLeft * vRight.Z ());
        return s;
    }

    inline P3Vector operator* (const P3Vector& vLeft,
                               const double& vRight)
    {
        P3Vector s (vLeft.X () * vRight, vLeft.Y () * vRight, vLeft.Z () * vRight);
        return s;
    }

    inline bool operator== (const P3Vector& vLeft,
                            const P3Vector& vRight)
    {
        return	IsNear (vLeft.X (), vRight.X ()) &&
                IsNear (vLeft.Y (), vRight.Y ()) &&
                IsNear (vLeft.Z (), vRight.Z ());
    }

    inline bool operator!= (const P3Vector& vLeft,
                            const P3Vector& vRight)
    {
        return	IsNotNear (vLeft.X (), vRight.X ()) ||
                IsNotNear (vLeft.Y (), vRight.Y ()) ||
                IsNotNear (vLeft.Z (), vRight.Z ());
    }

} // namespace Nessie
