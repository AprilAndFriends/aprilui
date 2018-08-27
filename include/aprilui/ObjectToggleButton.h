/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_TOGGLE_BUTTON_H
#define APRILUI_TOGGLE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	class apriluiExport ToggleButton : public ImageButton
	{
	public:
		ToggleButton(chstr name, grectf rect);

		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		void toggle() { mPushed = !mPushed; }
		void turnOn() { mPushed = true; }
		void turnOff() { mPushed = false; }
		
	protected:
		void OnDraw(gvec2f offset = gvec2f());
		
	};
}

#endif
