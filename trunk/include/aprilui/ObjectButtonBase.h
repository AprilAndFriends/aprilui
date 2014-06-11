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
/// Defines a base class for all objects with button functionality.

#ifndef APRILUI_BUTTON_BASE_H
#define APRILUI_BUTTON_BASE_H

#include <april/Keys.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;
	class Object;

	class apriluiExport ButtonBase
	{
	public:
		ButtonBase();
		virtual ~ButtonBase();

		HL_DEFINE_IS(hovered, Hovered);
		HL_DEFINE_IS(pushed, Pushed);
		virtual hstr getName() = 0;
		virtual bool isCursorInside() = 0;
		virtual int getFocusIndex() = 0;
		virtual Object* getParent() = 0;
		virtual Dataset* getDataset() = 0;

		virtual bool onMouseDown(april::Key keyCode);
		virtual bool onMouseUp(april::Key keyCode);
		virtual bool onMouseCancel(april::Key keyCode);
		virtual bool onMouseMove();
		virtual bool onButtonDown(april::Button buttonCode);
		virtual bool onButtonUp(april::Button buttonCode);
		virtual void mouseCancel();

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		static void setAllowedKeys(harray<april::Key> keys);
		static void setAllowedButtons(harray<april::Button> buttons);
		
	protected:
		bool hovered;
		bool pushed;
		
		virtual void update(float timeDelta);
		virtual bool _checkHover();
		
	};
	
}

#endif
