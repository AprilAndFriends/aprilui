/************************************************************************************
This source file is part of the APRIL User Interface Library
For latest info, see http://libatres.sourceforge.net/
*************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LGPL) as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*************************************************************************************/
#include <math.h>
#include "Vector.h"

namespace AprilUI
{
	Vector::Vector(float x_value,float y_value)
	{
		x=x_value;
		y=y_value;
	}

	Vector Vector::operator +(const Vector& v)
	{
		return Vector(x+v.x,y+v.y);
	}

	Vector Vector::operator -(const Vector& v)
	{
		return Vector(x-v.x,y-v.y);
	}

	Vector Vector::operator *(const float f)
	{
		return Vector(x*f,y*f);
	}

	void Vector::operator +=(const Vector& v)
	{
		x+=v.x; y+=v.y;
	}

	void Vector::operator -=(const Vector& v)
	{
		x-=v.x; y-=v.y;
	}

	void Vector::operator *=(const float f)
	{
		x*=f; y*=f;
	}

	float Vector::length()
	{
		return sqrt(x*x+y*y);
	}

	float Vector::squaredLength()
	{
		return	x*x+y*y;
	}

	void Vector::normalise()
	{
		float len=length();
		x/=len; y/=len;
	}

	Vector Vector::normalised()
	{
		Vector v(x,y);
		v.normalise();
		return v;
	}
}
