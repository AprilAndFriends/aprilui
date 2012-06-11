/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a slider object with a fill state.

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
		~Slider();
		static Object* createInstance(chstr name, grect rect);
		
		float getValue() { return mValue; }
		void setValue(float value);
		
		bool setProperty(chstr name, chstr value);
		bool onMouseDown(int button);
		bool onMouseUp(int button);
		void onMouseMove();
		void cancelMouseDown();
		
		DEPRECATED_ATTRIBUTE bool onMouseDown(float x, float y, int button) { return onMouseDown(button); }
		DEPRECATED_ATTRIBUTE bool onMouseUp(float x, float y, int button) { return onMouseUp(button); }
		DEPRECATED_ATTRIBUTE void onMouseMove(float x, float y) { onMouseMove(); }

	protected:
		bool mPushed;
		float mValue;
		
		void update(float k);
		void OnDraw();
		
	};
	
}

#endif
