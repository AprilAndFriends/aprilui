/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_ALPHA_FADER_H
#define APRILUI_ALPHA_FADER_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace AprilUI
{
	namespace Animators
	{
		class AprilUIExport AlphaFader : public Animator
		{
			float mInitialSpeed,mInitialAlpha,mSpeed,mAccel,mTimer,mDelay,mDestAlpha;
		public:
			AlphaFader(chstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void fade(float dest,float time);
			void update(float k);
			void reset();
		};
	}
}

#endif
