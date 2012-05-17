/// @file
/// @author  Boris Mikic
/// @version 1.61
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
		mClip = true;
		mAllowDrag = false;
		mInertia = Inertia;
		mDragThreshold = DragThreshold;
		mDragMaxSpeed = DragMaxSpeed;
		mSwapScrollWheels = false;
		mDragging = false;
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
		return (!mPushed && (_mDragSpeed.x != 0.0f || _mDragSpeed.y != 0.0f));
	}

	gvec2 ScrollArea::getScrollOffset()
	{
		return -getPosition();
	}

	void ScrollArea::setScrollOffset(gvec2 value)
	{
		setScrollOffsetX(value.x);
		setScrollOffsetY(value.y);
	}

	void ScrollArea::setScrollOffset(float x, float y)
	{
		setScrollOffsetX(x);
		setScrollOffsetY(y);
	}

	float ScrollArea::getScrollOffsetX()
	{
		return -getX();
	}

	void ScrollArea::setScrollOffsetX(float value)
	{
		setX(mParent != NULL ? hclamp(-value, hmin(mParent->getWidth() - getWidth(), 0.0f), 0.0f) : -value);
	}

	float ScrollArea::getScrollOffsetY()
	{
		return -getY();
	}

	void ScrollArea::setScrollOffsetY(float value)
	{
		setY(mParent != NULL ? hclamp(-value, hmin(mParent->getHeight() - getHeight(), 0.0f), 0.0f) : -value);
	}

	void ScrollArea::_snapScrollOffset()
	{
		gvec2 offset = getScrollOffset();
		setScrollOffset(hroundf(offset.x), hroundf(offset.y));
	}

	void ScrollArea::update(float k)
	{
		Object::update(k);
		if (mAllowDrag && mParent != NULL)
		{
			ButtonBase::update(k);
			gvec2 position = aprilui::getCursorPosition();
			if (mPushed)
			{
				if (!mDragging && (_mDragSpeed.x != 0.0f || _mDragSpeed.y != 0.0f ||
					!heqf(_mClickPosition.x, position.x, mDragThreshold) || !heqf(_mClickPosition.y, position.y, mDragThreshold)))
				{
					mDragging = true;
					_mClickPosition -= getPosition();
					_mLastPosition = position;
					foreach (Object*, it, mChildren)
					{
						(*it)->cancelMouseDown();
					}
				}
				else
				{
					_mDragSpeed.set(0.0f, 0.0f);
					_snapScrollOffset();
				}
			}
			if (mDragging)
			{
				setScrollOffset(_mClickPosition - position);
				_mDragSpeed = (position - _mLastPosition) / k;
				if (mDragMaxSpeed > 0.0f)
				{
					float length = _mDragSpeed.length();
					if (length > 0.0f && length > mDragMaxSpeed)
					{
						_mDragSpeed *= mDragMaxSpeed / length;
					}
				}
				_mLastPosition = position;
				_mLastScrollOffset = getScrollOffset();
				_mDragTimer.set(0.0f, 0.0f);
			}
		}
		if (!mDragging && mInertia > 0.0f && isScrolling())
		{
			_mDragTimer.x += k;
			_mDragTimer.y += k;
			gvec2 inertiaTime(habs(_mDragSpeed.x) / mInertia, habs(_mDragSpeed.y) / mInertia);
			gvec2 distance;
			if (_mDragSpeed.x != 0.0f)
			{
				if (_mDragTimer.x < inertiaTime.x)
				{
					distance.x = _mDragSpeed.x * _mDragTimer.x - hsgn(_mDragSpeed.x) * mInertia * (_mDragTimer.x * _mDragTimer.x * 0.5f);
				}
				else
				{
					_mLastScrollOffset.x -= hsgn(_mDragSpeed.x) * (mInertia * (inertiaTime.x * inertiaTime.x * 0.5f));
					_mDragSpeed.x = 0.0f;
					_mDragTimer.x = 0.0f;
				}
			}
			if (_mDragSpeed.y != 0.0f)
			{
				if (_mDragTimer.y < inertiaTime.y)
				{
					distance.y = _mDragSpeed.y * _mDragTimer.y - hsgn(_mDragSpeed.y) * mInertia * (_mDragTimer.y * _mDragTimer.y * 0.5f);
				}
				else
				{
					_mLastScrollOffset.y -= hsgn(_mDragSpeed.y) * (mInertia * (inertiaTime.y * inertiaTime.y * 0.5f));
					_mDragSpeed.y = 0.0f;
					_mDragTimer.y = 0.0f;
				}
			}
			setScrollOffset(_mLastScrollOffset - distance);
			_snapScrollOffset();
		}
		else
		{
			_mDragTimer.set(0.0f, 0.0f);
		}
	}

	bool ScrollArea::_checkHover()
	{
		return (!mDragging ? ButtonBase::_checkHover() : isCursorInside());
	}

	void ScrollArea::notifyEvent(chstr name, void* params)
	{
		Object::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollArea(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollArea(NULL);
			}
		}
		else if (name == "Resized")
		{
			setScrollOffset(getScrollOffset());
		}
	}

	hstr ScrollArea::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "allow_drag")			return isAllowDrag();
		if (name == "inertia")				return getInertia();
		if (name == "drag_threshold")		return getDragThreshold();
		if (name == "drag_max_speed")		return getDragMaxSpeed();
		if (name == "swap_scroll_wheels")	return isSwapScrollWheels();
		return Object::getProperty(name, property_exists);
	}

	bool ScrollArea::setProperty(chstr name, chstr value)
	{
		if (name == "allow_drag")				setAllowDrag(value);
		else if (name == "inertia")				setInertia(value);
		else if (name == "drag_threshold")		setDragThreshold(value);
		else if (name == "drag_max_speed")		setDragMaxSpeed(value);
		else if (name == "swap_scroll_wheels")	setSwapScrollWheels(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ScrollArea::onMouseDown(float x, float y, int button)
	{
		if (mAllowDrag)
		{
			mDragging = true;
			bool result = ButtonBase::onMouseDown(x, y, button);
			mDragging = false;
			if (result)
			{
				_mClickPosition = aprilui::getCursorPosition();
			}
		}
		return Object::onMouseDown(x, y, button);
	}

	bool ScrollArea::onMouseUp(float x, float y, int button)
	{
		if (mAllowDrag)
		{
			mDragging = false;
			_adjustDragSpeed();
			if (ButtonBase::onMouseUp(x, y, button))
			{
				return true;
			}
		}
		return Object::onMouseUp(x, y, button);
	}
	
	void ScrollArea::onMouseMove(float x, float y)
	{
		if (mAllowDrag)
		{
			ButtonBase::onMouseMove(x, y);
		}
		Object::onMouseMove(x, y);
	}

	void ScrollArea::_adjustDragSpeed()
	{
		_mLastScrollOffset = getScrollOffset();
		_mDragTimer.set(0.0f, 0.0f);
		Container* parent = dynamic_cast<Container*>(mParent);
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
