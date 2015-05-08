/// @file
/// @version 4.0
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
	OptionButton::OptionButton(chstr name) : ToggleButton(name)
	{
	}

	OptionButton::OptionButton(const OptionButton& other) : ToggleButton(other)
	{
	}

	OptionButton::~OptionButton()
	{
	}

	Object* OptionButton::createInstance(chstr name)
	{
		return new OptionButton(name);
	}

	int OptionButton::getOptionCount()
	{
		return (this->parent != NULL ? this->parent->getChildrenObjects().dynamicCast<OptionButton*>().size() : 0);
	}

	hstr OptionButton::getProperty(chstr name)
	{
		if (name == "option_count")	return this->getOptionCount();
		return ToggleButton::getProperty(name);
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
