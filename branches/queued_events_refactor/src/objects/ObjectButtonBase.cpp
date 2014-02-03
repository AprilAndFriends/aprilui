/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Keys.h>
#include <april/Window.h>
#include <hltypes/harray.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Object.h"
#include "ObjectButtonBase.h"

namespace aprilui
{
	static harray<april::Key> allowedKeys;
	static harray<april::Button> allowedButtons;

	ButtonBase::ButtonBase()
	{
		this->mHovered = false;
		this->mPushed = false;
	}
	
	ButtonBase::~ButtonBase()
	{
	}
	
	void ButtonBase::update(float k)
	{
		this->mHovered = this->_checkHover();
	}

	bool ButtonBase::_checkHover()
	{
		Object* root = NULL;
		Dataset* dataset = this->getDataset();
		if (dataset != NULL)
		{
			int focusIndex = this->getFocusIndex();
			if (focusIndex >= 0 && dataset->getFocusedObjectIndex() == focusIndex)
			{
				return true;
			}
			root = dataset->getRoot();
		}
		if (root == NULL)
		{
			Object* parent = this->getParent();
			while (parent != NULL)
			{
				root = parent;
				parent = root->getParent();
			}
		}
		if (root == NULL)
		{
			return this->isCursorInside();
		}
		Object* child = root->getChildUnderCursor();
		return (child != NULL && child == dynamic_cast<Object*>(this));
	}

	bool ButtonBase::onMouseDown(april::Key keyCode)
	{
		if (!allowedKeys.contains(keyCode))
		{
			return false;
		}
		this->mHovered = this->_checkHover();
		if (this->mHovered)
		{
			this->mPushed = true;
			return true;
		}
		return false;
	}

	bool ButtonBase::onMouseUp(april::Key keyCode)
	{
		if (!allowedKeys.contains(keyCode))
		{
			return false;
		}
		this->mHovered = this->_checkHover();
		if (this->mPushed && this->mHovered)
		{
			this->mPushed = false;
			return true;
		}
		this->mPushed = false;
		return false;
	}

	bool ButtonBase::onMouseMove()
	{
		this->mHovered = this->_checkHover();
		return false;
	}

	bool ButtonBase::onButtonDown(april::Button buttonCode)
	{
		if (!allowedButtons.contains(buttonCode))
		{
			return false;
		}
		this->mHovered = this->_checkHover();
		if (this->mHovered)
		{
			this->mPushed = true;
			return true;
		}
		return false;
	}

	bool ButtonBase::onButtonUp(april::Button buttonCode)
	{
		if (!allowedButtons.contains(buttonCode))
		{
			return false;
		}
		this->mHovered = this->_checkHover();
		if (this->mPushed && this->mHovered)
		{
			this->mPushed = false;
			return true;
		}
		this->mPushed = false;
		return false;
	}

	void ButtonBase::cancelMouseDown()
	{
		this->mPushed = false;
		this->mHovered = this->_checkHover();
	}

	hstr ButtonBase::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = false;
		}
		return "";
	}
	
	bool ButtonBase::setProperty(chstr name, chstr value)
	{
		return false;
	}
	
	void ButtonBase::setAllowedKeys(harray<april::Key> keys)
	{
		allowedKeys = keys;
	}
	
	void ButtonBase::setAllowedButtons(harray<april::Button> buttons)
	{
		allowedButtons = buttons;
	}
	
}