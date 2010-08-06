/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_SCALER_H
#define APRILUI_SCALER_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"
#include <gtypes/Vector2.h>

namespace AprilUI
{
	namespace Animators
	{
		class AprilUIExport Scaler : public Animator
		{
			gtypes::Vector2 mInitialSize,mInitialS;
			gtypes::Vector2 mAccel;
			gtypes::Vector2 mSpeed;
			gtypes::Vector2 mDest;
		public:
			Scaler(chstr name);

			void scale(float dest_w,float dest_h,float time);
			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
	}
}

#endif
