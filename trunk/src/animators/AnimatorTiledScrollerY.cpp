/// @file
/// @author  Boris Mikic
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
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
			TiledImage* image = this->_getParentsTiledImage();
			return (image != NULL ? image->getScrollY() : 0.0f);
		}

		void TiledScrollerY::_setObjectValue(float value)
		{
			TiledImage* image = this->_getParentsTiledImage();
			if (image != NULL)
			{
				image->setScrollY(value);
			}
		}

		TiledImage* TiledScrollerY::_getParentsTiledImage()
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::error(aprilui::logTag, "Animators::TiledScrollerY: parent object not a subclass of Objects::ImageBox!");
				return NULL;
			}
			TiledImage* image = dynamic_cast<TiledImage*>(imageBox->getImage());
			if (image == NULL)
			{
				hlog::error(aprilui::logTag, "Animators::TiledScrollerY: image in object not a subclass of Animators::TiledImage!");
				return NULL;
			}
			return image;
		}

		void TiledScrollerY::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}
