/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a scroll bar button for the scroll-forward functionality.

#ifndef APRILUI_SCROLL_BAR_BUTTON_FORWARD_H
#define APRILUI_SCROLL_BAR_BUTTON_FORWARD_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	class apriluiExport ScrollBarButtonForward : public ImageButton
	{
	public:
		ScrollBarButtonForward(chstr name, grect rect);
		~ScrollBarButtonForward();
		hstr getClassName() const { return "ScrollBarButtonForward"; }

		static Object* createInstance(chstr name, grect rect);

		void notifyEvent(chstr name, void* params);

	protected:
		static void _click(EventArgs* args);

	};
}

#endif
