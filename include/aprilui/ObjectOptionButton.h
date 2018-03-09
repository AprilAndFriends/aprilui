/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an option button that defines one out of several options of which only one can be active at the time.
/// Connected option buttons have the same parent.

#ifndef APRILUI_OPTION_BUTTON_H
#define APRILUI_OPTION_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectToggleButton.h"

namespace aprilui
{
	class apriluiExport OptionButton : public ToggleButton
	{
		APRILUI_CLONEABLE(OptionButton);
	public:
		OptionButton(chstr name);
		~OptionButton();
		inline hstr getClassName() const { return "OptionButton"; }

		static Object* createInstance(chstr name);

		harray<PropertyDescription> getPropertyDescriptions() const;

		int getOptionCount() const;

		void turnOn();
		void turnOff();
		
	protected:
		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;

	private:
		static harray<PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;

	};

}
#endif
