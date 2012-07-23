/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
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
		this->mText = "Label: " + name;
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

	void Label::OnDraw()
	{
		Object::OnDraw();
		april::Color color = this->_getDrawColor();
		color.a = (unsigned char)(color.a * this->_getDisabledAlphaFactor());
		LabelBase::_drawLabel(this->_getDrawRect(), color);
	}

	void Label::notifyEvent(chstr name, void* params)
	{
		if (name == "onLocalizationChanged")
		{
			if (this->mTextKey != "")
			{
				this->setTextKey(this->mTextKey);
			}
		}
		Object::notifyEvent(name, params);
	}
	
	bool Label::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		if (isCursorInside())
		{
			this->triggerEvent("MouseDown", button);
			return true;
		}
		return false;
	}
	
	bool Label::onMouseUp(int button)
	{
		if (Object::onMouseUp(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			// TODO - this is not good as it will happen if you didn't click on the label, but released the button over it
			this->triggerEvent("Click", button);
			return true;
		}
		return false;
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
