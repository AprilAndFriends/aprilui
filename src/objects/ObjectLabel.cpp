/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectLabel.h"

namespace aprilui
{
	Label::Label(chstr name) : Object(name), LabelBase()
	{
		this->text = "Label: " + name;
		this->debugColor = april::Color(april::Color::Aqua, 32);
	}

	Label::Label(const Label& other) : Object(other), LabelBase(other)
	{
	}

	Label::~Label()
	{
	}

	Object* Label::createInstance(chstr name)
	{
		return new Label(name);
	}

	Dataset* Label::getDataset()
	{
		return Object::getDataset();
	}

	harray<PropertyDescription> Label::getPropertyDescriptions()
	{
		return (Object::getPropertyDescriptions() + LabelBase::getPropertyDescriptions());
	}

	void Label::_draw()
	{
		Object::_draw();
		grect drawRect = this->_makeDrawRect();
		april::Color drawColor = this->_makeDrawColor();
		this->_drawLabelBackground(drawRect, drawColor, this->_makeBackgroundDrawColor(drawColor));
		this->_drawLabel(drawRect, drawColor);
	}

	void Label::notifyEvent(chstr type, EventArgs* args)
	{
		Object::notifyEvent(type, args);
		LabelBase::notifyEvent(type, args);
	}

	bool Label::triggerEvent(chstr type, april::Key keyCode)
	{
		return Object::triggerEvent(type, keyCode);
	}

	bool Label::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Object::triggerEvent(type, keyCode, string);
	}

	bool Label::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		return Object::triggerEvent(type, keyCode, position, string, userData);
	}

	bool Label::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return Object::triggerEvent(type, buttonCode, string, userData);
	}

	bool Label::triggerEvent(chstr type, chstr string, void* userData)
	{
		return Object::triggerEvent(type, string, userData);
	}

	bool Label::triggerEvent(chstr type, void* userData)
	{
		return Object::triggerEvent(type, userData);
	}

	hstr Label::getProperty(chstr name)
	{
		hstr result = LabelBase::getProperty(name);
		if (result == "")
		{
			result = Object::getProperty(name);
		}
		return result;
	}

	bool Label::setProperty(chstr name, chstr value)
	{
		if (LabelBase::setProperty(name, value))
		{
			return true;
		}
		return Object::setProperty(name, value);
	}
}
