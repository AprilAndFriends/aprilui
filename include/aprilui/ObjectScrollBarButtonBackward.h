/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a scroll bar button for the scroll-forward functionality.

#ifndef APRILUI_SCROLL_BAR_BUTTON_BACKWARD_H
#define APRILUI_SCROLL_BAR_BUTTON_BACKWARD_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	class apriluiExport ScrollBarButtonBackward : public ImageButton
	{
		APRILUI_CLONEABLE(ScrollBarButtonBackward);
	public:
		ScrollBarButtonBackward(chstr name);
		inline hstr getClassName() const { return "ScrollBarButtonBackward"; }
		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		static void _click(EventArgs* args);

	};

}
#endif
