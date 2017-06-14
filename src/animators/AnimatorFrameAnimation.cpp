/// @file
/// @version 4.3
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
		harray<PropertyDescription> FrameAnimation::_propertyDescriptions;

		FrameAnimation::FrameAnimation(chstr name) : Animator(name)
		{
			this->imageBaseName = "";
			this->firstFrame = 0;
			this->frameCount = 0;
		}

		FrameAnimation::FrameAnimation(const FrameAnimation& other) : Animator(other)
		{
			this->imageBaseName = other.imageBaseName;
			this->firstFrame = other.firstFrame;
			this->frameCount = other.frameCount;
		}

		FrameAnimation::~FrameAnimation()
		{
		}

		Animator* FrameAnimation::createInstance(chstr name)
		{
			return new FrameAnimation(name);
		}

		bool FrameAnimation::isAnimated() const
		{
			return (Animator::isAnimated() && this->imageBaseName != "" && this->frameCount > 0);
		}

		harray<PropertyDescription> FrameAnimation::getPropertyDescriptions() const
		{
			if (FrameAnimation::_propertyDescriptions.size() == 0)
			{
				FrameAnimation::_propertyDescriptions += PropertyDescription("base_name", PropertyDescription::Type::String);
				FrameAnimation::_propertyDescriptions += PropertyDescription("first_frame", PropertyDescription::Type::Int);
				FrameAnimation::_propertyDescriptions += PropertyDescription("frame_count", PropertyDescription::Type::Int);
			}
			return (Animator::getPropertyDescriptions() + FrameAnimation::_propertyDescriptions);
		}

		hstr FrameAnimation::getProperty(chstr name)
		{
			if (name == "base_name")	return this->getImageBaseName();
			if (name == "first_frame")	return this->getFirstFrame();
			if (name == "frame_count")	return this->getFrameCount();
			return Animator::getProperty(name);
		}

		bool FrameAnimation::setProperty(chstr name, chstr value)
		{
			if		(name == "base_name")		this->setImageBaseName(value);
			else if (name == "first_frame")		this->setFirstFrame(value);
			else if (name == "frame_count")		this->setFrameCount(value);
			else if (name == "inherit_value")
			{
				hlog::warn(logTag, "Animators::FrameAnimation does not support 'inherit_value'!");
			}
			else return Animator::setProperty(name, value);
			return true;
		}

		void FrameAnimation::notifyEvent(chstr type, EventArgs* args)
		{
			if (type == Event::AttachedToObject)
			{
				this->value = (float)this->firstFrame;
				float delay = this->delay;
				this->delay = 0.0f;
				this->update(0.0f);
				this->delay = delay;
			}
			Animator::notifyEvent(type, args);
		}

		void FrameAnimation::_update(float timeDelta)
		{
			if (this->imageBaseName != "")
			{
				if (this->_checkUpdate(timeDelta))
				{
					ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
					if (imageBox == NULL)
					{
						hlog::error(logTag, "Animators::FrameAnimation: parent object not a subclass of Objects::ImageBox!");
						return;
					}
					this->value = this->_calculateValue(this->timeDelta);
					int frame = hmax((int)this->value, 0);
					int lastFrame = this->frameCount - 1;
					if (isExpired() || frame > lastFrame)
					{
						frame = (this->resetOnExpire ? 0 : lastFrame);
					}
					imageBox->trySetImageByName(this->imageBaseName + hstr(this->firstFrame + frame));
				}
			}
		}
		
	}
}
