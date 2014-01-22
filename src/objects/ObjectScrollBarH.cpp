/// @file
/// @author  Boris Mikic
/// @version 2.8
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
		grect result = this->_getDrawRect();
		if (this->mSkinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL)
				{
					float range = this->getWidth();
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

	void ScrollBarH::addScrollValue(float value, bool useAccumulated)
	{
		Container* parent = dynamic_cast<Container*>(this->mParent);
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
			this->_initAreaDragging();
			bool noAccumulation = (area->_mDragDistance.x == 0.0f);
			if (useAccumulated)
			{
				area->_mDragDistance.x += value;
				value = area->_mDragDistance.x;
			}
			value = hroundf(value);
			if (area->_mDragSpeed.x != 0.0f && area->_mDragTimer.x > 0.0f && noAccumulation)
			{
				float time = habs(area->_mDragSpeed.x / inertia);
				float distance = area->_mDragSpeed.x * area->_mDragTimer.x - hsgn(area->_mDragSpeed.x) * inertia * area->_mDragTimer.x * area->_mDragTimer.x * 0.5f;
				value -= hroundf(hsgn(area->_mDragSpeed.x) * inertia * time * time * 0.5f - distance);
				area->_mLastScrollOffset.x = area->getScrollOffsetX();
			}
			else if (this->mGridSize > 0.0f && habs(value) < this->mGridSize)
			{
				value = hsgn(value) * this->mGridSize;
			}
			area->_mDragSpeed.x = -hsgn(value) * hsqrt(2 * inertia * habs(value));
			this->_adjustDragSpeed();
		}
		this->_updateBar();
	}

	float ScrollBarH::_calcScrollJump(float x, float y)
	{
		if (this->mButtonBar == NULL)
		{
			return 0.0f;
		}
		Container* parent = dynamic_cast<Container*>(this->mParent);
		if (parent == NULL)
		{
			return 0.0f;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return 0.0f;
		}
		float result = hsgn(x + this->mButtonBegin->getX() - this->mButtonBar->getX()) * parent->getWidth();
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

	float ScrollBarH::_calcScrollMove(float x, float y)
	{
		return (x * ScrollBar::ScrollDistance);
	}

	void ScrollBarH::notifyEvent(chstr name, void* params)
	{
		ScrollBar::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
			if (parent != NULL)
			{
				parent->_setScrollBarH(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
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
		mButtonEnd->setX(this->getWidth() - this->mButtonEnd->getWidth());
		mButtonEnd->setAnchorTop(false);
		mButtonEnd->setAnchorBottom(false);
		mButtonEnd->setAnchorLeft(false);
		mButtonEnd->setAnchorRight(true);
		mButtonBack->setX(this->mButtonBegin->getWidth());
		mButtonBack->setSize(this->getWidth() - this->mButtonBegin->getWidth() - this->mButtonEnd->getWidth(), this->getHeight());
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
		if (this->mButtonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(this->mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		area->setScrollOffsetX(hroundf(x * parent->getWidth() / this->mButtonBar->getWidth()));
		this->_updateBar();
	}

	void ScrollBarH::_updateBar()
	{
		if (this->mButtonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(this->mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		float range = this->getWidth() - this->mButtonBegin->getWidth() - this->mButtonEnd->getWidth();
		float factor = area->getWidth();
		float ratio = (factor - parent->getWidth()) / factor;
		if (ratio > 0.0f)
		{
			this->mButtonBar->setWidth(hclamp((1 - ratio) * range, 8.0f, range));
			this->mButtonBar->setX(hroundf(this->mButtonBegin->getWidth() - area->getX() / factor * range));
		}
		else
		{
			this->mButtonBar->setWidth(range);
			this->mButtonBar->setX(this->mButtonBegin->getWidth());
		}
	}

	void ScrollBarH::_adjustDragSpeed()
	{
		if (this->mGridSize <= 0.0f)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(this->mParent);
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
			s = -hsgn(area->_mDragSpeed.x) * area->_mDragSpeed.x * area->_mDragSpeed.x * 0.5f / inertia;
		}
		float difference = area->_mLastScrollOffset.x - hroundf(area->_mLastScrollOffset.x / this->mGridSize) * this->mGridSize;
		float offset = hroundf(s / this->mGridSize) * this->mGridSize - s;
		if (parent->getWidth() > this->mGridSize)
		{
			s = hroundf(s + offset - difference);
		}
		// these are grid snapping cases when grid size exceeds parent size
		else if (habs(difference) == 0.0f) // using habs because it can be -0.0f!
		{
			s = hsgn(s) * this->mGridSize;
		}
		else if (habs(s) < habs(difference) || hsgn(s) != hsgn(difference))
		{
			s = -difference;
		}
		else
		{
			s = hsgn(s) * hmodf(-habs(difference), this->mGridSize);
		}
		// v = sqrt(2 * a * s)
		area->_mDragSpeed.x = -hsgn(s) * hsqrt(2 * inertia * habs(s));
	}

	bool ScrollBarH::_checkAreaSize()
	{
		Container* parent = dynamic_cast<Container*>(this->mParent);
		if (parent == NULL)
		{
			return true;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return true;
		}
		return (area->getWidth() > parent->getWidth());
	}

}
