/// @file
/// @version 5.0
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
		hmap<hstr, PropertyDescription> FrameAnimation::_propertyDescriptions;
		hmap<hstr, PropertyDescription::Accessor*> FrameAnimation::_getters;
		hmap<hstr, PropertyDescription::Accessor*> FrameAnimation::_setters;

		FrameAnimation::FrameAnimation(chstr name) :
			Animator(name)
		{
			this->imageBaseName = "";
			this->firstFrame = 0;
			this->frameCount = 0;
		}

		FrameAnimation::FrameAnimation(const FrameAnimation& other) :
			Animator(other)
		{
			this->imageBaseName = other.imageBaseName;
			this->firstFrame = other.firstFrame;
			this->frameCount = other.frameCount;
		}

		Animator* FrameAnimation::createInstance(chstr name)
		{
			return new FrameAnimation(name);
		}

		hmap<hstr, PropertyDescription>& FrameAnimation::getPropertyDescriptions() const
		{
			if (FrameAnimation::_propertyDescriptions.size() == 0)
			{
				FrameAnimation::_propertyDescriptions = Animator::getPropertyDescriptions();
				FrameAnimation::_propertyDescriptions["base_name"] = PropertyDescription("base_name", PropertyDescription::Type::String);
				FrameAnimation::_propertyDescriptions["first_frame"] = PropertyDescription("first_frame", PropertyDescription::Type::Int);
				FrameAnimation::_propertyDescriptions["frame_count"] = PropertyDescription("frame_count", PropertyDescription::Type::Int);
			}
			return FrameAnimation::_propertyDescriptions;
		}

		hmap<hstr, PropertyDescription::Accessor*>& FrameAnimation::_getGetters() const
		{
			if (FrameAnimation::_getters.size() == 0)
			{
				FrameAnimation::_getters = Animator::_getGetters();
				FrameAnimation::_getters["base_name"] = new PropertyDescription::Get<FrameAnimation, hstr>(&FrameAnimation::getImageBaseName);
				FrameAnimation::_getters["first_frame"] = new PropertyDescription::Get<FrameAnimation, int>(&FrameAnimation::getFirstFrame);
				FrameAnimation::_getters["frame_count"] = new PropertyDescription::Get<FrameAnimation, int>(&FrameAnimation::getFrameCount);
			}
			return FrameAnimation::_getters;
		}

		hmap<hstr, PropertyDescription::Accessor*>& FrameAnimation::_getSetters() const
		{
			if (FrameAnimation::_setters.size() == 0)
			{
				FrameAnimation::_setters = Animator::_getSetters();
				FrameAnimation::_setters["base_name"] = new PropertyDescription::Set<FrameAnimation, hstr>(&FrameAnimation::setImageBaseName);
				FrameAnimation::_setters["first_frame"] = new PropertyDescription::Set<FrameAnimation, int>(&FrameAnimation::setFirstFrame);
				FrameAnimation::_setters["frame_count"] = new PropertyDescription::Set<FrameAnimation, int>(&FrameAnimation::setFrameCount);
			}
			return FrameAnimation::_setters;
		}

		bool FrameAnimation::isAnimated() const
		{
			return (Animator::isAnimated() && this->imageBaseName != "" && this->frameCount > 0);
		}

		bool FrameAnimation::setProperty(chstr name, chstr value)
		{
			if (name == "inherit_value")
			{
				hlog::warn(logTag, "Animators::FrameAnimation does not support 'inherit_value'!");
				return false;
			}
			return Animator::setProperty(name, value);
		}

		void FrameAnimation::notifyEvent(chstr type, EventArgs* args)
		{
			if (type == Event::AttachedToObject && this->applyValuesOnAttach)
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
