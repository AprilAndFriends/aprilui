/// @file
/// @version 3.5
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
	public:
		OptionButton(chstr name, grect rect);
		~OptionButton();
		inline hstr getClassName() const { return "OptionButton"; }

		static Object* createInstance(chstr name, grect rect);

		int getOptionCount();

		hstr getProperty(chstr name);

		void turnOn();
		void turnOff();
		
	};
}

#endif
