/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <april/Keys.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectContainer.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBar.h"
#include "ObjectScrollBarH.h"
#include "ObjectScrollBarV.h"

namespace aprilui
{
	float ScrollArea::DragThreshold = 16.0f;
	float ScrollArea::Inertia = 2000.0f;
	float ScrollArea::DragMaxSpeed = 0.0f;

	harray<PropertyDescription> ScrollArea::_propertyDescriptions;

	ScrollArea::ScrollArea(chstr name) : Object(name), ButtonBase()
	{
		this->clip = true;
		this->allowDrag = false;
		this->inertia = Inertia;
		this->dragThreshold = DragThreshold;
		this->dragMaxSpeed = DragMaxSpeed;
		this->swapScrollWheels = false;
		this->dragging = false;
		this->debugColor = april::Color(april::Color::Yellow, 32);
		this->_overrideHoverMode = false;
	}

	ScrollArea::ScrollArea(const ScrollArea& other) : Object(other), ButtonBase(other)
	{
		this->allowDrag = other.allowDrag;
		this->inertia = other.inertia;
		this->dragThreshold = other.dragThreshold;
		this->dragMaxSpeed = other.dragMaxSpeed;
		this->swapScrollWheels = other.swapScrollWheels;
		this->dragging = false;
		this->_overrideHoverMode = false;
	}

	ScrollArea::~ScrollArea()
	{
	}

	Object* ScrollArea::createInstance(chstr name)
	{
		return new ScrollArea(name);
	}

	hstr ScrollArea::getName()
	{
		return Object::getName();
	}

	int ScrollArea::getFocusIndex()
	{
		return Object::getFocusIndex();
	}

	bool ScrollArea::isCursorInside()
	{
		return Object::isCursorInside();
	}

	Object* ScrollArea::getParent()
	{
		return Object::getParent();
	}

	Dataset* ScrollArea::getDataset()
	{
		return Object::getDataset();
	}

	bool ScrollArea::isScrolling()
	{
		return (!this->pushed && (this->_dragSpeed.x != 0.0f || this->_dragSpeed.y != 0.0f));
	}
	
	bool ScrollArea::isScrollable()
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		return (parent != NULL && (this->rect.w > parent->rect.w || this->rect.h > parent->rect.h));
	}
	
	bool ScrollArea::isScrollableX()
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		return (parent != NULL && this->rect.w > parent->rect.w);
	}
	
	bool ScrollArea::isScrollableY()
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		return (parent != NULL && this->rect.h > parent->rect.h);
	}

	gvec2 ScrollArea::getScrollOffset()
	{
		return -this->getPosition();
	}

	void ScrollArea::setScrollOffset(gvec2 value)
	{
		this->setScrollOffsetX(value.x);
		this->setScrollOffsetY(value.y);
	}

	void ScrollArea::setScrollOffset(float x, float y)
	{
		this->setScrollOffsetX(x);
		this->setScrollOffsetY(y);
	}

	float ScrollArea::getScrollOffsetX()
	{
		return -this->getX();
	}

	void ScrollArea::setScrollOffsetX(float value)
	{
		this->setX(this->parent != NULL ? hclamp(-value, hmin(this->parent->getWidth() - this->getWidth(), 0.0f), 0.0f) : -value);
	}

	float ScrollArea::getScrollOffsetY()
	{
		return -this->getY();
	}

	void ScrollArea::setScrollOffsetY(float value)
	{
		this->setY(this->parent != NULL ? hclamp(-value, hmin(this->parent->getHeight() - this->getHeight(), 0.0f), 0.0f) : -value);
	}

	harray<PropertyDescription> ScrollArea::getPropertyDescriptions()
	{
		if (ScrollArea::_propertyDescriptions.size() == 0)
		{
			ScrollArea::_propertyDescriptions += PropertyDescription("allow_drag", PropertyDescription::BOOL);
			ScrollArea::_propertyDescriptions += PropertyDescription("inertia", PropertyDescription::FLOAT);
			ScrollArea::_propertyDescriptions += PropertyDescription("drag_threshold", PropertyDescription::FLOAT);
			ScrollArea::_propertyDescriptions += PropertyDescription("drag_max_speed", PropertyDescription::FLOAT);
			ScrollArea::_propertyDescriptions += PropertyDescription("swap_scroll_wheels", PropertyDescription::BOOL);
		}
		return (Object::getPropertyDescriptions() + ScrollArea::_propertyDescriptions);
	}

	void ScrollArea::_snapScrollOffset()
	{
		gvec2 offset = this->getScrollOffset();
		this->setScrollOffset(hroundf(offset.x), hroundf(offset.y));
	}

	void ScrollArea::update(float timeDelta)
	{
		Object::update(timeDelta);
		if (this->allowDrag && this->parent != NULL)
		{
			ButtonBase::update(timeDelta);
			gvec2 position = aprilui::getCursorPosition();
			if (this->pushed)
			{
				if (!this->dragging && (this->_dragSpeed.x != 0.0f || this->_dragSpeed.y != 0.0f ||
					!heqf(this->_clickPosition.x, position.x, this->dragThreshold) || !heqf(this->_clickPosition.y, position.y, this->dragThreshold)) && this->isScrollable())
				{
					this->dragging = true;
					this->_clickScrollOffset = this->getScrollOffset();
					this->_lastPosition = position;
					foreach (Object*, it, this->childrenObjects)
					{
						(*it)->onMouseCancel(april::AK_NONE);
					}
				}
				else
				{
					this->_dragSpeed.set(0.0f, 0.0f);
					this->_snapScrollOffset();
				}
			}
			if (this->dragging)
			{
				// some devices have a bad digitizer so this serves as special hack to smoothen the dragging movement on the scroll-area
				if (april::window->getInputMode() == april::Window::TOUCH && this->_lastPosition != position)
				{
					position = this->_lastPosition + (position - this->_lastPosition) * 0.5f;
				}
				this->setScrollOffset(this->_clickScrollOffset + (this->_clickPosition - position) / this->getDerivedScale());
				this->_dragSpeed = (position - this->_lastPosition) / timeDelta;
				if (this->dragMaxSpeed > 0.0f)
				{
					float length = this->_dragSpeed.length();
					if (length > 0.0f && length > this->dragMaxSpeed)
					{
						this->_dragSpeed *= this->dragMaxSpeed / length;
					}
				}
				this->_lastPosition = position;
				this->_lastScrollOffset = this->getScrollOffset();
				this->_dragTimer.set(0.0f, 0.0f);
				this->_adjustDragSpeed();
			}
		}
		if (!this->dragging && this->inertia > 0.0f && this->isScrolling())
		{
			this->_dragTimer.x += timeDelta;
			this->_dragTimer.y += timeDelta;
			gvec2 inertiaTime(habs(this->_dragSpeed.x) / this->inertia, habs(this->_dragSpeed.y) / this->inertia);
			gvec2 distance;
			if (this->_dragSpeed.x != 0.0f)
			{
				if (this->_dragTimer.x < inertiaTime.x)
				{
					distance.x = this->_dragSpeed.x * this->_dragTimer.x - hsgn(this->_dragSpeed.x) * this->inertia * (this->_dragTimer.x * this->_dragTimer.x * 0.5f);
				}
				else
				{
					this->_lastScrollOffset.x -= hsgn(this->_dragSpeed.x) * (this->inertia * (inertiaTime.x * inertiaTime.x * 0.5f));
					this->_dragSpeed.x = 0.0f;
					this->_dragTimer.x = 0.0f;
				}
			}
			if (this->_dragSpeed.y != 0.0f)
			{
				if (this->_dragTimer.y < inertiaTime.y)
				{
					distance.y = this->_dragSpeed.y * this->_dragTimer.y - hsgn(this->_dragSpeed.y) * this->inertia * (this->_dragTimer.y * this->_dragTimer.y * 0.5f);
				}
				else
				{
					this->_lastScrollOffset.y -= hsgn(this->_dragSpeed.y) * (this->inertia * (inertiaTime.y * inertiaTime.y * 0.5f));
					this->_dragSpeed.y = 0.0f;
					this->_dragTimer.y = 0.0f;
				}
			}
			gvec2 offset = this->getScrollOffset();
			this->setScrollOffset(this->_lastScrollOffset - distance);
			this->_snapScrollOffset();
			gvec2 newOffset = this->getScrollOffset();
			if (offset.x == newOffset.x)
			{
				this->_lastScrollOffset.x = offset.x;
				this->_dragSpeed.x = 0.0f;
				this->_dragTimer.x = 0.0f;
			}
			if (offset.y == newOffset.y)
			{
				this->_lastScrollOffset.y = offset.y;
				this->_dragSpeed.y = 0.0f;
				this->_dragTimer.y = 0.0f;
			}
		}
		else
		{
			this->_dragTimer.set(0.0f, 0.0f);
		}
	}

    bool ScrollArea::_isScrollableScrollArea(aprilui::Object* object)
    {
        ScrollArea* scrollArea = dynamic_cast<ScrollArea*>(object);
		return (scrollArea != NULL && scrollArea->isScrollable());
	}
    
	aprilui::Object* ScrollArea::_findHoverObject()
	{
		if (this->dragging)
		{
			return (this->isCursorInside() ? this : NULL);
		}
		aprilui::Object* child = ButtonBase::_findHoverObject();
		if (this->_overrideHoverMode && child != this && !this->_isScrollableScrollArea(child) && child != NULL)
		{
			aprilui::Object* parent = child->getParent();
			while (parent != NULL)
			{
				if (parent == this)
				{
					return (this->isCursorInside() ? this : NULL);
				}
                if (this->_isScrollableScrollArea(parent) && parent->getChildUnderCursor() == child)
				{
					return child;
				}
				parent = parent->getParent();
			}
		}
		return child;
	}

	void ScrollArea::notifyEvent(chstr type, EventArgs* args)
	{
		Object::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->scrollArea = this;
			}
		}
		else if (type == Event::DetachedFromObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->scrollArea = NULL;
			}
		}
		else if (type == Event::Resized)
		{
			this->setScrollOffset(this->getScrollOffset());
		}
	}

	bool ScrollArea::triggerEvent(chstr type, april::Key keyCode)
	{
		return Object::triggerEvent(type, keyCode);
	}

	bool ScrollArea::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Object::triggerEvent(type, keyCode, string);
	}

	bool ScrollArea::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		return Object::triggerEvent(type, keyCode, position, string, userData);
	}

	bool ScrollArea::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return Object::triggerEvent(type, buttonCode, string, userData);
	}

	bool ScrollArea::triggerEvent(chstr type, chstr string, void* userData)
	{
		return Object::triggerEvent(type, string, userData);
	}

	bool ScrollArea::triggerEvent(chstr type, void* userData)
	{
		return Object::triggerEvent(type, userData);
	}

	hstr ScrollArea::getProperty(chstr name)
	{
		if (name == "allow_drag")			return this->isAllowDrag();
		if (name == "inertia")				return this->getInertia();
		if (name == "drag_threshold")		return this->getDragThreshold();
		if (name == "drag_max_speed")		return this->getDragMaxSpeed();
		if (name == "swap_scroll_wheels")	return this->isSwapScrollWheels();
		hstr result = ButtonBase::getProperty(name);
		if (result == "")
		{
			result = Object::getProperty(name);
		}
		return result;
	}

	bool ScrollArea::setProperty(chstr name, chstr value)
	{
		if		(name == "allow_drag")			this->setAllowDrag(value);
		else if (name == "inertia")				this->setInertia(value);
		else if (name == "drag_threshold")		this->setDragThreshold(value);
		else if (name == "drag_max_speed")		this->setDragMaxSpeed(value);
		else if (name == "swap_scroll_wheels")	this->setSwapScrollWheels(value);
		else if (ButtonBase::setProperty(name, value)) { }
		else return Object::setProperty(name, value);
		return true;
	}

	bool ScrollArea::onMouseDown(april::Key keyCode)
	{
		// has to override its children which is why onMouseDown() is overriden and not _mouseDown()
		if (this->allowDrag && this->hitTest != HIT_TEST_DISABLED_RECURSIVE && this->isVisible() && this->isDerivedEnabled())
		{
			this->_overrideHoverMode = true;
			bool result = ButtonBase::_mouseDown(keyCode);
			this->_overrideHoverMode = false;
			if (result)
			{
				this->_clickPosition = aprilui::getCursorPosition();
			}
		}
		return Object::onMouseDown(keyCode);
	}

	bool ScrollArea::onMouseUp(april::Key keyCode)
	{
		// has to override its children which is why onMouseUp() is overriden and not _mouseUp()
		if (this->allowDrag && this->hitTest != HIT_TEST_DISABLED_RECURSIVE && this->isVisible() && this->isDerivedEnabled())
		{
			this->dragging = false;
			this->_adjustDragSpeed();
			if (ButtonBase::_mouseUp(keyCode))
			{
				return true;
			}
		}
		return Object::onMouseUp(keyCode);
	}
	
	void ScrollArea::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		Object::_mouseCancel(keyCode);
	}

	bool ScrollArea::_mouseMove()
	{
		if (this->allowDrag)
		{
			return ButtonBase::_mouseMove();
		}
		return Object::_mouseMove();
	}

	bool ScrollArea::_mouseScroll(float x, float y)
	{
		bool result = false;
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent != NULL && parent->isCursorInside())
		{
			if (this->swapScrollWheels)
			{
				hswap(x, y);
			}
			if (parent->scrollBarV != NULL)
			{
				parent->scrollBarV->addScrollValue(parent->scrollBarV->_calcScrollMove(x, y));
				result = true;
			}
			if (parent->scrollBarH != NULL)
			{
				parent->scrollBarH->addScrollValue(parent->scrollBarH->_calcScrollMove(x, y));
				result = true;
			}
		}
		return (result || Object::_mouseScroll(x, y));
	}

	bool ScrollArea::_buttonDown(april::Button buttonCode)
	{
		return (ButtonBase::_buttonDown(buttonCode) || Object::_buttonDown(buttonCode));
	}

	bool ScrollArea::_buttonUp(april::Button buttonCode)
	{
		return (ButtonBase::_buttonUp(buttonCode) || Object::_buttonUp(buttonCode));
	}
	
	// TODO - remove this temporary hack
	void ScrollArea::__stop()
	{
		this->dragging = false;
		this->pushed = false;
		this->_dragSpeed.set(0.0f, 0.0f);
		this->_dragTimer.set(0.0f, 0.0f);
	}
	
	void ScrollArea::_adjustDragSpeed()
	{
		this->_lastScrollOffset = this->getScrollOffset();
		this->_dragTimer.set(0.0f, 0.0f);
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL)
		{
			return;
		}
		if (parent->scrollBarH != NULL)
		{
			parent->scrollBarH->_adjustDragSpeed();
		}
		if (parent->scrollBarV != NULL)
		{
			parent->scrollBarV->_adjustDragSpeed();
		}
	}
	
}
