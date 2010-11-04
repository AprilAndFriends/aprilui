/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_SLIDER_H
#define APRILUI_SLIDER_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectImageBox.h"

namespace AprilUI
{
	class AprilUIExport Slider : public ImageBox
	{
		bool mPushed;
		float mValue;
	public:
		Slider(chstr name,float x,float y,float w,float h);
		bool OnMouseDown(float x,float y,int button);
		bool OnMouseUp(float x,float y,int button);
		void OnMouseMove(float x,float y);
		void OnDraw(float offset_x,float offset_y);
		void setProperty(chstr name,chstr value);
		float getValue() { return mValue; }
		void setValue(float value) { mValue=value; }
		
	};
	
}

#endif
