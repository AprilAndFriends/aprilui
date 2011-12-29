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

#include "aprilui.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectSlider.h"

namespace aprilui
{
	Slider::Slider(chstr name, grect rect) :
		ImageBox(name, rect)
	{
		mValue = 0.0f;
		mPushed = false;
	}
	
	Slider::~Slider()
	{
	}

	Object* Slider::createInstance(chstr name, grect rect)
	{
		return new Slider(name, rect);
	}

	void Slider::update(float k)
	{
		ImageBox::update(k);
		gvec2 position = getCursorPosition();
		if (mPushed && isPointInside(position))
		{
			grect rect = getDerivedRect();
			setValue((position.x - rect.x) / (rect.w - 4));
		}
	}
	
	void Slider::setValue(float value)
	{
		mValue = hclamp(value, 0.0f, 1.0f);
	}

	bool Slider::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			mPushed = true;
			setValue((x - mRect.x) / (mRect.w - 4));
			_triggerEvent("Set", x, y, button);
			return true;
		}
		return false;
	}

	bool Slider::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button))
		{
			return true;
		}
		if (mPushed && isCursorInside())
		{
			mPushed = false;
			return true;
		}
		mPushed = false;
		return false;
	}

	void Slider::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
		if (mPushed)
		{
			setValue((x - mRect.x) / (mRect.w - 4));
			_triggerEvent("Set", x, y);
		}
	}

	void Slider::OnDraw()
	{
		grect rect = _getDrawRect();
		if (rect.w < 5 || rect.h < 5)
		{
			return;
		}
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		april::Color color = _getDrawColor();
		april::rendersys->drawColoredQuad(rect, april::Color(APRIL_COLOR_WHITE, color.a));
		april::Color backColor(color / 4.0f, color.a);
		rect = grect(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2);
		april::rendersys->drawColoredQuad(rect, backColor);
		rect = grect(rect.x + 1, rect.y + 1, floor((rect.w - 2) * mValue), rect.h - 2);
		mImage->draw(rect, color);
	}

	bool Slider::setProperty(chstr name, chstr value)
	{
		return ImageBox::setProperty(name, value);
	}
	
}
