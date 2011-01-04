/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <math.h>

#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "ObjectRotatableImageBox.h"

namespace aprilui
{
	RotatableImageBox::RotatableImageBox(chstr name, grect rect) :
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
			mAngle += sgn(mDestAngle - mAngle) * hmin(k * mRotationSpeed, (float)fabs(mDestAngle - mAngle));
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
