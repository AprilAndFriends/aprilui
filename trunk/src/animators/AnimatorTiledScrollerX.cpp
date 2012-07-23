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
#include "AnimatorTiledScrollerX.h"
#include "ObjectImageBox.h"
#include "TiledImage.h"

namespace aprilui
{
	namespace Animators
	{
		TiledScrollerX::TiledScrollerX(chstr name) : Animator(name)
		{
		}

		TiledScrollerX::~TiledScrollerX()
		{
		}

		Animator* TiledScrollerX::createInstance(chstr name)
		{
			return new TiledScrollerX(name);
		}

		void TiledScrollerX::notifyEvent(chstr name, void* params)
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
		
		void TiledScrollerX::update(float k)
		{
			if (this->_checkUpdate(k))
			{
				ImageBox* imageBox = dynamic_cast<ImageBox*>(this->mParent);
				if (imageBox == NULL)
				{
					aprilui::log("Animators::TiledScrollerX: parent object not a subclass of Objects::ImageBox!");
					return;
				}
				TiledImage* image = dynamic_cast<TiledImage*>(imageBox->getImage());
				if (image == NULL)
				{
					aprilui::log("Animators::TiledScrollerX: image in object not a subclass of Animators::TiledImage!");
					return;
				}
				this->mValue = image->getScrollX();
				this->mValue = this->_calculateValue(this->mTimeSinceLastFrame);
				image->setScrollX(this->mValue);
			}
		}
		
	}
}
