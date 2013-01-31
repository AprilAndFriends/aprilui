/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "AnimatorFrameAnimation.h"
#include "aprilui.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		FrameAnimation::FrameAnimation(chstr name) : Animator(name)
		{
			this->mImageBaseName = "";
			this->mFirstFrame = 0;
			this->mFrameCount = 0;
		}

		FrameAnimation::~FrameAnimation()
		{
		}

		Animator* FrameAnimation::createInstance(chstr name)
		{
			return new FrameAnimation(name);
		}

		bool FrameAnimation::isAnimated()
		{
			if (!Animator::isAnimated())
			{
				return false;
			}
			if (this->mImageBaseName == "")
			{
				return false;
			}
			if (this->mFrameCount <= 0)
			{
				return false;
			}
			return true;
		}

		bool FrameAnimation::setProperty(chstr name, chstr value)
		{
			if		(name == "base_name")		this->mImageBaseName = value;
			else if (name == "first_frame")		this->mFirstFrame = value;
			else if (name == "frame_count")		this->mFrameCount = value;
			else if (name == "inherit_value")
			{
				hlog::warn(aprilui::logTag, "Animators::FrameAnimation does not support 'inherit_value'!");
			}
			else Animator::setProperty(name, value);
			return true;
		}

		void FrameAnimation::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				this->mValue = (float)this->mFirstFrame;
				float delay = this->mDelay;
				this->mDelay = 0.0f;
				this->update(0.0f);
				this->mDelay = delay;
			}
			Animator::notifyEvent(name, params);
		}

		void FrameAnimation::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				ImageBox* imageBox = dynamic_cast<ImageBox*>(this->mParent);
				if (imageBox == NULL)
				{
					hlog::error(aprilui::logTag, "Animators::FrameAnimation: parent object not a subclass of Objects::ImageBox!");
					return;
				}
				this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
				int frame = (int)this->mValue;
				int lastFrame = this->mFirstFrame + this->mFrameCount - 1;
				if (isExpired() || frame > lastFrame)
				{
					frame = (this->mReset ? this->mFirstFrame : lastFrame);
				}
				imageBox->setImageByName(this->mImageBaseName + hstr(frame));
			}
		}
		
	}
}
