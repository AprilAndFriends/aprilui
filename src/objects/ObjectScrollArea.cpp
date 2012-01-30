/// @file
/// @author  Boris Mikic
/// @version 1.5
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

namespace aprilui
{
	float ScrollArea::DragThreshold = 16.0f;
	float ScrollArea::DragInertia = 1.0f;

	ScrollArea::ScrollArea(chstr name, grect rect) :
		Object(name, rect),
		ButtonBase()
	{
		mClip = true;
		mAllowDrag = false;
		mDragThreshold = DragThreshold;
		mDragInertia = DragInertia;
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
		return (!mPushed && mDragInertia > 0.0f && (_mAcceleration.x != 0.0f || _mAcceleration.y != 0.0f));
	}

	void ScrollArea::update(float k)
	{
		Object::update(k);
		if (mAllowDrag && mParent != NULL)
		{
			ButtonBase::update(k);
			gvec2 position = aprilui::getCursorPosition();
			if (mPushed && !mDragging && (fabs(_mClickPosition.x - position.x) >= mDragThreshold || fabs(_mClickPosition.y - position.y) >= mDragThreshold))
			{
				mDragging = true;
				_mClickPosition -= getPosition();
				_mLastPosition = position;
				foreach (Object*, it, mChildren)
				{
					(*it)->cancelMouseDown();
				}
			}
			if (mDragging)
			{
				setX(hclamp(position.x - _mClickPosition.x, mParent->getWidth() - getWidth(), 0.0f));
				setY(hclamp(position.y - _mClickPosition.y, mParent->getHeight() - getHeight(), 0.0f));
				_mAcceleration = (position - _mLastPosition) / k;
				_mLastPosition = position;
			}
			else if (isScrolling())
			{
				float length = _mAcceleration.length();
				float newLength = length - k * 1000 * mDragInertia;
				if (fabs(newLength) < fabs(length) && sgn(newLength) == sgn(length))
				{
					float x = getX();
					float y = getY();
					setX(hclamp((float)(int)(x + _mAcceleration.x * k), mParent->getWidth() - getWidth(), 0.0f));
					setY(hclamp((float)(int)(y + _mAcceleration.y * k), mParent->getHeight() - getHeight(), 0.0f));
					if (getX() != x || getY() != y)
					{
						_mAcceleration *= newLength / length;
					}
					else
					{
						_mAcceleration.set(0.0f, 0.0f);
					}
				}
				else
				{
					_mAcceleration.set(0.0f, 0.0f);
				}
			}
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
	}

	hstr ScrollArea::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "allow_drag")		return isAllowDrag();
		if (name == "drag_threshold")	return getDragThreshold();
		if (name == "drag_inertia")		return getDragInertia();
		return Object::getProperty(name, property_exists);
	}

	bool ScrollArea::setProperty(chstr name, chstr value)
	{
		if (name == "allow_drag")			setAllowDrag(value);
		else if (name == "drag_threshold")	setDragThreshold(value);
		else if (name == "drag_inertia")	setDragInertia(value);
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
	
}
