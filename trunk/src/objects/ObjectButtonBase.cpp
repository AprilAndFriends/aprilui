/// @file
/// @version 3.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		this->hovered = false;
		this->pushed = false;
	}
	
	ButtonBase::~ButtonBase()
	{
	}
	
	void ButtonBase::update(float timeDelta)
	{
		this->_updateHover();
	}

	aprilui::Object* ButtonBase::_findHoverObject()
	{
		Object* root = NULL;
		Dataset* dataset = this->getDataset();
		if (dataset != NULL)
		{
			int focusIndex = this->getFocusIndex();
			if (focusIndex >= 0 && dataset->getFocusedObjectIndex() == focusIndex)
			{
				return dynamic_cast<Object*>(this);
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
			return (this->isCursorInside() ? dynamic_cast<Object*>(this) : NULL);
		}
		return root->getChildUnderCursor();
	}

	void ButtonBase::_updateHover()
	{
		bool previousHovered = this->hovered;
		this->hovered = (this->_findHoverObject() == dynamic_cast<Object*>(this));
		if (previousHovered != this->hovered)
		{
			this->triggerEvent(this->hovered ? Event::HoverStarted : Event::HoverFinished);
		}
	}

	bool ButtonBase::_mouseDown(april::Key keyCode)
	{
		if (!allowedKeys.contains(keyCode))
		{
			return false;
		}
		this->_updateHover();
		if (this->hovered)
		{
			this->pushed = true;
			return true;
		}
		return false;
	}

	bool ButtonBase::_mouseUp(april::Key keyCode)
	{
		if (!allowedKeys.contains(keyCode))
		{
			return false;
		}
		this->_updateHover();
		if (this->pushed && this->hovered)
		{
			this->pushed = false;
			return true;
		}
		this->pushed = false;
		return false;
	}

	void ButtonBase::_mouseCancel(april::Key keyCode)
	{
		this->pushed = false;
		this->_updateHover();
	}

	bool ButtonBase::_mouseMove()
	{
		this->_updateHover();
		return false;
	}

	bool ButtonBase::_buttonDown(april::Button buttonCode)
	{
		if (!allowedButtons.contains(buttonCode))
		{
			return false;
		}
		this->_updateHover();
		if (this->hovered)
		{
			this->pushed = true;
			return true;
		}
		return false;
	}

	bool ButtonBase::_buttonUp(april::Button buttonCode)
	{
		if (!allowedButtons.contains(buttonCode))
		{
			return false;
		}
		this->_updateHover();
		if (this->pushed && this->hovered)
		{
			this->pushed = false;
			return true;
		}
		this->pushed = false;
		return false;
	}

	hstr ButtonBase::getProperty(chstr name)
	{
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
