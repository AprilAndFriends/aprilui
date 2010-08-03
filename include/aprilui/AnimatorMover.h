/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_MOVER_H
#define APRILUI_MOVER_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"

namespace AprilUI
{
	namespace Animators
	{
		class Mover : public Object
		{
			float mInitialSX,mInitialSY;
			float mAccelX,mAccelY;
			float mSpeedX,mSpeedY;
			float mDestX,mDestY;
		public:
			Mover(chstr name);
			
			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
	}
}

#endif
