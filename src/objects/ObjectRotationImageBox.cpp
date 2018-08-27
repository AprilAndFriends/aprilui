/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectRotationImageBox.h"

namespace aprilui
{
	RotationImageBox::RotationImageBox(chstr name, grectf rect) :
		ImageBox(name, rect)
	{
		mAngle = 0.0f;
	}

	bool RotationImageBox::setProperty(chstr name, chstr value)
	{
		ImageBox::setProperty(name, value);
		if (name == "angle") mAngle = value;
		return 1;
	}

	void RotationImageBox::OnDraw(gvec2f offset)
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
		april::Color color;
		color.a = (unsigned char)(alpha * 255);
		mImage->draw(_getDrawRect() + offset, color, mAngle);
	}

	bool RotationImageBox::angleEquals(float angle)
	{
		float s1 = (float)hsin(angle);
		float s2 = (float)hsin(mAngle);
		float c1 = (float)hcos(angle);
		float c2 = (float)hcos(mAngle);
		return (fabs(s1 - s2) < 0.01f && fabs(c1 - c2) < 0.01f);
	}
	
}
