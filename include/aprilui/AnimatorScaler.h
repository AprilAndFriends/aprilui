/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_SCALER_H
#define APRILUI_SCALER_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace AprilUI
{
	namespace Animators
	{
		class AprilUIExport Scaler : public Animator
		{
			gvec2 mInitialSize,mInitialS;
			gvec2 mAccel;
			gvec2 mSpeed;
			gvec2 mDest;
            float mDelay;
		public:
			Scaler(chstr name);

			void scale(float dest_w,float dest_h,float time);
			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
            
            void setDelay(float delay) { mDelay=delay; }
		};
	}
}

#endif
