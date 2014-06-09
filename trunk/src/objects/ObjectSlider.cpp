/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
	Slider::Slider(chstr name, grect rect) : ImageBox(name, rect)
	{
		this->value = 0.0f;
		this->pushed = false;
	}
	
	Slider::~Slider()
	{
	}
	
	Object* Slider::createInstance(chstr name, grect rect)
	{
		return new Slider(name, rect);
	}
	
	void Slider::update(float timeDelta)
	{
		ImageBox::update(timeDelta);
		gvec2 position = aprilui::getCursorPosition();
		if (this->pushed && this->isPointInside(position))
		{
			grect rect = this->getBoundingRect();
			this->setValue((position.x - rect.x) / (rect.w - 4));
		}
	}
	
	void Slider::setValue(float value)
	{
		this->value = hclamp(value, 0.0f, 1.0f);
	}
	
	bool Slider::onMouseDown(april::Key keyCode)
	{
		if (Object::onMouseDown(keyCode)) // currently needs to override ImageBox behavior
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->pushed = true;
			gvec2 position = (aprilui::getCursorPosition() - this->getDerivedPosition()) / this->getDerivedScale();
			this->setValue(position.x / (this->rect.w - 4));
			this->triggerEvent("Set", keyCode);
			return true;
		}
		return false;
	}
	
	bool Slider::onMouseUp(april::Key keyCode)
	{
		if (Object::onMouseUp(keyCode)) // currently needs to override ImageBox behavior
		{
			return true;
		}
		if (this->pushed && this->isCursorInside())
		{
			this->pushed = false;
			return true;
		}
		this->pushed = false;
		return false;
	}
	
	bool Slider::onMouseMove()
	{
		if (Object::onMouseMove()) // currently needs to override ImageBox behavior
		{
			return true;
		}
		if (this->pushed)
		{
			gvec2 position = (aprilui::getCursorPosition() - this->getDerivedPosition()) / this->getDerivedScale();
			this->setValue(position.x / (this->rect.w - 4));
			this->triggerEvent("Set");
		}
		return false;
	}
	
	void Slider::mouseCancel()
	{
		this->pushed = false;
		ImageBox::mouseCancel();
	}
	
	void Slider::OnDraw()
	{
		grect rect = this->_getDrawRect();
		if (rect.w < 5 || rect.h < 5)
		{
			return;
		}
		if (this->image == NULL)
		{
			this->image = this->dataset->getImage(APRILUI_IMAGE_NAME_NULL);
		}
		april::Color color = this->_getDrawColor();
		april::rendersys->drawFilledRect(rect, april::Color(april::Color::White, color.a));
		april::Color backColor(color / 4.0f, color.a);
		rect.set(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2);
		april::rendersys->drawFilledRect(rect, backColor);
		rect.set(rect.x + 1, rect.y + 1, floor((rect.w - 2) * this->value), rect.h - 2);
		this->image->draw(rect, color);
	}
	
}
