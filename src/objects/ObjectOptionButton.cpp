/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "Image.h"
#include "ObjectOptionButton.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> OptionButton::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> OptionButton::_getters;

	OptionButton::OptionButton(chstr name) :
		ToggleButton(name)
	{
	}

	OptionButton::OptionButton(const OptionButton& other) :
		ToggleButton(other)
	{
	}

	Object* OptionButton::createInstance(chstr name)
	{
		return new OptionButton(name);
	}

	hmap<hstr, PropertyDescription>& OptionButton::getPropertyDescriptions() const
	{
		if (OptionButton::_propertyDescriptions.size() == 0)
		{
			OptionButton::_propertyDescriptions = ToggleButton::getPropertyDescriptions();
			OptionButton::_propertyDescriptions["option_count"] = PropertyDescription("option_count", PropertyDescription::Type::Int);
		}
		return OptionButton::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& OptionButton::_getGetters() const
	{
		if (OptionButton::_getters.size() == 0)
		{
			OptionButton::_getters = ToggleButton::_getGetters();
			OptionButton::_getters["option_count"] = new PropertyDescription::Get<OptionButton, int>(&OptionButton::getOptionCount);
		}
		return OptionButton::_getters;
	}

	int OptionButton::getOptionCount() const
	{
		return (this->parent != NULL ? this->parent->getChildrenObjects().dynamicCast<OptionButton*>().size() : 0);
	}

	void OptionButton::turnOn()
	{
		if (this->parent != NULL)
		{
			harray<OptionButton*> children = this->parent->getChildrenObjects().dynamicCast<OptionButton*>() - this;
			foreach (OptionButton*, it, children)
			{
				(*it)->toggled = false;
			}
		}
		this->toggled = true;
	}

	void OptionButton::turnOff()
	{
		// turning off manually is forbidden as one button always has to be on
		hlog::debugf(logTag, "Turning off option button '%s' manually is forbidden.", this->name.cStr());
	}
		
}
