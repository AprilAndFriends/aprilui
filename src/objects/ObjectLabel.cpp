/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectLabel.h"

namespace aprilui
{
	Label::Label(chstr name, grect rect) :
		LabelBase(),
		Object(name, rect)
	{
		mText = "Label: " + name;
	}

	Label::~Label()
	{
	}

	Object* Label::createInstance(chstr name, grect rect)
	{
		return new Label(name, rect);
	}

	hstr Label::getName()
	{
		return Object::getName();
	}

	void Label::OnDraw()
	{
		Object::OnDraw();
		april::Color color = _getDrawColor();
		if (!_isDerivedEnabled())
		{
			color.a /= 2;
		}
		LabelBase::_drawLabel(_getDrawRect(), color);
	}

	void Label::notifyEvent(chstr name, void* params)
	{
		if (name == "UpdateText")
		{
			setTextKey(mTextKey);
		}
		Object::notifyEvent(name, params);
	}
	
	void Label::setTextKey(chstr value)
	{
		setText(mDataset->getText(value));
		LabelBase::setTextKey(value);
	}
	
	hstr Label::getProperty(chstr name, bool* property_exists)
	{
		bool exists = false;
		hstr result = LabelBase::getProperty(name, &exists);
		if (!exists)
		{
			return Object::getProperty(name, property_exists);
		}
		if (property_exists != NULL)
		{
			*property_exists = exists;
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
