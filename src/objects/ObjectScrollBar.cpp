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
#include "EventArgs.h"
#include "EventUtils.h"
#include "ObjectContainer.h"
#include "ObjectImageButton.h"
#include "ObjectScrollBar.h"
#include "Util.h"

namespace aprilui
{
	float ScrollBar::ScrollDistance = 32.0f;

	ScrollBar::ScrollBar(chstr name, grect rect) :
		Object(name, rect)
	{
		mScrollMode = Invisible;
		mButtonBegin = NULL;
		mButtonEnd = NULL;
		mButtonBack = NULL;
		mButtonBar = NULL;
		_mClickPosition.set(0.0f, 0.0f);
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
		if (name == "skin")	return getSkinName();
		if (name == "scroll_mode")
		{
			if (mScrollMode == Invisible)	return "invisible";
			if (mScrollMode == ActiveOnly)	return "active_only";
			if (mScrollMode == Always)		return "always";
		}
		return Object::getProperty(name, property_exists);
	}

	bool ScrollBar::setProperty(chstr name, chstr value)
	{
		if (name == "skin")	setSkinName(value);
		else if (name == "scroll_mode")
		{
			if (value == "invisible")	setScrollMode(Invisible);
			if (value == "active_only")	setScrollMode(ActiveOnly);
			if (value == "always")		setScrollMode(Always);
		}
		else return Object::setProperty(name, value);
		return true;
	}

	void ScrollBar::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
		if (mButtonBar != NULL && mButtonBar->isPushed())
		{
			_moveScrollBar(x - _mClickPosition.x, y - _mClickPosition.y);
		}
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
		scrollBar->addScrollValue(scrollBar->_calcScrollJump(args->x, args->y));
	}

	void ScrollBar::_mouseDownScrollBar(EventArgs* args)
	{
		ScrollBar* scrollBar = (ScrollBar*)args->object->getParent();
		scrollBar->_mClickPosition = gvec2(args->x, args->y) - scrollBar->mButtonBar->getPosition() + scrollBar->mButtonBegin->getSize();
	}

}
