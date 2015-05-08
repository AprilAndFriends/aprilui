/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "AnimatorTileScrollerX.h"
#include "aprilui.h"
#include "ObjectImageBox.h"
#include "TileImage.h"

namespace aprilui
{
	namespace Animators
	{
		TileScrollerX::TileScrollerX(chstr name) : Animator(name)
		{
		}

		TileScrollerX::TileScrollerX(const TileScrollerX& other) : Animator(other)
		{
		}

		TileScrollerX::~TileScrollerX()
		{
		}

		Animator* TileScrollerX::createInstance(chstr name)
		{
			return new TileScrollerX(name);
		}

		float TileScrollerX::_getObjectValue()
		{
			TileImage* image = this->_getParentsTileImage();
			return (image != NULL ? image->getScrollX() : 0.0f);
		}

		void TileScrollerX::_setObjectValue(float value)
		{
			TileImage* image = this->_getParentsTileImage();
			if (image != NULL)
			{
				image->setScrollX(value);
			}
		}

		TileImage* TileScrollerX::_getParentsTileImage()
		{
			hstr parentName = (this->parent != NULL ? this->parent->getName() : "null");
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::TileScrollerX: parent object '%s' not a subclass of Objects::ImageBox!", parentName.cStr());
				return NULL;
			}
			hstr imageName = (imageBox->getImage() != NULL ? imageBox->getImage()->getName() : "null");
			TileImage* image = dynamic_cast<TileImage*>(imageBox->getImage());
			if (image == NULL)
			{
				hlog::errorf(logTag, "Animators::TileScrollerX: Image '%s' in ImageBox '%s' not a subclass of TileImage!", imageName.cStr(), parentName.cStr());
				return NULL;
			}
			return image;
		}

		void TileScrollerX::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}
