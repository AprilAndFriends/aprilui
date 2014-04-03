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
			TiledImage* image = this->_getParentsTiledImage();
			return (image != NULL ? image->getScrollX() : 0.0f);
		}

		void TiledScrollerX::_setObjectValue(float value)
		{
			TiledImage* image = this->_getParentsTiledImage();
			if (image != NULL)
			{
				image->setScrollX(value);
			}
		}

		TiledImage* TiledScrollerX::_getParentsTiledImage()
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::error(aprilui::logTag, "Animators::TiledScrollerX: parent object not a subclass of Objects::ImageBox!");
				return NULL;
			}
			TiledImage* image = dynamic_cast<TiledImage*>(imageBox->getImage());
			if (image == NULL)
			{
				hlog::error(aprilui::logTag, "Animators::TiledScrollerX: image in object not a subclass of Animators::TiledImage!");
				return NULL;
			}
			return image;
		}

		void TiledScrollerX::update(float k)
		{
			this->_valueUpdateSimple(k);
		}
		
	}
}
