/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectRotationImageBox.h"

namespace AprilUI
{
	RotationImageBox::RotationImageBox(chstr name, grect rect) :
		ImageBox(name, rect)
	{
		mAngle = 0.0f;
	}

	void RotationImageBox::setProperty(chstr name, chstr value)
	{
		ImageBox::setProperty(name, value);
		if (name == "angle")	mAngle = value;
	}

	void RotationImageBox::OnDraw(float offset_x, float offset_y)
	{
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		float alpha = getDerivedAlpha();
		if (!isDerivedEnabled())
		{
			alpha /= 2;
		}
		April::Color color;
		color.a = alpha * 255;
		mImage->draw(mRect + gvec2(offset_x, offset_y), color, mAngle);
	}

	bool RotationImageBox::angleEquals(float angle)
	{
		float s1 = dsin(angle);
		float s2 = dsin(mAngle);
		float c1 = dcos(angle);
		float c2 = dcos(mAngle);
		return (fabs(s1 - s2) < 0.01f && fabs(c1 - c2) < 0.01f);
	}
	
}
