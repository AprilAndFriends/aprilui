/// @file
/// @author  Boris Mikic
/// @version 3.06
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
	hstr ScrollBarH::SkinNameHBackground = "h_background";
	hstr ScrollBarH::SkinNameHSliderNormal = "h_slider_normal";
	hstr ScrollBarH::SkinNameHSliderHover = "h_slider_hover";
	hstr ScrollBarH::SkinNameHSliderPushed = "h_slider_pushed";
	hstr ScrollBarH::SkinNameHForwardNormal = "right_normal";
	hstr ScrollBarH::SkinNameHForwardHover = "right_hover";
	hstr ScrollBarH::SkinNameHForwardPushed = "right_pushed";
	hstr ScrollBarH::SkinNameHBackwardNormal = "left_normal";
	hstr ScrollBarH::SkinNameHBackwardHover = "left_hover";
	hstr ScrollBarH::SkinNameHBackwardPushed = "left_pushed";

	ScrollBarH::ScrollBarH(chstr name, grect rect) : ScrollBar(name, rect)
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
				area->_dragTimer.x = 0.0f;
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
		if (this->buttonSlider == NULL)
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
		float result = hsgn(x + this->buttonBackward->getX() - this->buttonSlider->getX()) * parent->getWidth();
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
		this->buttonBackward->setAnchorTop(false);
		this->buttonBackward->setAnchorBottom(false);
		this->buttonBackward->setAnchorLeft(true);
		this->buttonBackward->setAnchorRight(false);
		this->buttonForward->setX(this->getWidth() - this->buttonForward->getWidth());
		this->buttonForward->setAnchorTop(false);
		this->buttonForward->setAnchorBottom(false);
		this->buttonForward->setAnchorLeft(false);
		this->buttonForward->setAnchorRight(true);
		this->buttonBackground->setX(this->buttonBackward->getWidth());
		this->buttonBackground->setSize(this->getWidth() - this->buttonBackward->getWidth() - this->buttonForward->getWidth(), this->getHeight());
		this->buttonBackground->setAnchorTop(false);
		this->buttonBackground->setAnchorBottom(false);
		this->buttonBackground->setAnchorLeft(true);
		this->buttonBackground->setAnchorRight(true);
		this->buttonSlider->setAnchorTop(false);
		this->buttonSlider->setAnchorBottom(false);
		this->buttonSlider->setAnchorLeft(true);
		this->buttonSlider->setAnchorRight(false);
	}

	void ScrollBarH::_moveScrollBar(float x, float y)
	{
		if (this->buttonSlider == NULL)
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
		area->setScrollOffsetX(hroundf(x * parent->getWidth() / this->buttonSlider->getWidth()));
		this->_updateBar();
	}

	void ScrollBarH::_updateBar()
	{
		if (this->buttonSlider == NULL)
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
		float range = this->getWidth() - this->buttonBackward->getWidth() - this->buttonForward->getWidth();
		float factor = area->getWidth();
		float ratio = (factor - parent->getWidth()) / factor;
		if (ratio > 0.0f)
		{
			this->buttonSlider->setWidth(hclamp((1 - ratio) * range, 8.0f, range));
			this->buttonSlider->setX(hroundf(this->buttonBackward->getWidth() - area->getX() / factor * range));
		}
		else
		{
			this->buttonSlider->setWidth(range);
			this->buttonSlider->setX(this->buttonBackward->getWidth());
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
