/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "AnimatorTileScrollerY.h"
#include "aprilui.h"
#include "ObjectImageBox.h"
#include "TileImage.h"

namespace aprilui
{
	namespace Animators
	{
		TileScrollerY::TileScrollerY(chstr name) : Animator(name)
		{
		}

		TileScrollerY::TileScrollerY(const TileScrollerY& other) : Animator(other)
		{
		}

		TileScrollerY::~TileScrollerY()
		{
		}

		Animator* TileScrollerY::createInstance(chstr name)
		{
			return new TileScrollerY(name);
		}

		float TileScrollerY::_getObjectValue()
		{
			TileImage* image = this->_getParentsTileImage();
			return (image != NULL ? image->getScrollY() : 0.0f);
		}

		void TileScrollerY::_setObjectValue(float value)
		{
			TileImage* image = this->_getParentsTileImage();
			if (image != NULL)
			{
				image->setScrollY(value);
			}
		}

		TileImage* TileScrollerY::_getParentsTileImage()
		{
			hstr parentName = (this->parent != NULL ? this->parent->getName() : "null");
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::TileScrollerY: parent object '%s' not a subclass of Objects::ImageBox!", parentName.cStr());
				return NULL;
			}
			hstr imageName = imageBox->getImageName();
			TileImage* image = dynamic_cast<TileImage*>(imageBox->getImage());
			if (image == NULL)
			{
				hlog::errorf(logTag, "Animators::TileScrollerY: Image '%s' in ImageBox '%s' not a subclass of TileImage!", imageName.cStr(), parentName.cStr());
				return NULL;
			}
			return image;
		}

		void TileScrollerY::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}
