/// @file
/// @author  Boris Mikic
/// @version 2.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
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
		this->mGridSize = GridSize;
		this->mUseFading = true;
		this->mHeightHide = true;
		this->mButtonBegin = NULL;
		this->mButtonEnd = NULL;
		this->mButtonBack = NULL;
		this->mButtonBar = NULL;
		this->_mClickPosition.set(0.0f, 0.0f);
		this->_mRetainTime = 0.0f;
	}

	ScrollBar::~ScrollBar()
	{
	}

	void ScrollBar::setSkinName(chstr value)
	{
		this->mSkinName = value;
		this->notifyEvent("SkinChange", NULL);
	}

	void ScrollBar::update(float k)
	{
		Object::update(k);
		this->_updateBar();
		if (this->mSkinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL && this->_mRetainTime > 0.0f)
				{
					this->_mRetainTime -= k;
				}
			}
		}
	}

	void ScrollBar::_initAreaDragging()
	{
		Container* parent = dynamic_cast<Container*>(this->mParent);
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
		if (this->mSkinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL)
				{
					if (area->isDragging() || area->isScrolling())
					{
						this->_mRetainTime = RETAIN_TIME;
					}
					if ((!this->mUseFading || this->_mRetainTime > 0.0f) &&
						(!this->mHeightHide || this->_checkAreaSize()))
					{
						april::Color color = this->_getDrawColor();
						if (this->mUseFading && this->_mRetainTime < FADE_OUT_TIME)
						{
							color.a = (unsigned char)hclamp(color.a * this->_mRetainTime / FADE_OUT_TIME, 0.0f, 255.0f);
						}
						april::rendersys->drawFilledRect(this->_getBarDrawRect(), color);
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
			Container* parent = dynamic_cast<Container*>(this->mParent);
			if (parent == NULL)
			{
				hlog::warnf(aprilui::logTag, "ScrollBar '%s' not attached to object of class Container!", this->mName.c_str());
			}
		}
		else if (name == "SkinChange")
		{
			if (this->mSkinName != "")
			{
				if (this->mButtonBegin == NULL)
				{
					this->mButtonBegin = new ImageButton(aprilui::generateName("aprilui::ScrollSkinButtonBegin"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					this->registerChild(this->mButtonBegin);
					_SET_CLICK_EVENT_FUNCTION(this->mButtonBegin, _clickScrollBegin);
					this->mButtonEnd = new ImageButton(aprilui::generateName("aprilui::ScrollSkinButtonEnd"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					this->registerChild(this->mButtonEnd);
					_SET_CLICK_EVENT_FUNCTION(this->mButtonEnd, _clickScrollEnd);
					this->mButtonBack = new ImageButton(aprilui::generateName("aprilui::ScrollSkinButtonBack"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					this->registerChild(this->mButtonBack);
					_SET_CLICK_EVENT_FUNCTION(this->mButtonBack, _clickScrollBack);
					this->mButtonBar = new ImageButton(aprilui::generateName("aprilui::ScrollSkinButtonBar"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					this->registerChild(this->mButtonBar);
					_SET_MOUSEDOWN_EVENT_FUNCTION(this->mButtonBar, _mouseDownScrollBar);
					_SET_CLICK_EVENT_FUNCTION(this->mButtonBar, _clickScrollBar);
				}
				this->mButtonBegin->trySetImageByName(this->mSkinName + "/" + this->_getSkinNameBeginNormal());
				this->mButtonBegin->trySetHoverImageByName(this->mSkinName + "/" + this->_getSkinNameBeginHover());
				this->mButtonBegin->trySetPushedImageByName(this->mSkinName + "/" + this->_getSkinNameBeginPushed());
				this->mButtonBegin->resizeToFitImage();
				this->mButtonEnd->trySetImageByName(this->mSkinName + "/" + this->_getSkinNameEndNormal());
				this->mButtonEnd->trySetHoverImageByName(this->mSkinName + "/" + this->_getSkinNameEndHover());
				this->mButtonEnd->trySetPushedImageByName(this->mSkinName + "/" + this->_getSkinNameEndPushed());
				this->mButtonEnd->resizeToFitImage();
				this->mButtonBack->trySetImageByName(this->mSkinName + "/" + this->_getSkinNameBackground());
				this->mButtonBack->trySetHoverImageByName(this->mSkinName + "/" + this->_getSkinNameBackground());
				this->mButtonBack->trySetPushedImageByName(this->mSkinName + "/" + this->_getSkinNameBackground());
				this->mButtonBack->resizeToFitImage();
				this->mButtonBar->trySetImageByName(this->mSkinName + "/" + this->_getSkinNameBarNormal());
				this->mButtonBar->trySetHoverImageByName(this->mSkinName + "/" + this->_getSkinNameBarHover());
				this->mButtonBar->trySetPushedImageByName(this->mSkinName + "/" + this->_getSkinNameBarPushed());
				this->mButtonBar->resizeToFitImage();
				this->_updateChildren();
				this->_updateBar();
			}
			else if (mButtonBegin != NULL)
			{
				this->unregisterChild(this->mButtonBegin);
				delete this->mButtonBegin;
				this->mButtonBegin = NULL;
				this->unregisterChild(this->mButtonEnd);
				delete this->mButtonEnd;
				this->mButtonEnd = NULL;
				this->unregisterChild(this->mButtonBack);
				delete this->mButtonBack;
				this->mButtonBack = NULL;
				this->unregisterChild(this->mButtonBar);
				delete this->mButtonBar;
				this->mButtonBar = NULL;
			}
		}
	}

	hstr ScrollBar::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "skin")			return this->getSkinName();
		if (name == "grid_size")	return this->getGridSize();
		if (name == "use_fading")	return this->isUseFading();
		if (name == "height_hide")	return this->isHeightHide();
		return Object::getProperty(name, propertyExists);
	}

	bool ScrollBar::setProperty(chstr name, chstr value)
	{
		if		(name == "skin")		this->setSkinName(value);
		else if	(name == "grid_size")	this->setGridSize(value);
		else if	(name == "use_fading")	this->setUseFading(value);
		else if	(name == "height_hide")	this->setHeightHide(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ScrollBar::onMouseMove()
	{
		if (Object::onMouseMove())
		{
			return true;
		}
		if (this->mButtonBar != NULL && this->mButtonBar->isPushed())
		{
			gvec2 position = aprilui::getCursorPosition() / this->getDerivedScale() - this->_mClickPosition;
			this->_moveScrollBar(position.x, position.y);
		}
		return false;
	}

	bool ScrollBar::onMouseScroll(float x, float y)
	{
		if (Object::onMouseScroll(x, y))
		{
			return true;
		}
		Container* parent = dynamic_cast<Container*>(this->mParent);
		if (parent != NULL && (parent->isCursorInside() || this->isCursorInside()))
		{
			ScrollArea* area = parent->_getScrollArea();
			if (area != NULL && area->isSwapScrollWheels())
			{
				hswap(x, y);
			}
			this->addScrollValue(this->_calcScrollMove(x, y), true);
		}
		return false;
	}

	void ScrollBar::addScrollValueBegin(float multiplier)
	{
		this->addScrollValue(-hmax(habs(this->mGridSize), (float)(int)(habs(ScrollBar::ScrollDistance) * multiplier)));
	}

	void ScrollBar::addScrollValueEnd(float multiplier)
	{
		this->addScrollValue(hmax(habs(this->mGridSize), (float)(int)(habs(ScrollBar::ScrollDistance) * multiplier)));
	}

	void ScrollBar::_clickScrollBegin(EventArgs* args)
	{
		((ScrollBar*)args->object->getParent())->addScrollValueBegin();
	}

	void ScrollBar::_clickScrollEnd(EventArgs* args)
	{
		((ScrollBar*)args->object->getParent())->addScrollValueEnd();
	}

	void ScrollBar::_clickScrollBack(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		gvec2 position = (aprilui::getCursorPosition() - scrollBar->getDerivedPosition()) / scrollBar->getDerivedScale();
		scrollBar->addScrollValue(scrollBar->_calcScrollJump(position.x, position.y));
	}

	void ScrollBar::_mouseDownScrollBar(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		scrollBar->_mClickPosition = aprilui::getCursorPosition() / scrollBar->getDerivedScale() -
			scrollBar->mButtonBar->getPosition() + scrollBar->mButtonBegin->getSize();
	}

	void ScrollBar::_clickScrollBar(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		scrollBar->_initAreaDragging();
		scrollBar->_adjustDragSpeed();
	}

}
