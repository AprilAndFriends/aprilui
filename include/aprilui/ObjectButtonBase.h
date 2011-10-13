/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
		~ButtonBase();
		
		virtual bool isCursorInside() = 0;
		virtual Object* getParent() = 0;
		virtual Dataset* getDataset() = 0;
		
		bool onMouseDown(float x, float y, int button);
		bool onMouseUp(float x, float y, int button);
		void onMouseMove(float x, float y);

		static void setAllowedButtons(harray<unsigned char> buttons);
		
	protected:
		bool mPushed;
		bool mHover;
		
		virtual void update(float k);
		bool _checkHover();
		
	};
	
}

#endif
