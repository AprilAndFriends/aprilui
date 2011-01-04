/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_ROTATOR_H
#define APRILUI_ROTATOR_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport Rotator : public Animator
		{
		public:
			Rotator(chstr name);
			
			bool isAnimated();
			void setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
		protected:
			float mInitialSpeed;
			float mInitialAngle;
			float mSpeed;
			float mAccel;
			float mDelay;
			float mDestAngle;
			
		};
	}
}

#endif
