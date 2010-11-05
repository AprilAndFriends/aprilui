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
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectColoredImageBox.h"

namespace AprilUI
{
	ColoredImageBox::ColoredImageBox(chstr name,grect rect) :
		ImageBox(name,rect)
	{
		_setTypeName("ColoredImageBox");
	}

	void ColoredImageBox::OnDraw(float offset_x,float offset_y)
	{
		if (!mImage) mImage=mDataset->getImage("null");
		float alpha=getDerivedAlpha();
		if (!isDerivedEnabled()) alpha/=2;
		April::Color color = mColor;
		color.a = alpha * 255;
		mImage->draw(mRect + gvec2(offset_x, offset_y), color);
		//rendersys->setBlendMode(April::ALPHA_BLEND);
	}

	void ColoredImageBox::setProperty(chstr name,chstr value)
	{
		ImageBox::setProperty(name,value);
		if (name == "color") setColor(value);
	}

}
