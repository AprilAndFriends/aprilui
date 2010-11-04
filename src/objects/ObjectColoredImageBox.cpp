/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/RenderSystem.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectColoredImageBox.h"

namespace AprilUI
{
	ColoredImageBox::ColoredImageBox(chstr name,float x,float y,float w,float h) :
		ImageBox(name,x,y,w,h)
	{
		_setTypeName("ColoredImageBox");
	}

	void ColoredImageBox::setColor(chstr color)
	{
		mColor.setColor(color);
	}

	void ColoredImageBox::OnDraw(float offset_x,float offset_y)
	{
		if (!mImage) mImage=mDataPtr->getImage("null");
		float alpha=getDerivedAlpha();
		if (!getDerivedEnabled()) alpha/=2;
		mImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,mColor.r_float(),mColor.g_float(),mColor.b_float(),alpha);
		//rendersys->setBlendMode(April::ALPHA_BLEND);
	}

	void ColoredImageBox::setProperty(chstr name,chstr value)
	{
		ImageBox::setProperty(name,value);
		if (name == "color") setColor(value);
	}

}
