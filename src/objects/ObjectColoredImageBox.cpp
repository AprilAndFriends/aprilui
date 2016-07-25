/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectColoredImageBox.h"

namespace aprilui
{
	ColoredImageBox::ColoredImageBox(chstr name, grect rect) :
		ImageBox(name, rect)
	{
		_setTypeName("ColoredImageBox");
	}

	void ColoredImageBox::OnDraw(gvec2 offset)
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
		april::Color color = mColor;
		color.a = (unsigned char)(alpha * 255);
		mImage->draw(_getDrawRect() + offset, color);
		//rendersys->setTextureBlendMode(april::ALPHA_BLEND);
	}

	bool ColoredImageBox::setProperty(chstr name, chstr value)
	{
		ImageBox::setProperty(name, value);
		if (name == "color") setColor(value);
		return 1;
	}

}
