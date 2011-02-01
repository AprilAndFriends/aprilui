/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorFrameAnimation.h"
#include "aprilui.h"
#include "ObjectColoredImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		FrameAnimation::FrameAnimation(chstr name) : Animator("Animators::FrameAnimation", name, grect(0, 0, 1, 1))
		{
			mStartFrame = 0;
			mEndFrame = 100;
			mAnimationTime = 10.0f;
			mTimer = 0.0f;
			mLoop = 1;
		}

		bool FrameAnimation::isAnimated()
		{
			return (mTimer >= mAnimationTime);
		}

		void FrameAnimation::setProperty(chstr name, chstr value)
		{
			if      (name == "start_frame") mStartFrame = value;
			else if (name == "end_frame")   mEndFrame = value;
			else if (name == "time")        mAnimationTime = value;
			else if (name == "base_name")   mImageBaseName = value;
			else if (name == "loop")        mLoop = value;
            else if (name == "delay")       mInitialDelay = mDelay = value;
		}

		void FrameAnimation::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				mTimer = 0.0f;
			}
			Object::notifyEvent(name, params);
		}

		void FrameAnimation::update(float k)
		{
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			mTimer += k;
			if (mTimer >= mAnimationTime && mLoop)
			{
				mTimer -= mAnimationTime;
			}
			if (mTimer < mAnimationTime)
			{
				int frame = mStartFrame + (int)((mEndFrame - mStartFrame) * mTimer / mAnimationTime);
				ImageBox* image = dynamic_cast<ImageBox*>(mParent);
				if (image)
				{
					image->setImageByName(mImageBaseName + hstr(frame));
				}
				else
				{
					logMessage("Animators::FrameAnimation: parent object not a subclass of Objects::ImageBox!");
				}
			}
		}
		
	}
}
