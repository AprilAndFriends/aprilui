/// @file
/// @version 5.1
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
	hmap<hstr, PropertyDescription> Label::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> Label::_getters;
	hmap<hstr, PropertyDescription::Accessor*> Label::_setters;

	Label::Label(chstr name) :
		Object(name),
		LabelBase()
	{
		this->text = "Label: " + name;
		this->debugColor.set(april::Color::Aqua, 32);
	}

	Label::Label(const Label& other) :
		Object(other),
		LabelBase(other)
	{
	}

	Object* Label::createInstance(chstr name)
	{
		return new Label(name);
	}

	hmap<hstr, PropertyDescription>& Label::getPropertyDescriptions() const
	{
		if (Label::_propertyDescriptions.size() == 0)
		{
			Label::_propertyDescriptions = Object::getPropertyDescriptions() + LabelBase::getPropertyDescriptions();
		}
		return Label::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Label::_getGetters() const
	{
		if (Label::_getters.size() == 0)
		{
			Label::_getters = Object::_getGetters() + LabelBase::_generateGetters<Label>();
		}
		return Label::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Label::_getSetters() const
	{
		if (Label::_setters.size() == 0)
		{
			Label::_setters = Object::_getSetters() + LabelBase::_generateSetters<Label>();
		}
		return Label::_setters;
	}

	Dataset* Label::getDataset() const
	{
		return Object::getDataset();
	}

	hstr Label::getAutoScaledFont()
	{
		this->_calcAutoScaledFont(this->_makeDrawRect());
		return this->autoScaledFont;
	}

	void Label::_draw()
	{
		Object::_draw();
		grectf drawRect = this->_makeDrawRect();
		april::Color drawColor = this->_makeDrawColor();
		this->_drawLabelBackground(drawRect, drawColor, this->_makeBackgroundDrawColor(drawColor));
		this->_drawLabel(drawRect, drawColor);
	}

	hstr Label::getProperty(chstr name)
	{
		hstr result = LabelBase::getProperty(name); // check side-class first
		if (result == "")
		{
			result = Object::getProperty(name);
		}
		return result;
	}

	bool Label::setProperty(chstr name, chstr value)
	{
		if (LabelBase::setProperty(name, value)) // check side-class first
		{
			return true;
		}
		return Object::setProperty(name, value);
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

	bool Label::triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string, void* userData)
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

}
