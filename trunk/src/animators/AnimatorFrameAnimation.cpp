/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>

#include "AnimatorFrameAnimation.h"
#include "Util.h"
#include "ObjectColoredImageBox.h"

namespace AprilUI
{
	namespace Animators
	{
		FrameAnimation::FrameAnimation(chstr name) : Object("Animators::FrameAnimation",name,0,0,1,1)
		{
			mStartFrame=0; mEndFrame=100;
			mAnimationTime=10;
			mTimer=0;
		}

		void FrameAnimation::setProperty(chstr name,chstr value)
		{
			if      (name == "start_frame") mStartFrame=value;
			else if (name == "end_frame")   mEndFrame=value;
			else if (name == "time")        mAnimationTime=value;
			else if (name == "base_name")   mImageBaseName=value;
		}

		void FrameAnimation::notifyEvent(chstr event_name,void* params)
		{
			if (event_name == "AttachToObject")
			{
				mTimer=0;
			}
			Object::notifyEvent(event_name,params);
		}

		void FrameAnimation::update(float k)
		{
			mTimer+=k;
			if (mTimer >= mAnimationTime) mTimer-=mAnimationTime;
			int frame=mStartFrame+(int) ((mEndFrame-mStartFrame)*mTimer/mAnimationTime);
			ImageBox* img=dynamic_cast<ImageBox*>(mParent);
			if (img)
			{
				img->setImageByName(mImageBaseName+hstr(frame));
			}
			else writelog("Animators::FrameAnimation: parent object not a subclass of Objects::ImageBox!");
		}
	}
}
