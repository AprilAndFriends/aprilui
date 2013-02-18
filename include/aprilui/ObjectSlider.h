/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
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
		
		float getValue() { return this->mValue; }
		void setValue(float value);
		
		bool setProperty(chstr name, chstr value);
		bool onMouseDown(april::Key button);
		bool onMouseUp(april::Key button);
		bool onMouseMove();
		void cancelMouseDown();
		
	protected:
		bool mPushed;
		float mValue;
		
		void update(float k);
		void OnDraw();
		
	};
	
}

#endif
