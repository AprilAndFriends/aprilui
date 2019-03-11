/// @file
/// @version 5.1
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
		TileScrollerX::TileScrollerX(chstr name) :
			Animator(name)
		{
		}

		TileScrollerX::TileScrollerX(const TileScrollerX& other) :
			Animator(other)
		{
		}

		Animator* TileScrollerX::createInstance(chstr name)
		{
			return new TileScrollerX(name);
		}

		float TileScrollerX::_getObjectValue() const
		{
			ImageBox* imageBox = this->_getValidParent();
			return (imageBox != NULL ? imageBox->getTileScrollX() : 0.0f);
		}

		void TileScrollerX::_setObjectValue(float value)
		{
			ImageBox* imageBox = this->_getValidParent();
			if (imageBox != NULL)
			{
				imageBox->setTileScrollX(value);
			}
		}

		ImageBox* TileScrollerX::_getValidParent() const
		{
			hstr parentName = (this->parent != NULL ? this->parent->getName() : "NULL");
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::TileScrollerX: parent object '%s' not a subclass of Objects::ImageBox!", parentName.cStr());
				return NULL;
			}
			return imageBox;
		}

		void TileScrollerX::_update(float timeDelta)
		{
			this->_valueUpdateSimple(timeDelta);
		}
		
	}
}
