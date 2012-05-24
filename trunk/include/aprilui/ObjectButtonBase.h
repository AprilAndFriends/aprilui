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
/// Defines a base class for all objects with button functionality.

#ifndef APRILUI_BUTTON_BASE_H
#define APRILUI_BUTTON_BASE_H

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

		bool isHovered() { return mHovered; }
		bool isPushed() { return mPushed; }
		virtual hstr getName() = 0;
		virtual bool isCursorInside() = 0;
		virtual Object* getParent() = 0;
		virtual Dataset* getDataset() = 0;
		
		virtual bool onMouseDown(int button);
		virtual bool onMouseUp(int button);
		virtual void onMouseMove();
		virtual void cancelMouseDown();

		DEPRECATED_ATTRIBUTE virtual bool onMouseDown(float x, float y, int button) { return onMouseDown(button); }
		DEPRECATED_ATTRIBUTE virtual bool onMouseUp(float x, float y, int button) { return onMouseUp(button); }
		DEPRECATED_ATTRIBUTE virtual void onMouseMove(float x, float y) { onMouseMove(); }

		static void setAllowedButtons(harray<unsigned char> buttons);
		
	protected:
		bool mHovered;
		bool mPushed;
		
		virtual void update(float k);
		virtual bool _checkHover();
		
	};
	
}

#endif
