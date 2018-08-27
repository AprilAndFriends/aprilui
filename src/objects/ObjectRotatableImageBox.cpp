/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "ObjectRotatableImageBox.h"

namespace aprilui
{
	RotatableImageBox::RotatableImageBox(chstr name, grectf rect) :
		RotationImageBox(name, rect)
	{
		mDestAngle = 0.0f;
		mRotationSpeed = 90;
	}

	void RotatableImageBox::update(float k)
	{
		Object::update(k);
		if (fabs(mDestAngle - mAngle) > 0.01f)
		{
			mAngle += hsgn(mDestAngle - mAngle) * hmin(k * mRotationSpeed, (float)fabs(mDestAngle - mAngle));
			if (fabs(mDestAngle - mAngle) < 0.01f)
			{
				mAngle = mDestAngle;
			}
		}
	}

	bool RotatableImageBox::isRotating()
	{
		return (fabs(mAngle - mDestAngle) > 0.01f);
	}
	
}
