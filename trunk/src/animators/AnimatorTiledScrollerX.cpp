/// @file
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "AnimatorTiledScrollerX.h"
#include "aprilui.h"
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

		float TiledScrollerX::_getObjectValue()
		{
			return dynamic_cast<TiledImage*>(dynamic_cast<ImageBox*>(this->parent)->getImage())->getScrollX();
		}

		void TiledScrollerX::_setObjectValue(float value)
		{
			dynamic_cast<TiledImage*>(dynamic_cast<ImageBox*>(this->parent)->getImage())->setScrollX(value);
		}

		void TiledScrollerX::update(float k)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::error(aprilui::logTag, "Animators::TiledScrollerX: parent object not a subclass of Objects::ImageBox!");
				return;
			}
			TiledImage* image = dynamic_cast<TiledImage*>(imageBox->getImage());
			if (image == NULL)
			{
				hlog::error(aprilui::logTag, "Animators::TiledScrollerX: image in object not a subclass of Animators::TiledImage!");
				return;
			}
			this->_valueUpdateSimple(k);
		}
		
	}
}
