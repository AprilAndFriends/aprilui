/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_TEXT_BUTTON_H
#define APRILUI_TEXT_BUTTON_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectLabel.h"

namespace AprilUI
{
	class AprilUIExport TextButton : public Label
	{
	protected:
		bool mPushed,mBackgroundEnabled;
		void OnDraw(float offset_x,float offset_y);
	public:
		TextButton(chstr name,float x,float y,float w,float h);
		
		void setTextKey(chstr key);
		bool OnMouseDown(int button,float x,float y);
		bool OnMouseUp(int button,float x,float y);
		void setProperty(chstr name,chstr value);
	};
}

#endif
