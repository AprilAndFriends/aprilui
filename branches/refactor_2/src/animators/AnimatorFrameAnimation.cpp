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
#include "AprilUI.h"
#include "ObjectImageBox.h"

namespace AprilUI
{
	namespace Animators
	{
		FrameAnimation::FrameAnimation(chstr name) : Animator("Animators::FrameAnimation", name, grect(0, 0, 1, 1))
		{
			mFirstFrame = 0;
			mLastFrame = 0;
			mImageBaseName = "";
			mAnimationTime = 1.0f;
			mFrameTime = 0.1f;
			mLoops = 0;
			mReset = true;
		}

		bool FrameAnimation::isAnimated()
		{
			if (!Animator::isAnimated())
			{
				return false;
			}
			if (mImageBaseName == "")
			{
				return false;
			}
			if (mLoops > 0 && mTimer >= mAnimationTime * mLoops)
			{
				return false;
			}
			return true;
		}

		void FrameAnimation::setProperty(chstr name, chstr value)
		{
			Animator::setProperty(name, value);
			if      (name == "first_frame")	mFirstFrame = value;
			else if (name == "last_frame")	mLastFrame = value;
			else if (name == "base_name")	mImageBaseName = value;
			else if (name == "time")        mAnimationTime = value;
			else if (name == "frame_time")	mFrameTime = value;
			else if (name == "loops")		mLoops = value;
		}

		void FrameAnimation::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				update(0);
			}
			Animator::notifyEvent(name, params);
		}

		void FrameAnimation::update(float k)
		{
			bool animated = this->isAnimated();
			Animator::update(k);
			if (!animated)
			{
				return;
			}
			int frame;
			if (mLoops > 0 && mTimer >= mAnimationTime * mLoops)
			{
				frame = (mReset ? mFirstFrame : mLastFrame);
			}
			else
			{
				float time = fmod(mTimer, mAnimationTime);
				int frameCount = mLastFrame - mFirstFrame + 1;
				if (time >= frameCount * mFrameTime)
				{
					frame = (mReset ? mFirstFrame : mLastFrame);
				}
				else
				{
					frame = (int)(time / mFrameTime);
				}
			}
			ImageBox* image = dynamic_cast<ImageBox*>(mParent);
			if (image != NULL)
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
