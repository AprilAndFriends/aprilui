/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectSlider.h"

namespace aprilui
{
	Slider::Slider(chstr name, grect rect) :
		ImageBox(name, rect)
	{
		this->mValue = 0.0f;
		this->mPushed = false;
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
		gvec2 position = aprilui::getCursorPosition();
		if (this->mPushed && this->isPointInside(position))
		{
			grect rect = this->getBoundingRect();
			this->setValue((position.x - rect.x) / (rect.w - 4));
		}
	}
	
	void Slider::setValue(float value)
	{
		this->mValue = hclamp(value, 0.0f, 1.0f);
	}
	
	bool Slider::onMouseDown(april::Key button)
	{
		if (ImageBox::onMouseDown(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->mPushed = true;
			gvec2 position = (aprilui::getCursorPosition() - this->getDerivedPosition()) / this->getDerivedScale();
			this->setValue(position.x / (this->mRect.w - 4));
			this->triggerEvent("Set", button);
			return true;
		}
		return false;
	}
	
	bool Slider::onMouseUp(april::Key button)
	{
		if (ImageBox::onMouseUp(button))
		{
			return true;
		}
		if (this->mPushed && this->isCursorInside())
		{
			this->mPushed = false;
			return true;
		}
		this->mPushed = false;
		return false;
	}
	
	bool Slider::onMouseMove()
	{
		if (ImageBox::onMouseMove())
		{
			return true;
		}
		if (this->mPushed)
		{
			gvec2 position = (aprilui::getCursorPosition() - this->getDerivedPosition()) / this->getDerivedScale();
			this->setValue(position.x / (this->mRect.w - 4));
			this->triggerEvent("Set");
		}
		return false;
	}
	
	void Slider::cancelMouseDown()
	{
		this->mPushed = false;
		ImageBox::cancelMouseDown();
	}
	
	void Slider::OnDraw()
	{
		grect rect = this->_getDrawRect();
		if (rect.w < 5 || rect.h < 5)
		{
			return;
		}
		if (this->mImage == NULL)
		{
			this->mImage = this->mDataset->getImage("null");
		}
		april::Color color = this->_getDrawColor();
		april::rendersys->drawFilledRect(rect, april::Color(april::Color::White, color.a));
		april::Color backColor(color / 4.0f, color.a);
		rect.set(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2);
		april::rendersys->drawFilledRect(rect, backColor);
		rect.set(rect.x + 1, rect.y + 1, floor((rect.w - 2) * mValue), rect.h - 2);
		this->mImage->draw(rect, color);
	}
	
	bool Slider::setProperty(chstr name, chstr value)
	{
		return ImageBox::setProperty(name, value);
	}
	
}
