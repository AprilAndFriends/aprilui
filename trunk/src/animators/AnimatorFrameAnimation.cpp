/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.01
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
			this->imageBaseName = "";
			this->firstFrame = 0;
			this->frameCount = 0;
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
			if (this->imageBaseName == "")
			{
				return false;
			}
			if (this->frameCount <= 0)
			{
				return false;
			}
			return true;
		}

		bool FrameAnimation::setProperty(chstr name, chstr value)
		{
			if		(name == "base_name")		this->imageBaseName = value;
			else if (name == "first_frame")		this->firstFrame = value;
			else if (name == "frame_count")		this->frameCount = value;
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
				this->value = (float)this->firstFrame;
				float delay = this->delay;
				this->delay = 0.0f;
				this->update(0.0f);
				this->delay = delay;
			}
			Animator::notifyEvent(name, params);
		}

		void FrameAnimation::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
				if (imageBox == NULL)
				{
					hlog::error(aprilui::logTag, "Animators::FrameAnimation: parent object not a subclass of Objects::ImageBox!");
					return;
				}
				this->value = this->_calculateValue(this->timeSinceLastFrame);
				int frame = hmax((int)this->value, 0);
				int lastFrame = this->frameCount - 1;
				if (isExpired() || frame > lastFrame)
				{
					frame = (this->reset ? 0 : lastFrame);
				}
				imageBox->trySetImageByName(this->imageBaseName + hstr(this->firstFrame + frame));
			}
		}
		
	}
}
