/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_TOGGLE_BUTTON_H
#define APRILUI_TOGGLE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectImageButton.h"

namespace AprilUI
{
	class AprilUIExport ToggleButton : public ImageButton
	{
	public:
		ToggleButton(chstr name, grect rect);

		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		void untoggle() { mPushed = false; }
		void toggle() { mPushed = !mPushed; }
		void turnOn() { mPushed = true; }
		void turnOff() { mPushed = false; }
		bool isToggled() { return mPushed; }
		
	protected:
		void OnDraw(gvec2 offset = gvec2());
		
	};
}

#endif
