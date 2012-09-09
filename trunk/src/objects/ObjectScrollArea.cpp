/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <april/RenderSystem.h>
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

	ScrollArea::ScrollArea(chstr name, grect rect) :
		Object(name, rect),
		ButtonBase()
	{
		this->mClip = true;
		this->mAllowDrag = false;
		this->mInertia = Inertia;
		this->mDragThreshold = DragThreshold;
		this->mDragMaxSpeed = DragMaxSpeed;
		this->mSwapScrollWheels = false;
		this->mDragging = false;
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
		return (!this->mPushed && (this->_mDragSpeed.x != 0.0f || this->_mDragSpeed.y != 0.0f));
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
		this->setX(this->mParent != NULL ? hclamp(-value, hmin(this->mParent->getWidth() - this->getWidth(), 0.0f), 0.0f) : -value);
	}

	float ScrollArea::getScrollOffsetY()
	{
		return -this->getY();
	}

	void ScrollArea::setScrollOffsetY(float value)
	{
		this->setY(this->mParent != NULL ? hclamp(-value, hmin(this->mParent->getHeight() - this->getHeight(), 0.0f), 0.0f) : -value);
	}

	void ScrollArea::_snapScrollOffset()
	{
		gvec2 offset = this->getScrollOffset();
		this->setScrollOffset(hroundf(offset.x), hroundf(offset.y));
	}

	void ScrollArea::update(float k)
	{
		Object::update(k);
		if (this->mAllowDrag && this->mParent != NULL)
		{
			ButtonBase::update(k);
			gvec2 position = aprilui::getCursorPosition();
			if (this->mPushed)
			{
				if (!this->mDragging && (this->_mDragSpeed.x != 0.0f || this->_mDragSpeed.y != 0.0f ||
					!heqf(this->_mClickPosition.x, position.x, this->mDragThreshold) || !heqf(this->_mClickPosition.y, position.y, this->mDragThreshold)))
				{
					this->mDragging = true;
					this->_mClickScrollOffset = this->getScrollOffset();
					this->_mLastPosition = position;
					foreach (Object*, it, this->mChildren)
					{
						(*it)->cancelMouseDown();
					}
				}
				else
				{
					this->_mDragSpeed.set(0.0f, 0.0f);
					this->_snapScrollOffset();
				}
			}
			if (this->mDragging)
			{
				this->setScrollOffset(this->_mClickScrollOffset + (this->_mClickPosition - position) / this->getDerivedScale());
				this->_mDragSpeed = (position - this->_mLastPosition) / k;
				if (this->mDragMaxSpeed > 0.0f)
				{
					float length = this->_mDragSpeed.length();
					if (length > 0.0f && length > this->mDragMaxSpeed)
					{
						this->_mDragSpeed *= this->mDragMaxSpeed / length;
					}
				}
				this->_mLastPosition = position;
				this->_mLastScrollOffset = this->getScrollOffset();
				this->_mDragTimer.set(0.0f, 0.0f);
			}
		}
		if (!this->mDragging && this->mInertia > 0.0f && this->isScrolling())
		{
			this->_mDragTimer.x += k;
			this->_mDragTimer.y += k;
			gvec2 inertiaTime(habs(this->_mDragSpeed.x) / this->mInertia, habs(this->_mDragSpeed.y) / this->mInertia);
			gvec2 distance;
			if (this->_mDragSpeed.x != 0.0f)
			{
				if (this->_mDragTimer.x < inertiaTime.x)
				{
					distance.x = this->_mDragSpeed.x * this->_mDragTimer.x - hsgn(this->_mDragSpeed.x) * this->mInertia * (this->_mDragTimer.x * this->_mDragTimer.x * 0.5f);
				}
				else
				{
					this->_mLastScrollOffset.x -= hsgn(this->_mDragSpeed.x) * (this->mInertia * (inertiaTime.x * inertiaTime.x * 0.5f));
					this->_mDragSpeed.x = 0.0f;
					this->_mDragTimer.x = 0.0f;
				}
			}
			if (this->_mDragSpeed.y != 0.0f)
			{
				if (this->_mDragTimer.y < inertiaTime.y)
				{
					distance.y = this->_mDragSpeed.y * this->_mDragTimer.y - hsgn(this->_mDragSpeed.y) * this->mInertia * (this->_mDragTimer.y * this->_mDragTimer.y * 0.5f);
				}
				else
				{
					this->_mLastScrollOffset.y -= hsgn(this->_mDragSpeed.y) * (this->mInertia * (inertiaTime.y * inertiaTime.y * 0.5f));
					this->_mDragSpeed.y = 0.0f;
					this->_mDragTimer.y = 0.0f;
				}
			}
			this->setScrollOffset(this->_mLastScrollOffset - distance);
			this->_snapScrollOffset();
		}
		else
		{
			this->_mDragTimer.set(0.0f, 0.0f);
		}
	}

	bool ScrollArea::_checkHover()
	{
		return (!this->mDragging ? ButtonBase::_checkHover() : this->isCursorInside());
	}

	void ScrollArea::notifyEvent(chstr name, void* params)
	{
		Object::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
			if (parent != NULL)
			{
				parent->_setScrollArea(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
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

	hstr ScrollArea::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "allow_drag")			return this->isAllowDrag();
		if (name == "inertia")				return this->getInertia();
		if (name == "drag_threshold")		return this->getDragThreshold();
		if (name == "drag_max_speed")		return this->getDragMaxSpeed();
		if (name == "swap_scroll_wheels")	return this->isSwapScrollWheels();
		return Object::getProperty(name, property_exists);
	}

	bool ScrollArea::setProperty(chstr name, chstr value)
	{
		if (name == "allow_drag")				this->setAllowDrag(value);
		else if (name == "inertia")				this->setInertia(value);
		else if (name == "drag_threshold")		this->setDragThreshold(value);
		else if (name == "drag_max_speed")		this->setDragMaxSpeed(value);
		else if (name == "swap_scroll_wheels")	this->setSwapScrollWheels(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ScrollArea::onMouseDown(int button)
	{
		if (this->mAllowDrag)
		{
			this->mDragging = true;
			bool result = ButtonBase::onMouseDown(button);
			this->mDragging = false;
			if (result)
			{
				this->_mClickPosition = aprilui::getCursorPosition();
			}
		}
		return Object::onMouseDown(button);
	}

	bool ScrollArea::onMouseUp(int button)
	{
		if (this->mAllowDrag)
		{
			this->mDragging = false;
			this->_adjustDragSpeed();
			if (ButtonBase::onMouseUp(button))
			{
				return true;
			}
		}
		return Object::onMouseUp(button);
	}
	
	void ScrollArea::onMouseMove()
	{
		if (this->mAllowDrag)
		{
			ButtonBase::onMouseMove();
		}
		Object::onMouseMove();
	}

	void ScrollArea::_adjustDragSpeed()
	{
		this->_mLastScrollOffset = this->getScrollOffset();
		this->_mDragTimer.set(0.0f, 0.0f);
		Container* parent = dynamic_cast<Container*>(this->mParent);
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
