/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Keys.h>
#include <april/Window.h>
#include <hltypes/harray.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Object.h"
#include "ObjectButtonBase.h"

// because the derived class' object has not been created yet in this class' constructor
#define _THIS_OBJECT (this->_thisObject != NULL ? this->_thisObject : this->_thisObject = dynamic_cast<Object*>(this))

namespace aprilui
{
	harray<PropertyDescription> ButtonBase::_propertyDescriptions;

	harray<april::Key> ButtonBase::allowedKeys;
	harray<april::Button> ButtonBase::allowedButtons;

	ButtonBase::ButtonBase() : _thisObject(NULL)
	{
		this->pushDeadZone = -1.0f;
		this->hoverColor = april::Color::White;
		this->pushedColor = april::Color::White;
		this->disabledColor = april::Color::White;
		this->hovered = false;
		this->pushed = false;
		this->_useHoverColor = false;
		this->_usePushedColor = false;
		this->_useDisabledColor = false;
	}

	ButtonBase::ButtonBase(const ButtonBase& other) : _thisObject(NULL)
	{
		this->pushDeadZone = other.pushDeadZone;
		this->hoverColor = other.hoverColor;
		this->pushedColor = other.pushedColor;
		this->disabledColor = other.disabledColor;
		this->hovered = other.hovered;
		this->pushed = other.pushed;
		this->_useHoverColor = other._useHoverColor;
		this->_usePushedColor = other._usePushedColor;
		this->_useDisabledColor = other._useDisabledColor;
	}
	
	ButtonBase::~ButtonBase()
	{
	}
	
	void ButtonBase::setHoverColor(const april::Color& value)
	{
		this->hoverColor = value;
		this->_useHoverColor = true;
	}

	void ButtonBase::setHoverSymbolicColor(chstr value)
	{
		this->setHoverColor(aprilui::_makeColor(value));
	}

	void ButtonBase::setPushedColor(const april::Color& value)
	{
		this->pushedColor = value;
		this->_usePushedColor = true;
	}

	void ButtonBase::setPushedSymbolicColor(chstr value)
	{
		this->setPushedColor(aprilui::_makeColor(value));
	}

	void ButtonBase::setDisabledColor(const april::Color& value)
	{
		this->disabledColor = value;
		this->_useDisabledColor = true;
	}

	void ButtonBase::setDisabledSymbolicColor(chstr value)
	{
		this->setDisabledColor(aprilui::_makeColor(value));
	}

	harray<PropertyDescription> ButtonBase::getPropertyDescriptions() const
	{
		if (ButtonBase::_propertyDescriptions.size() == 0)
		{
			ButtonBase::_propertyDescriptions += PropertyDescription("push_dead_zone", PropertyDescription::Type::Float);
			ButtonBase::_propertyDescriptions += PropertyDescription("hover_color", PropertyDescription::Type::Color);
			ButtonBase::_propertyDescriptions += PropertyDescription("pushed_color", PropertyDescription::Type::Color);
			ButtonBase::_propertyDescriptions += PropertyDescription("disabled_color", PropertyDescription::Type::Color);
		}
		return ButtonBase::_propertyDescriptions;
	}

	void ButtonBase::_update(float timeDelta)
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
				return _THIS_OBJECT;
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
			return (this->isCursorInside() ? _THIS_OBJECT : NULL);
		}
		return root->getChildUnderCursor();
	}

	void ButtonBase::_updateHover()
	{
		bool previousHovered = this->hovered;
		Object* thisObject = _THIS_OBJECT;
		if (thisObject->isDerivedEnabled())
		{
			this->hovered = (this->_findHoverObject() == thisObject);
		}
		else
		{
			this->hovered = false;
			this->pushed = false;
		}
		if (previousHovered != this->hovered)
		{
			this->triggerEvent(this->hovered ? Event::HoverStarted : Event::HoverFinished);
		}
	}

	hstr ButtonBase::getProperty(chstr name)
	{
		if (name == "push_dead_zone")	return this->getPushDeadZone();
		if (name == "hover_color")		return this->getHoverColor().hex();
		if (name == "pushed_color")		return this->getPushedColor().hex();
		if (name == "disabled_color")	return this->getDisabledColor().hex();
		return "";
	}

	bool ButtonBase::setProperty(chstr name, chstr value)
	{
		if (name == "push_dead_zone")		this->setPushDeadZone(value);
		else if (name == "hover_color")		this->setHoverColor(aprilui::_makeColor(value));
		else if (name == "pushed_color")	this->setPushedColor(aprilui::_makeColor(value));
		else if (name == "disabled_color")	this->setDisabledColor(aprilui::_makeColor(value));
		else return false;
		return true;
	}

	bool ButtonBase::_mouseDown(april::Key keyCode)
	{
		if (!allowedKeys.has(keyCode))
		{
			return false;
		}
		this->_updateHover();
		if (this->hovered)
		{
			this->pushed = true;
			this->_mouseDownPosition = this->_thisObject->transformToLocalSpace(aprilui::getCursorPosition());
			return true;
		}
		return false;
	}

	bool ButtonBase::_mouseUp(april::Key keyCode)
	{
		if (!allowedKeys.has(keyCode))
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
		if (this->pushDeadZone >= 0.0f)
		{
			if ((this->_mouseDownPosition - this->_thisObject->transformToLocalSpace(aprilui::getCursorPosition())).squaredLength() > this->pushDeadZone * this->pushDeadZone)
			{
				this->_mouseCancel(april::Key::None);
			}
		}
		return false;
	}

	bool ButtonBase::_buttonDown(april::Button buttonCode)
	{
		if (!allowedButtons.has(buttonCode))
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
		if (!allowedButtons.has(buttonCode))
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

}
