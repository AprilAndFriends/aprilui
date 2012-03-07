/// @file
/// @author  Boris Mikic
/// @version 1.52
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "ObjectContainer.h"
#include "ObjectImageBox.h"
#include "ObjectImageButton.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBarH.h"

namespace aprilui
{
	hstr ScrollBarH::SkinNameLeftNormal = "left_normal";
	hstr ScrollBarH::SkinNameLeftHover = "left_hover";
	hstr ScrollBarH::SkinNameLeftPushed = "left_pushed";
	hstr ScrollBarH::SkinNameRightNormal = "right_normal";
	hstr ScrollBarH::SkinNameRightHover = "right_hover";
	hstr ScrollBarH::SkinNameRightPushed = "right_pushed";
	hstr ScrollBarH::SkinNameBackgroundH = "background_h";
	hstr ScrollBarH::SkinNameBarHNormal = "bar_h_normal";
	hstr ScrollBarH::SkinNameBarHHover = "bar_h_hover";
	hstr ScrollBarH::SkinNameBarHPushed = "bar_h_pushed";

	ScrollBarH::ScrollBarH(chstr name, grect rect) :
		ScrollBar(name, rect)
	{
	}

	ScrollBarH::~ScrollBarH()
	{
	}

	Object* ScrollBarH::createInstance(chstr name, grect rect)
	{
		return new ScrollBarH(name, rect);
	}

	grect ScrollBarH::_getBarDrawRect()
	{
		grect result = _getDrawRect();
		if (mSkinName == "")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL)
				{
					float range = getWidth();
					float factor = area->getWidth();
					float ratio = (factor - parent->getWidth()) / factor;
					if (ratio > 0.0f)
					{
						result.x += (float)(int)(-area->getX() / factor * range);
						result.w = hclamp((1 - ratio) * range, 8.0f, range);
					}
				}
			}
		}
		return result;
	}

	void ScrollBarH::addScrollValue(float value)
	{
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		float inertia = area->getInertia();
		if (inertia <= 0.0f)
		{
			area->setScrollOffsetX(area->getScrollOffsetX() + value);
		}
		else
		{
			_initAreaDragging();
			if (area->_mDragSpeed.x != 0.0f)
			{
				float time = habs(area->_mDragSpeed.x / inertia);
				float distance = area->_mDragSpeed.x * area->_mDragTimer.x - sgn(area->_mDragSpeed.x) * inertia * area->_mDragTimer.x * area->_mDragTimer.x * 0.5f;
				value -= hroundf(sgn(area->_mDragSpeed.x) * inertia * time * time * 0.5f - distance);
				area->_mLastScrollOffset.x = area->getScrollOffsetX();
				area->_mDragTimer.x = 0.0f;
			}
			area->_mDragSpeed.x = -sgn(value) * sqrt(2 * inertia * habs(value));
			_adjustDragSpeed();
		}
		_updateBar();
	}

	float ScrollBarH::_calcScrollJump(float x, float y)
	{
		if (mButtonBar == NULL)
		{
			return 0.0f;
		}
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return 0.0f;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return 0.0f;
		}
		float result = sgn(x + mButtonBegin->getX() - mButtonBar->getX()) * parent->getWidth();
		if (result < 0.0f)
		{
			result = hmax(result, -area->getScrollOffsetX());
		}
		else
		{
			result = hmin(result, area->getWidth() - parent->getWidth() - area->getScrollOffsetX());
		}
		return result;
	}

	void ScrollBarH::notifyEvent(chstr name, void* params)
	{
		ScrollBar::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollBarH(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollBarH(NULL);
			}
		}
	}

	void ScrollBarH::_updateChildren()
	{
		mButtonBegin->setAnchorTop(false);
		mButtonBegin->setAnchorBottom(false);
		mButtonBegin->setAnchorLeft(true);
		mButtonBegin->setAnchorRight(false);
		mButtonEnd->setX(getWidth() - mButtonEnd->getWidth());
		mButtonEnd->setAnchorTop(false);
		mButtonEnd->setAnchorBottom(false);
		mButtonEnd->setAnchorLeft(false);
		mButtonEnd->setAnchorRight(true);
		mButtonBack->setX(mButtonBegin->getWidth());
		mButtonBack->setSize(getWidth() - mButtonBegin->getWidth() - mButtonEnd->getWidth(), getHeight());
		mButtonBack->setAnchorTop(false);
		mButtonBack->setAnchorBottom(false);
		mButtonBack->setAnchorLeft(true);
		mButtonBack->setAnchorRight(true);
		mButtonBar->setAnchorTop(false);
		mButtonBar->setAnchorBottom(false);
		mButtonBar->setAnchorLeft(true);
		mButtonBar->setAnchorRight(false);
	}

	void ScrollBarH::_moveScrollBar(float x, float y)
	{
		if (mButtonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		area->setScrollOffsetX(hroundf(x * parent->getWidth() / mButtonBar->getWidth()));
		_updateBar();
	}

	void ScrollBarH::_updateBar()
	{
		if (mButtonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		float range = getWidth() - mButtonBegin->getWidth() - mButtonEnd->getWidth();
		float factor = area->getWidth();
		float ratio = (factor - parent->getWidth()) / factor;
		if (ratio > 0.0f)
		{
			mButtonBar->setWidth(hclamp((1 - ratio) * range, 8.0f, range));
			mButtonBar->setX(hroundf(mButtonBegin->getWidth() - area->getX() / factor * range));
		}
		else
		{
			mButtonBar->setWidth(range);
			mButtonBar->setX(mButtonBegin->getWidth());
		}
	}

	void ScrollBarH::_adjustDragSpeed()
	{
		if (mGridSize <= 0.0f)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		float inertia = area->getInertia();
		if (inertia <= 0.0f)
		{
			return;
		}
		float s = 0.0f;
		if (area->_mDragSpeed.x != 0.0f)
		{
			// s0 = v0 ^ 2 / (2 * a)
			s = -sgn(area->_mDragSpeed.x) * area->_mDragSpeed.x * area->_mDragSpeed.x * 0.5f / inertia;
		}
		float difference = area->_mLastScrollOffset.x - hroundf(area->_mLastScrollOffset.x / mGridSize) * mGridSize;
		float offset = hroundf(s / mGridSize) * mGridSize - s;
		if (parent->getWidth() > mGridSize)
		{
			s = hroundf(s + offset - difference);
		}
		// these are grid snapping cases when grid size exceeds parent size
		else if (habs(difference) == 0.0f) // using habs because it can be -0.0f!
		{
			s = sgn(s) * mGridSize;
		}
		else if (habs(s) < habs(difference) || sgn(s) != sgn(difference))
		{
			s = -difference;
		}
		else
		{
			s = sgn(s) * hmodf(-habs(difference), mGridSize);
		}
		// v = sqrt(2 * a * s)
		area->_mDragSpeed.x = -sgn(s) * sqrt(2 * inertia * habs(s));
	}

}
