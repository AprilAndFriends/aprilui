/// @file
/// @version 3.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a scroll bar button for the slider background.

#ifndef APRILUI_SCROLL_BAR_BUTTON_BACKGROUND_H
#define APRILUI_SCROLL_BAR_BUTTON_BACKGROUND_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	class apriluiExport ScrollBarButtonBackground : public ImageButton
	{
	public:
		ScrollBarButtonBackground(chstr name, grect rect);
		~ScrollBarButtonBackground();
		inline hstr getClassName() const { return "ScrollBarButtonBackground"; }

		static Object* createInstance(chstr name, grect rect);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		static void _click(EventArgs* args);

	};
}

#endif
