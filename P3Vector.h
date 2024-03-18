#pragma once
// P3Vector.h

#pragma once

#include	"Real.h"

namespace Nessie {


    /****************************************************************************/
    /*                        Helper functions                                  */
    /****************************************************************************/

    inline double SmallFilter (double	a)
    {
#define DEBUG_SMALL
#if defined (DEBUG_SMALL)
        if (IsSmallEnough(a, SmallerEps)) {
            a = 0.0;
        }
#endif
        return a;
    }

    /****************************************************************************/
    /*                              P3Vector                                    */
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
    /*                P3Vector Global Scope Operators                           */
    /****************************************************************************/

    P3Vector operator^ (const P3Vector& vLeft,
                        const P3Vector& vRight);

    double operator* (const P3Vector& vLeft,
                      const P3Vector& vRight);



    /****************************************************************************/
    /*                              P3Vector in line                            */
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
        return IsNearZero(x, eps) && IsNearZero(y, eps) && IsNearZero(z, eps);
    }

    inline bool P3Vector::IsNonZeroLength (double	eps /* = SmallEps */) const
    {
        return IsNotTooSmall(x, eps) || IsNotTooSmall(y, eps) || IsNotTooSmall(z, eps);
    }

    inline bool P3Vector::IsEqual (const P3Vector   & v,
                                   double			eps /* = SmallEps */) const
    {
        return IsNear(x, v.x, eps) && IsNear(y, v.y, eps) && IsNear(z, v.z, eps);
    }

    inline bool P3Vector::IsNotEqual (const P3Vector    & v,
                                      double			eps /* = SmallEps */) const
    {
        return IsNotNear(x, v.x, eps) || IsNotNear(y, v.y, eps) || IsNotNear(z, v.z, eps);
    }


    inline bool P3Vector::IsParalell (const P3Vector& v) const
    {
        return IsNearZero((*this ^ v).Length());
    }


    inline bool P3Vector::IsPerpendicular (const P3Vector& v) const
    {
        return (IsZeroLength () || v.IsZeroLength () || IsNearZero ((*this) * v));
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
