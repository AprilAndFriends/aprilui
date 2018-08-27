/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectSlider.h"

namespace aprilui
{
	Slider::Slider(chstr name, grectf rect) :
		ImageBox(name, rect)
	{
		_setTypeName("Slider");
		mValue = 0.0f;
		mPushed = false;
	}
	
	void Slider::setValue(float value)
	{
		mValue = hclamp(value, 0.0f, 1.0f);
	}

	bool Slider::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isPointInside(x, y))
		{
			mPushed = true;
			setValue((x - mRect.x) / (mRect.w - 4));
			triggerEvent("Set", x, y, 0);
			return true;
		}
		return false;
	}

	bool Slider::OnMouseUp(float x, float y, int button)
	{
		if (Object::OnMouseUp(x, y, button))
		{
			return true;
		}
		if (mPushed && isPointInside(x, y))
		{
			mPushed = false;
			return true;
		}
		mPushed = false;
		return false;
	}

	bool Slider::OnMouseMove(float x, float y)
	{
		if (Object::OnMouseMove(x, y))
		{
			return true;
		}
		if (mPushed)
		{
			setValue((x - mRect.x) / (mRect.w - 4));
			triggerEvent("Set", x, y, 0);
			return true;
		}
		return false;
	}

	void Slider::OnDraw(gvec2f offset)
	{
		grectf rect = _getDrawRect() + offset;
		if (rect.w < 5 || rect.h < 5)
		{
			return;
		}
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		float alpha = getDerivedAlpha();
		april::rendersys->drawFilledRect(rect, april::Color(april::Color::White, COLOR_COMP_FOR_NEW_APRIL(alpha)));
		rect = grectf(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2);
		april::rendersys->drawFilledRect(rect, april::Color(COLOR_COMP_FOR_NEW_APRIL(0.3f), 
															 COLOR_COMP_FOR_NEW_APRIL(0.3f),
															 COLOR_COMP_FOR_NEW_APRIL(0.3f),
															 COLOR_COMP_FOR_NEW_APRIL(alpha)));
		april::Color color;
		color.a = (unsigned char)(alpha * 255);
		rect = grectf(rect.x + 1, rect.y + 1, floor((rect.w - 2) * mValue), rect.h - 2);
		mImage->draw(rect, color);
	}

	bool Slider::setProperty(chstr name, chstr value)
	{
		ImageBox::setProperty(name, value);
		return 1;
	}
	
}
