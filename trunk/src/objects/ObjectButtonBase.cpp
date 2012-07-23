/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Keys.h>
#include <hltypes/harray.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Object.h"
#include "ObjectButtonBase.h"

namespace aprilui
{
	static harray<unsigned char> allowedButtons;

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

	bool ButtonBase::onMouseDown(int button)
	{
		if (!allowedButtons.contains(button))
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

	bool ButtonBase::onMouseUp(int button)
	{
		if (!allowedButtons.contains(button))
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

	void ButtonBase::onMouseMove()
	{
		this->mHovered = this->_checkHover();
	}

	void ButtonBase::cancelMouseDown()
	{
		this->mPushed = false;
		this->mHovered = this->_checkHover();
	}

	void ButtonBase::setAllowedButtons(harray<unsigned char> buttons)
	{
		allowedButtons = buttons;
	}
	
}
