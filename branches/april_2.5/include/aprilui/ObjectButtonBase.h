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
/// Defines a base class for all objects with button functionality.

#ifndef APRILUI_BUTTON_BASE_H
#define APRILUI_BUTTON_BASE_H

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

		bool isHovered() { return this->mHovered; }
		bool isPushed() { return this->mPushed; }
		virtual hstr getName() = 0;
		virtual bool isCursorInside() = 0;
		virtual Object* getParent() = 0;
		virtual Dataset* getDataset() = 0;
		
		virtual bool onMouseDown(int button);
		virtual bool onMouseUp(int button);
		virtual bool onMouseMove();
		virtual void cancelMouseDown();

		virtual hstr getProperty(chstr name, bool* propertyExists = NULL);
		virtual bool setProperty(chstr name, chstr value);

		static void setAllowedButtons(harray<unsigned char> buttons);
		
	protected:
		bool mHovered;
		bool mPushed;
		
		virtual void update(float k);
		virtual bool _checkHover();
		
	};
	
}

#endif