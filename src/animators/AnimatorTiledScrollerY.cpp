/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

#include "AnimatorTiledScrollerY.h"
#include "aprilui.h"
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

		float TiledScrollerY::_getObjectValue()
		{
			return dynamic_cast<TiledImage*>(dynamic_cast<ImageBox*>(this->mParent)->getImage())->getScrollY();
		}

		void TiledScrollerY::_setObjectValue(float value)
		{
			dynamic_cast<TiledImage*>(dynamic_cast<ImageBox*>(this->mParent)->getImage())->setScrollY(value);
		}

		void TiledScrollerY::update(float k)
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
			this->_valueUpdateSimple(k);
		}
		
	}
}
