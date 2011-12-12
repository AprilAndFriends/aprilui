/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
		mPushed = false;
		mHovered = false;
	}
	
	ButtonBase::~ButtonBase()
	{
	}
	
	void ButtonBase::update(float k)
	{
		mHovered = _checkHover();
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
			return isCursorInside();
		}
		return (root->getChildUnderCursor() == dynamic_cast<Object*>(this));
	}

	bool ButtonBase::onMouseDown(float x, float y, int button)
	{
		if (!allowedButtons.contains(button))
		{
			return false;
		}
		mHovered = _checkHover();
		if (mHovered)
		{
			mPushed = true;
			return true;
		}
		return false;
	}

	bool ButtonBase::onMouseUp(float x, float y, int button)
	{
		if (!allowedButtons.contains(button))
		{
			return false;
		}
		mHovered = _checkHover();
		if (mPushed && mHovered)
		{
			mPushed = false;
			return true;
		}
		mPushed = false;
		return false;
	}

	void ButtonBase::onMouseMove(float x, float y)
	{
		mHovered = _checkHover();
	}

	void ButtonBase::setAllowedButtons(harray<unsigned char> buttons)
	{
		allowedButtons = buttons;
	}
	
}
