/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
		
		HL_DEFINE_GET(float, value, Value);
		void setValue(float value);
		
		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onMouseMove();
		void mouseCancel();
		
	protected:
		bool pushed;
		float value;
		
		void update(float timeDelta);
		void OnDraw();
		
	};
	
}

#endif
