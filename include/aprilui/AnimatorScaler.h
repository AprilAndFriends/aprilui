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
		public:
			Scaler(chstr name);
			
			bool isAnimated();
			void setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			void scale(float dest_w, float dest_h, float time);
            
		protected:
			gvec2 mInitialSize;
			gvec2 mInitialS;
			gvec2 mAccel;
			gvec2 mSpeed;
			gvec2 mDest;
			
		};
	}
}

#endif
