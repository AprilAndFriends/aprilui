/// @file
/// @author  Boris Mikic
/// @version 3.0
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
		if (this->skinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
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
			area->setScrollOffsetY(area->getScrollOffsetY() + value);
		}
		else
		{
			this->_initAreaDragging();
			bool noAccumulation = (area->_dragDistance.y == 0.0f);
			if (useAccumulated)
			{
				area->_dragDistance.y += value;
				value = area->_dragDistance.y;
			}
			value = hroundf(value);
			if (area->_dragSpeed.y != 0.0f && area->_dragTimer.y > 0.0f && noAccumulation)
			{
				float time = habs(area->_dragSpeed.y / inertia);
				float distance = area->_dragSpeed.y * area->_dragTimer.y - hsgn(area->_dragSpeed.y) * inertia * area->_dragTimer.y * area->_dragTimer.y * 0.5f;
				value -= hroundf(hsgn(area->_dragSpeed.y) * inertia * time * time * 0.5f - distance);
				area->_lastScrollOffset.y = area->getScrollOffsetY();
			}
			else if (this->gridSize > 0.0f && habs(value) < this->gridSize)
			{
				value = hsgn(value) * this->gridSize;
			}
			area->_dragSpeed.y = -hsgn(value) * hsqrt(2 * inertia * habs(value));
			this->_adjustDragSpeed();
		}
		this->_updateBar();
	}

	float ScrollBarV::_calcScrollJump(float x, float y)
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
		float result = hsgn(y + this->buttonBegin->getY() - this->buttonBar->getY()) * parent->getHeight();
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
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->_setScrollBarV(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->_setScrollBarV(NULL);
			}
		}
	}

	void ScrollBarV::_updateChildren()
	{
		buttonBegin->setAnchorTop(true);
		buttonBegin->setAnchorBottom(false);
		buttonBegin->setAnchorLeft(false);
		buttonBegin->setAnchorRight(false);
		buttonEnd->setY(this->getHeight() - this->buttonEnd->getHeight());
		buttonEnd->setAnchorTop(false);
		buttonEnd->setAnchorBottom(true);
		buttonEnd->setAnchorLeft(false);
		buttonEnd->setAnchorRight(false);
		buttonBack->setY(this->buttonBegin->getHeight());
		buttonBack->setSize(this->getWidth(), this->getHeight() - this->buttonBegin->getHeight() - this->buttonEnd->getHeight());
		buttonBack->setAnchorTop(true);
		buttonBack->setAnchorBottom(true);
		buttonBack->setAnchorLeft(false);
		buttonBack->setAnchorRight(false);
		buttonBar->setAnchorTop(true);
		buttonBar->setAnchorBottom(false);
		buttonBar->setAnchorLeft(false);
		buttonBar->setAnchorRight(false);
	}

	void ScrollBarV::_moveScrollBar(float x, float y)
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
		area->setScrollOffsetY(hroundf(y * parent->getHeight() / this->buttonBar->getHeight()));
		this->_updateBar();
	}

	void ScrollBarV::_updateBar()
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
		float range = this->getHeight() - this->buttonBegin->getHeight() - this->buttonEnd->getHeight();
		float factor = area->getHeight();
		float ratio = (factor - parent->getHeight()) / factor;
		if (ratio > 0.0f)
		{
			this->buttonBar->setHeight(hclamp((1 - ratio) * range, 8.0f, range));
			this->buttonBar->setY(hroundf(this->buttonBegin->getHeight() - area->getY() / factor * range));
		}
		else
		{
			this->buttonBar->setHeight(range);
			this->buttonBar->setY(this->buttonBegin->getHeight());
		}
	}

	void ScrollBarV::_adjustDragSpeed()
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
		if (area->_dragSpeed.y != 0.0f)
		{
			// s0 = v0 ^ 2 / (2 * a)
			s = -hsgn(area->_dragSpeed.y) * area->_dragSpeed.y * area->_dragSpeed.y * 0.5f / inertia;
		}
		float difference = area->_lastScrollOffset.y - hroundf(area->_lastScrollOffset.y / this->gridSize) * this->gridSize;
		float offset = hroundf(s / this->gridSize) * this->gridSize - s;
		if (parent->getHeight() > this->gridSize)
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
		area->_dragSpeed.y = -hsgn(s) * hsqrt(2 * inertia * habs(s));
	}

	bool ScrollBarV::_checkAreaSize()
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
		return (area->getHeight() > parent->getHeight());
	}

}
