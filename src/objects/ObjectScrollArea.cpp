/// @file
/// @version 3.2
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

	ScrollArea::ScrollArea(chstr name, grect rect) : Object(name, rect), ButtonBase()
	{
		this->clip = true;
		this->allowDrag = false;
		this->inertia = Inertia;
		this->dragThreshold = DragThreshold;
		this->dragMaxSpeed = DragMaxSpeed;
		this->swapScrollWheels = false;
		this->dragging = false;
	}

	ScrollArea::~ScrollArea()
	{
	}

	Object* ScrollArea::createInstance(chstr name, grect rect)
	{
		return new ScrollArea(name, rect);
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
			ScrollArea::_propertyDescriptions += PropertyDescription("allow_drag", PropertyDescription::TYPE_BOOL);
			ScrollArea::_propertyDescriptions += PropertyDescription("inertia", PropertyDescription::TYPE_FLOAT);
			ScrollArea::_propertyDescriptions += PropertyDescription("drag_threshold", PropertyDescription::TYPE_FLOAT);
			ScrollArea::_propertyDescriptions += PropertyDescription("drag_max_speed", PropertyDescription::TYPE_FLOAT);
			ScrollArea::_propertyDescriptions += PropertyDescription("swap_scroll_wheels", PropertyDescription::TYPE_BOOL);
		}
		return (Object::getPropertyDescriptions() + ScrollArea::_propertyDescriptions);
	}

	void ScrollArea::_snapScrollOffset()
	{
		gvec2 offset = this->getScrollOffset();
		this->setScrollOffset(hroundf(offset.x), hroundf(offset.y));
	}

	void ScrollArea::OnDraw()
	{
		Object::OnDraw();
		if (aprilui::isDebugEnabled())
		{
			grect rect = this->_getDrawRect();
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Green, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::Yellow, 64));
		}
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
					!heqf(this->_clickPosition.x, position.x, this->dragThreshold) || !heqf(this->_clickPosition.y, position.y, this->dragThreshold)))
				{
					this->dragging = true;
					this->_clickScrollOffset = this->getScrollOffset();
					this->_lastPosition = position;
					foreach (Object*, it, this->children)
					{
						(*it)->onMouseCancel(april::AK_LBUTTON);
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
				this->_dragSpeed.x = 0.0f;
				this->_dragTimer.x = 0.0f;
			}
			if (offset.y == newOffset.y)
			{
				this->_dragSpeed.y = 0.0f;
				this->_dragTimer.y = 0.0f;
			}
		}
		else
		{
			this->_dragTimer.set(0.0f, 0.0f);
		}
	}

	bool ScrollArea::_checkHover()
	{
		return (!this->dragging ? ButtonBase::_checkHover() : this->isCursorInside());
	}

	void ScrollArea::notifyEvent(chstr name, void* params)
	{
		Object::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->_setScrollArea(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->_setScrollArea(NULL);
			}
		}
		else if (name == "Resized")
		{
			this->setScrollOffset(this->getScrollOffset());
		}
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
		if (this->allowDrag)
		{
			this->dragging = true;
			bool result = ButtonBase::onMouseDown(keyCode);
			this->dragging = false;
			if (result)
			{
				this->_clickPosition = aprilui::getCursorPosition();
			}
		}
		return Object::onMouseDown(keyCode);
	}

	bool ScrollArea::onMouseUp(april::Key keyCode)
	{
		if (this->allowDrag)
		{
			this->dragging = false;
			this->_adjustDragSpeed();
			if (ButtonBase::onMouseUp(keyCode))
			{
				return true;
			}
		}
		return Object::onMouseUp(keyCode);
	}
	
	bool ScrollArea::onMouseMove()
	{
		if (Object::onMouseMove())
		{
			return true;
		}
		if (this->allowDrag)
		{
			return ButtonBase::onMouseMove();
		}
		return false;
	}

	bool ScrollArea::onButtonDown(april::Button buttonCode)
	{
		return Object::onButtonDown(buttonCode);
	}

	bool ScrollArea::onButtonUp(april::Button buttonCode)
	{
		return Object::onButtonUp(buttonCode);
	}
	
	void ScrollArea::mouseCancel()
	{
		Object::mouseCancel();
	}
	
	// TODO - remove this temporary hack
	void ScrollArea::__stop()
	{
		this->dragging = false;
		this->pushed = false;
		this->_dragSpeed.set(0.0f, 0.0f);
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
		ScrollBar* scrollBarH = parent->_getScrollBarH();
		if (scrollBarH != NULL)
		{
			scrollBarH->_adjustDragSpeed();
		}
		ScrollBar* scrollBarV = parent->_getScrollBarV();
		if (scrollBarV != NULL)
		{
			scrollBarV->_adjustDragSpeed();
		}
	}
	
}
