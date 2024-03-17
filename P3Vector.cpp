// P3Vector.cpp									

#include	"P3Vector.h"

namespace Nessie {

/****************************************************************************/
/*					P3Vector Global Scope Operators							*/
/****************************************************************************/

P3Vector operator^ (const P3Vector&	vLeft,
					const P3Vector&	vRight)
{
	P3Vector v;
	v.X (vLeft.Y () * vRight.Z () - vLeft.Z () * vRight.Y ());
	v.Y (vLeft.Z () * vRight.X () - vLeft.X () * vRight.Z ());
	v.Z (vLeft.X () * vRight.Y () - vLeft.Y () * vRight.X ());
	return v;
}

P3Vector& P3Vector::operator^= (const P3Vector& vRight)
{
	P3Vector vLeft (*this);
	X (vLeft.Y () * vRight.Z () - vLeft.Z () * vRight.Y ());
	Y (vLeft.Z () * vRight.X () - vLeft.X () * vRight.Z ());
	Z (vLeft.X () * vRight.Y () - vLeft.Y () * vRight.X ());
	return *this;
}

/****************************************************************************/
/*								P3Vector									*/
/****************************************************************************/

const P3Vector P3Vector::CV::O_NULL	(0.0, 0.0, 0.0);	// O Null Vector
const P3Vector P3Vector::CV::X_UNIT	(1.0, 0.0, 0.0);	// X Unit Vector
const P3Vector P3Vector::CV::Y_UNIT	(0.0, 1.0, 0.0);	// Y Unit Vector
const P3Vector P3Vector::CV::Z_UNIT	(0.0, 0.0, 1.0);	// Z Unit Vector

P3Vector::P3Vector ()
:	x (0.0),
	y (0.0),
	z (0.0)
{
}

P3Vector::P3Vector (double xi,
					double yi,
					double zi /* = 0.0 */)
:	x (xi),
	y (yi),
	z (zi)
{
}

P3Vector::P3Vector (const P3Vector& v)
:	x (v.x),
	y (v.y),
	z (v.z)
{
}

P3Vector::~P3Vector ()
{
}

P3Vector P3Vector::operator/ (const double& s) const
{
	P3Vector v;
	if (fabs (s) > SmallEps) {
		v.X (x / s);
		v.Y (y / s);
		v.Z (z / s);
	} else {
		if (s < 0.0) {
			v.X (x / (-SmallEps));
			v.Y (y / (-SmallEps));
			v.Z (z / (-SmallEps));
		} else {
			v.X (x / SmallEps);
			v.Y (y / SmallEps);
			v.Z (z / SmallEps);
		}
	}
	return v;
}

P3Vector P3Vector::UnitVector () const
{
	double len = this->Length ();
	if (len > SmallEps) {
		return (*this / len);
	} else {
		WASSERT(false);
		return CV::X_UNIT;
	}
}

P3Vector P3Vector::UnitVector (double eps) const
{
	WASSERT(eps > 0.0);
	double len = this->Length ();
	if (len > eps && eps > 0.0) {
		return (*this / len);
	} else {
		WASSERT(false);
		return CV::X_UNIT;
	}
}

P3Vector P3Vector::Rotate2D (const double& angle) const
{
	P3Vector ret;

	ret.Z (Z ());

	ret.X ((X () * cos (angle)) - (Y () * sin (angle)));
	ret.Y ((X () * sin (angle)) + (Y () * cos (angle)));

	return ret;
}

double P3Vector::Angle (const P3Vector&	v) const
{
	P3Vector	dv (v - *this);
	double		a (Length ());
	double		b (v.Length ());
	double		c (dv.Length ());

	double		fi;

	if (a < SmallEps || b < SmallEps) {
		fi = 0.0;
	} else {
		double co = (a * a + b * b - c * c) / (2.0 * a * b);
		if (co > 1.0) {
			co = 1.0;
		}else if (co < -1.0) {
			co = -1.0;
		}
		fi = acos (co);
		if (fi < 0.0) {
			fi = fi + 2.0 * Pi;
		}
	}
	return (fi);
}

double P3Vector::Angle2D (const P3Vector&	v) const
{
	double fi = this->Angle (v);
	P3Vector vc = *this ^ v;
	if (vc.Z () < 0) {
		fi = -fi;
	}
	return (fi);
}

double P3Vector::PlaneAngle (const P3Vector&	v,
							 const P3Vector&	vNormal) const
{
	P3Vector	dv (*this ^ v);
	double		sign (dv * vNormal);
	double		fi = this->Angle (v);
	WASSERT(fi <= Pi);
	if (sign < 0.0) {
		fi = 2.0 * Pi - fi;
	}

	return (fi);
}

} // namespace Nessie
