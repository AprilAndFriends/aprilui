/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_COLOR_ALTERNATOR_H
#define APRILUI_COLOR_ALTERNATOR_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "AprilUIExport.h"
namespace AprilUI
{
	namespace Animators
	{
		// note: only works on ColoredImages
		class AprilUIExport ColorAlternator : public Object
		{
			float mLow[4],mHigh[4];
			float mTimer,mSpeed;
		public:
			ColorAlternator(chstr name);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
	}
}

#endif
