/// @file
/// @author  Boris Mikic
/// @version 2.8
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "ObjectContainer.h"
#include "ObjectImageBox.h"
#include "ObjectImageButton.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBarV.h"

namespace aprilui
{
	hstr ScrollBarV::SkinNameUpNormal = "up_normal";
	hstr ScrollBarV::SkinNameUpHover = "up_hover";
	hstr ScrollBarV::SkinNameUpPushed = "up_pushed";
	hstr ScrollBarV::SkinNameDownNormal = "down_normal";
	hstr ScrollBarV::SkinNameDownHover = "down_hover";
	hstr ScrollBarV::SkinNameDownPushed = "down_pushed";
	hstr ScrollBarV::SkinNameBackgroundV = "background_v";
	hstr ScrollBarV::SkinNameBarVNormal = "bar_v_normal";
	hstr ScrollBarV::SkinNameBarVHover = "bar_v_hover";
	hstr ScrollBarV::SkinNameBarVPushed = "bar_v_pushed";

	ScrollBarV::ScrollBarV(chstr name, grect rect) :
		ScrollBar(name, rect)
	{
	}

	ScrollBarV::~ScrollBarV()
	{
	}

	Object* ScrollBarV::createInstance(chstr name, grect rect)
	{
		return new ScrollBarV(name, rect);
	}

	grect ScrollBarV::_getBarDrawRect()
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
					float range = this->getHeight();
					float factor = area->getHeight();
					float ratio = (factor - parent->getHeight()) / factor;
					if (ratio > 0.0f)
					{
						result.y += (float)(int)(-area->getY() / factor * range);
						result.h = hclamp((1 - ratio) * range, 8.0f, range);
					}
				}
			}
		}
		return result;
	}

	void ScrollBarV::addScrollValue(float value, bool useAccumulated)
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
			area->setScrollOffsetY(area->getScrollOffsetY() + value);
		}
		else
		{
			this->_initAreaDragging();
			bool noAccumulation = (area->_mDragDistance.y == 0.0f);
			if (useAccumulated)
			{
				area->_mDragDistance.y += value;
				value = area->_mDragDistance.y;
			}
			value = hroundf(value);
			if (area->_mDragSpeed.y != 0.0f && area->_mDragTimer.y > 0.0f && noAccumulation)
			{
				float time = habs(area->_mDragSpeed.y / inertia);
				float distance = area->_mDragSpeed.y * area->_mDragTimer.y - hsgn(area->_mDragSpeed.y) * inertia * area->_mDragTimer.y * area->_mDragTimer.y * 0.5f;
				value -= hroundf(hsgn(area->_mDragSpeed.y) * inertia * time * time * 0.5f - distance);
				area->_mLastScrollOffset.y = area->getScrollOffsetY();
			}
			else if (this->mGridSize > 0.0f && habs(value) < this->mGridSize)
			{
				value = hsgn(value) * this->mGridSize;
			}
			area->_mDragSpeed.y = -hsgn(value) * hsqrt(2 * inertia * habs(value));
			this->_adjustDragSpeed();
		}
		this->_updateBar();
	}

	float ScrollBarV::_calcScrollJump(float x, float y)
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
		float result = hsgn(y + this->mButtonBegin->getY() - this->mButtonBar->getY()) * parent->getHeight();
		if (result < 0.0f)
		{
			result = hmax(result, -area->getScrollOffsetY());
		}
		else
		{
			result = hmin(result, area->getHeight() - parent->getHeight() - area->getScrollOffsetY());
		}
		return result;
	}

	float ScrollBarV::_calcScrollMove(float x, float y)
	{
		return (y * ScrollBar::ScrollDistance);
	}

	void ScrollBarV::notifyEvent(chstr name, void* params)
	{
		ScrollBar::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
			if (parent != NULL)
			{
				parent->_setScrollBarV(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(this->mParent);
			if (parent != NULL)
			{
				parent->_setScrollBarV(NULL);
			}
		}
	}

	void ScrollBarV::_updateChildren()
	{
		mButtonBegin->setAnchorTop(true);
		mButtonBegin->setAnchorBottom(false);
		mButtonBegin->setAnchorLeft(false);
		mButtonBegin->setAnchorRight(false);
		mButtonEnd->setY(this->getHeight() - this->mButtonEnd->getHeight());
		mButtonEnd->setAnchorTop(false);
		mButtonEnd->setAnchorBottom(true);
		mButtonEnd->setAnchorLeft(false);
		mButtonEnd->setAnchorRight(false);
		mButtonBack->setY(this->mButtonBegin->getHeight());
		mButtonBack->setSize(this->getWidth(), this->getHeight() - this->mButtonBegin->getHeight() - this->mButtonEnd->getHeight());
		mButtonBack->setAnchorTop(true);
		mButtonBack->setAnchorBottom(true);
		mButtonBack->setAnchorLeft(false);
		mButtonBack->setAnchorRight(false);
		mButtonBar->setAnchorTop(true);
		mButtonBar->setAnchorBottom(false);
		mButtonBar->setAnchorLeft(false);
		mButtonBar->setAnchorRight(false);
	}

	void ScrollBarV::_moveScrollBar(float x, float y)
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
		area->setScrollOffsetY(hroundf(y * parent->getHeight() / this->mButtonBar->getHeight()));
		this->_updateBar();
	}

	void ScrollBarV::_updateBar()
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
		float range = this->getHeight() - this->mButtonBegin->getHeight() - this->mButtonEnd->getHeight();
		float factor = area->getHeight();
		float ratio = (factor - parent->getHeight()) / factor;
		if (ratio > 0.0f)
		{
			this->mButtonBar->setHeight(hclamp((1 - ratio) * range, 8.0f, range));
			this->mButtonBar->setY(hroundf(this->mButtonBegin->getHeight() - area->getY() / factor * range));
		}
		else
		{
			this->mButtonBar->setHeight(range);
			this->mButtonBar->setY(this->mButtonBegin->getHeight());
		}
	}

	void ScrollBarV::_adjustDragSpeed()
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
		if (area->_mDragSpeed.y != 0.0f)
		{
			// s0 = v0 ^ 2 / (2 * a)
			s = -hsgn(area->_mDragSpeed.y) * area->_mDragSpeed.y * area->_mDragSpeed.y * 0.5f / inertia;
		}
		float difference = area->_mLastScrollOffset.y - hroundf(area->_mLastScrollOffset.y / this->mGridSize) * this->mGridSize;
		float offset = hroundf(s / this->mGridSize) * this->mGridSize - s;
		if (parent->getHeight() > this->mGridSize)
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
		area->_mDragSpeed.y = -hsgn(s) * hsqrt(2 * inertia * habs(s));
	}

	bool ScrollBarV::_checkAreaSize()
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
		return (area->getHeight() > parent->getHeight());
	}

}
