/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_FRAME_ANIMATION_H
#define APRILUI_FRAME_ANIMATION_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport FrameAnimation : public Animator
		{
		public:
			FrameAnimation(chstr name);

			bool isAnimated();
            void setDelay(float value) { mDelay = value; }
			
			void setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
		protected:
			hstr mImageBaseName;
			int mStartFrame;
			int mEndFrame;
			float mAnimationTime;
			float mTimer;
			int mLoop;
            float mDelay;
			
		};
	}
}

#endif
