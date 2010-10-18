/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_TOGGLE_BUTTON_H
#define APRILUI_TOGGLE_BUTTON_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectImageButton.h"

namespace AprilUI
{
	/*******************************************************************************/
	class AprilUIExport ToggleButton : public ImageButton
	{
	public:
		ToggleButton(chstr name,float x,float y,float w,float h);

		void OnDraw(float offset_x,float offset_y);
		bool OnMouseDown(float x,float y,int button);
		bool OnMouseUp(float x,float y,int button);
		void untoggle() { mPushed=0; }
		void toggle() { mPushed=!mPushed; }
		void turnOn() { mPushed=1; }
		void turnOff() { mPushed=0; }
		bool isToggled() { return mPushed; }
		
	};
}

#endif
