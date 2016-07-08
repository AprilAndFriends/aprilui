/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "AnimatorProgressChanger.h"
#include "aprilui.h"
#include "Object.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	namespace Animators
	{
		ProgressChanger::ProgressChanger(chstr name) : Animator(name)
		{
		}

		ProgressChanger::ProgressChanger(const ProgressChanger& other) : Animator(other)
		{
		}

		ProgressChanger::~ProgressChanger()
		{
		}

		Animator* ProgressChanger::createInstance(chstr name)
		{
			return new ProgressChanger(name);
		}

		float ProgressChanger::_getObjectValue() const
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::ProgressChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			return progressObject->getProgress();
		}

		void ProgressChanger::_setObjectValue(float value)
		{
			ProgressBase* progressObject = dynamic_cast<ProgressBase*>(this->parent);
			if (progressObject == NULL)
			{
				hlog::errorf(logTag, "Animators::ProgressChanger: parent object '%s' not a subclass of Objects::ProgressBase!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			progressObject->setProgress(value);
		}

		void ProgressChanger::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}
