/// @file
/// @author  Boris Mikic
/// @version 3.0
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
		if (this->skinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
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

	void ScrollBarH::addScrollValue(float value)
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
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
			if (area->_dragSpeed.x != 0.0f)
			{
				float time = habs(area->_dragSpeed.x / inertia);
				float distance = area->_dragSpeed.x * area->_dragTimer.x - hsgn(area->_dragSpeed.x) * inertia * area->_dragTimer.x * area->_dragTimer.x * 0.5f;
				value -= hroundf(hsgn(area->_dragSpeed.x) * inertia * time * time * 0.5f - distance);
				area->_lastScrollOffset.x = area->getScrollOffsetX();
			}
			else if (this->gridSize > 0.0f && habs(value) < this->gridSize)
			{
				value = hsgn(value) * this->gridSize;
			}
			area->_dragSpeed.x = -hsgn(value) * hsqrt(2 * inertia * habs(value));
			this->_adjustDragSpeed();
		}
		this->_updateBar();
	}

	float ScrollBarH::_calcScrollJump(float x, float y)
	{
		if (this->buttonBar == NULL)
		{
			return 0.0f;
		}
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL)
		{
			return 0.0f;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return 0.0f;
		}
		float result = hsgn(x + this->buttonBegin->getX() - this->buttonBar->getX()) * parent->getWidth();
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
		return hroundf(x * ScrollBar::ScrollDistance);
	}

	void ScrollBarH::notifyEvent(chstr name, void* params)
	{
		ScrollBar::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->_setScrollBarH(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->_setScrollBarH(NULL);
			}
		}
	}

	void ScrollBarH::_updateChildren()
	{
		buttonBegin->setAnchorTop(false);
		buttonBegin->setAnchorBottom(false);
		buttonBegin->setAnchorLeft(true);
		buttonBegin->setAnchorRight(false);
		buttonEnd->setX(this->getWidth() - this->buttonEnd->getWidth());
		buttonEnd->setAnchorTop(false);
		buttonEnd->setAnchorBottom(false);
		buttonEnd->setAnchorLeft(false);
		buttonEnd->setAnchorRight(true);
		buttonBack->setX(this->buttonBegin->getWidth());
		buttonBack->setSize(this->getWidth() - this->buttonBegin->getWidth() - this->buttonEnd->getWidth(), this->getHeight());
		buttonBack->setAnchorTop(false);
		buttonBack->setAnchorBottom(false);
		buttonBack->setAnchorLeft(true);
		buttonBack->setAnchorRight(true);
		buttonBar->setAnchorTop(false);
		buttonBar->setAnchorBottom(false);
		buttonBar->setAnchorLeft(true);
		buttonBar->setAnchorRight(false);
	}

	void ScrollBarH::_moveScrollBar(float x, float y)
	{
		if (this->buttonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		area->setScrollOffsetX(hroundf(x * parent->getWidth() / this->buttonBar->getWidth()));
		this->_updateBar();
	}

	void ScrollBarH::_updateBar()
	{
		if (this->buttonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		float range = this->getWidth() - this->buttonBegin->getWidth() - this->buttonEnd->getWidth();
		float factor = area->getWidth();
		float ratio = (factor - parent->getWidth()) / factor;
		if (ratio > 0.0f)
		{
			this->buttonBar->setWidth(hclamp((1 - ratio) * range, 8.0f, range));
			this->buttonBar->setX(hroundf(this->buttonBegin->getWidth() - area->getX() / factor * range));
		}
		else
		{
			this->buttonBar->setWidth(range);
			this->buttonBar->setX(this->buttonBegin->getWidth());
		}
	}

	void ScrollBarH::_adjustDragSpeed()
	{
		if (this->gridSize <= 0.0f)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(this->parent);
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
		if (area->_dragSpeed.x != 0.0f)
		{
			// s0 = v0 ^ 2 / (2 * a)
			s = -hsgn(area->_dragSpeed.x) * area->_dragSpeed.x * area->_dragSpeed.x * 0.5f / inertia;
		}
		float difference = area->_lastScrollOffset.x - hroundf(area->_lastScrollOffset.x / this->gridSize) * this->gridSize;
		float offset = hroundf(s / this->gridSize) * this->gridSize - s;
		if (parent->getWidth() > this->gridSize)
		{
			s = hroundf(s + offset - difference);
		}
		// these are grid snapping cases when grid size exceeds parent size
		else if (habs(difference) == 0.0f) // using habs because it can be -0.0f!
		{
			s = hsgn(s) * this->gridSize;
		}
		else if (habs(s) < habs(difference) || hsgn(s) != hsgn(difference))
		{
			s = -difference;
		}
		else
		{
			s = hsgn(s) * hmodf(-habs(difference), this->gridSize);
		}
		// v = sqrt(2 * a * s)
		area->_dragSpeed.x = -hsgn(s) * hsqrt(2 * inertia * habs(s));
	}

	bool ScrollBarH::_checkAreaSize()
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
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
