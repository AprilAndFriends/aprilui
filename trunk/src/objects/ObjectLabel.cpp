/// @file
/// @version 3.3
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
	Label::Label(chstr name, grect rect) : Object(name, rect), LabelBase()
	{
		this->text = "Label: " + name;
	}

	Label::~Label()
	{
	}

	Object* Label::createInstance(chstr name, grect rect)
	{
		return new Label(name, rect);
	}

	Dataset* Label::getDataset()
	{
		return Object::getDataset();
	}

	harray<PropertyDescription> Label::getPropertyDescriptions()
	{
		return (Object::getPropertyDescriptions() + LabelBase::getPropertyDescriptions());
	}

	void Label::OnDraw()
	{
		Object::OnDraw();
		float disabledAlphaFactor = this->_getDisabledAlphaFactor();
		april::Color color = this->_getDrawColor();
		color.a = (unsigned char)(color.a * disabledAlphaFactor);
		april::Color backgroundColor = this->backgroundColor;
		backgroundColor.a = (unsigned char)(backgroundColor.a * disabledAlphaFactor);
		LabelBase::_drawLabel(this->_getDrawRect(), color, backgroundColor);
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
		hstr result = Object::getProperty(name);
		if (result == "")
		{
			result = LabelBase::getProperty(name);
		}
		return result;
	}

	bool Label::setProperty(chstr name, chstr value)
	{
		if (Object::setProperty(name, value))
		{
			return true;
		}
		return LabelBase::setProperty(name, value);
	}
}
