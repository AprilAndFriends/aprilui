/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		Slider(chstr name, grectf rect);
		
		float getValue() { return mValue; }
		void setValue(float value);
		
		bool setProperty(chstr name, chstr value);
		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		bool OnMouseMove(float x, float y);
		
	protected:
		bool mPushed;
		float mValue;
		
		void OnDraw(gvec2f offset = gvec2f());
		
	};
	
}

#endif
