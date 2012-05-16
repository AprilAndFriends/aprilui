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
#include "apriluiUtil.h"
#include "EventArgs.h"
#include "EventUtils.h"
#include "ObjectContainer.h"
#include "ObjectImageButton.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBar.h"

#define RETAIN_TIME 1.0f
#define FADE_OUT_TIME 0.25f

namespace aprilui
{
	float ScrollBar::ScrollDistance = 32.0f;
	float ScrollBar::GridSize = 0.0f;

	ScrollBar::ScrollBar(chstr name, grect rect) :
		Object(name, rect)
	{
		mGridSize = GridSize;
		mButtonBegin = NULL;
		mButtonEnd = NULL;
		mButtonBack = NULL;
		mButtonBar = NULL;
		_mClickPosition.set(0.0f, 0.0f);
		_mRetainTime = 0.0f;
	}

	ScrollBar::~ScrollBar()
	{
	}

	void ScrollBar::setSkinName(chstr value)
	{
		mSkinName = value;
		notifyEvent("SkinChange", NULL);
	}

	void ScrollBar::update(float k)
	{
		Object::update(k);
		_updateBar();
		if (mSkinName == "")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL && _mRetainTime > 0.0f)
				{
					_mRetainTime -= k;
				}
			}
		}
	}

	void ScrollBar::_initAreaDragging()
	{
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent != NULL)
		{
			ScrollArea* area = parent->_getScrollArea();
			if (area != NULL)
			{
				if (area->_mDragSpeed.x == 0.0f)
				{
					area->_mLastScrollOffset.x = area->getScrollOffsetX();
					area->_mDragTimer.x = 0.0f;
				}
				if (area->_mDragSpeed.y == 0.0f)
				{
					area->_mLastScrollOffset.y = area->getScrollOffsetY();
					area->_mDragTimer.y = 0.0f;
				}
			}
		}
	}

	void ScrollBar::OnDraw()
	{
		Object::OnDraw();
		if (mSkinName == "")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL)
				{
					if (area->isDragging() || area->isScrolling())
					{
						_mRetainTime = RETAIN_TIME;
					}
					if (_mRetainTime > 0.0f)
					{
						april::Color color = _getDrawColor();
						if (_mRetainTime < FADE_OUT_TIME)
						{
							color.a = (unsigned char)hclamp(color.a * _mRetainTime / FADE_OUT_TIME, 0.0f, 255.0f);
						}
						april::rendersys->drawColoredQuad(_getBarDrawRect(), color);
					}
				}
			}
		}
	}

	void ScrollBar::notifyEvent(chstr name, void* params)
	{
		Object::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent == NULL)
			{
				aprilui::log("Warning! ScrollBar '" + mName + "' not attached to object of class Container!");
			}
		}
		else if (name == "SkinChange")
		{
			if (mSkinName != "")
			{
				if (mButtonBegin == NULL)
				{
					mButtonBegin = new ImageButton(generateName("aprilui::ScrollSkinButtonBegin"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					registerChild(mButtonBegin);
					_SET_CLICK_EVENT_FUNCTION(mButtonBegin, _clickScrollBegin);
					mButtonEnd = new ImageButton(generateName("aprilui::ScrollSkinButtonEnd"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					registerChild(mButtonEnd);
					_SET_CLICK_EVENT_FUNCTION(mButtonEnd, _clickScrollEnd);
					mButtonBack = new ImageButton(generateName("aprilui::ScrollSkinButtonBack"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					registerChild(mButtonBack);
					_SET_CLICK_EVENT_FUNCTION(mButtonBack, _clickScrollBack);
					mButtonBar = new ImageButton(generateName("aprilui::ScrollSkinButtonBar"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					registerChild(mButtonBar);
					_SET_MOUSEDOWN_EVENT_FUNCTION(mButtonBar, _mouseDownScrollBar);
					_SET_CLICK_EVENT_FUNCTION(mButtonBar, _clickScrollBar);
				}
				mButtonBegin->setImageByName(mSkinName + "/" + _getSkinNameBeginNormal());
				mButtonBegin->setHoverImageByName(mSkinName + "/" + _getSkinNameBeginHover());
				mButtonBegin->setPushedImageByName(mSkinName + "/" + _getSkinNameBeginPushed());
				mButtonBegin->resizeToFitImage();
				mButtonEnd->setImageByName(mSkinName + "/" + _getSkinNameEndNormal());
				mButtonEnd->setHoverImageByName(mSkinName + "/" + _getSkinNameEndHover());
				mButtonEnd->setPushedImageByName(mSkinName + "/" + _getSkinNameEndPushed());
				mButtonEnd->resizeToFitImage();
				mButtonBack->setImageByName(mSkinName + "/" + _getSkinNameBackground());
				mButtonBack->setHoverImageByName(mSkinName + "/" + _getSkinNameBackground());
				mButtonBack->setPushedImageByName(mSkinName + "/" + _getSkinNameBackground());
				mButtonBack->resizeToFitImage();
				mButtonBar->setImageByName(mSkinName + "/" + _getSkinNameBarNormal());
				mButtonBar->setHoverImageByName(mSkinName + "/" + _getSkinNameBarHover());
				mButtonBar->setPushedImageByName(mSkinName + "/" + _getSkinNameBarPushed());
				mButtonBar->resizeToFitImage();
				_updateChildren();
				_updateBar();
			}
			else if (mButtonBegin != NULL)
			{
				unregisterChild(mButtonBegin);
				delete mButtonBegin;
				mButtonBegin = NULL;
				unregisterChild(mButtonEnd);
				delete mButtonEnd;
				mButtonEnd = NULL;
				unregisterChild(mButtonBack);
				delete mButtonBack;
				mButtonBack = NULL;
				unregisterChild(mButtonBar);
				delete mButtonBar;
				mButtonBar = NULL;
			}
		}
	}

	hstr ScrollBar::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "skin")			return getSkinName();
		if (name == "grid_size")	return getGridSize();
		return Object::getProperty(name, property_exists);
	}

	bool ScrollBar::setProperty(chstr name, chstr value)
	{
		if		(name == "skin")		setSkinName(value);
		else if	(name == "grid_size")	setGridSize(value);
		else return Object::setProperty(name, value);
		return true;
	}

	void ScrollBar::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
		if (mButtonBar != NULL && mButtonBar->isPushed())
		{
			gvec2 position = getCursorPosition() / getDerivedScale() - _mClickPosition;
			_moveScrollBar(position.x, position.y);
		}
	}

	void ScrollBar::onMouseScroll(float x, float y)
	{
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent != NULL && (parent->isCursorInside() || isCursorInside()))
		{
			addScrollValue(_calcScrollMove(x, y));
		}
		Object::onMouseScroll(x, y);
	}

	void ScrollBar::_clickScrollBegin(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		scrollBar->addScrollValue(-ScrollBar::ScrollDistance);
	}

	void ScrollBar::_clickScrollEnd(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		scrollBar->addScrollValue(ScrollBar::ScrollDistance);
	}

	void ScrollBar::_clickScrollBack(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		gvec2 position = (getCursorPosition() - scrollBar->getDerivedPosition()) / scrollBar->getDerivedScale();
		scrollBar->addScrollValue(scrollBar->_calcScrollJump(position.x, position.y));
	}

	void ScrollBar::_mouseDownScrollBar(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		scrollBar->_mClickPosition = getCursorPosition() / scrollBar->getDerivedScale() -
			scrollBar->mButtonBar->getPosition() + scrollBar->mButtonBegin->getSize();
	}

	void ScrollBar::_clickScrollBar(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		scrollBar->_initAreaDragging();
		scrollBar->_adjustDragSpeed();
	}

}
