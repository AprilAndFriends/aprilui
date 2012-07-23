/// @file
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "AnimatorTiledScrollerY.h"
#include "ObjectImageBox.h"
#include "TiledImage.h"

namespace aprilui
{
	namespace Animators
	{
		TiledScrollerY::TiledScrollerY(chstr name) : Animator(name)
		{
		}

		TiledScrollerY::~TiledScrollerY()
		{
		}

		Animator* TiledScrollerY::createInstance(chstr name)
		{
			return new TiledScrollerY(name);
		}

		void TiledScrollerY::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject" || name == "OnDelayEnd" && this->mInheritValue)
			{
				this->mValue = this->mOffset = this->mParent->getY();
				if (this->mUseTarget)
				{
					this->mAmplitude = this->mTarget - this->mValue;
				}
			}
			Object::notifyEvent(name, params);
		}
		
		void TiledScrollerY::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				ImageBox* imageBox = dynamic_cast<ImageBox*>(this->mParent);
				if (imageBox == NULL)
				{
					aprilui::log("Animators::TiledScrollerY: parent object not a subclass of Objects::ImageBox!");
					return;
				}
				TiledImage* image = dynamic_cast<TiledImage*>(imageBox->getImage());
				if (image == NULL)
				{
					aprilui::log("Animators::TiledScrollerY: image in object not a subclass of Animators::TiledImage!");
					return;
				}
				this->mValue = image->getScrollY();
				this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
				image->setScrollY(this->mValue);
			}
		}
		
	}
}
