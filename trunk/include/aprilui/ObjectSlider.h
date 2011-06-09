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

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport Slider : public ImageBox
	{
	public:
		Slider(chstr name, grect rect);
		
		float getValue() { return mValue; }
		void setValue(float value);
		
		bool setProperty(chstr name, chstr value);
		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		void OnMouseMove(float x, float y);
		
	protected:
		bool mPushed;
		float mValue;
		
		void update(float k);
		void OnDraw();
		
	};
	
}

#endif
