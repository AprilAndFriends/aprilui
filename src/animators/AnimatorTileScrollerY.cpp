/// @file
/// @version 5.1
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
		TileScrollerY::TileScrollerY(chstr name) :
			Animator(name)
		{
		}

		TileScrollerY::TileScrollerY(const TileScrollerY& other) :
			Animator(other)
		{
		}

		Animator* TileScrollerY::createInstance(chstr name)
		{
			return new TileScrollerY(name);
		}

		float TileScrollerY::_getObjectValue() const
		{
			ImageBox* imageBox = this->_getValidParent();
			return (imageBox != NULL ? imageBox->getTileScrollY() : 0.0f);
		}

		void TileScrollerY::_setObjectValue(float value)
		{
			ImageBox* imageBox = this->_getValidParent();
			if (imageBox != NULL)
			{
				imageBox->setTileScrollY(value);
			}
		}

		ImageBox* TileScrollerY::_getValidParent() const
		{
			hstr parentName = (this->parent != NULL ? this->parent->getName() : "NULL");
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
			return imageBox;
		}

		void TileScrollerY::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}
