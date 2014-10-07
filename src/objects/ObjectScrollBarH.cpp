/// @file
/// @version 3.4
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
#include "ObjectScrollBarButtonBackground.h"
#include "ObjectScrollBarButtonBackward.h"
#include "ObjectScrollBarButtonForward.h"
#include "ObjectScrollBarButtonSlider.h"
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

	float ScrollBarH::_calcScrollJump(float x, float y, gvec2 size)
	{
		if (this->_getButtonSlider() == NULL)
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
		float offset = area->getScrollOffsetX();
		float result = 0.0f;
		if (x / size.x * area->getWidth() < offset)
		{
			result = hmax(-parent->getWidth(), -offset);
		}
		else
		{
			result = hmin(parent->getWidth(), area->getWidth() - parent->getWidth() - offset);
		}
		return result;
	}

	float ScrollBarH::_calcScrollMove(float x, float y)
	{
		return hroundf(x * ScrollBar::ScrollDistance);
	}

	void ScrollBarH::notifyEvent(chstr type, EventArgs* args)
	{
		ScrollBar::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->_setScrollBarH(this);
			}
		}
		else if (type == Event::DetachedFromObject)
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
		if (this->_buttonBackgroundSkinned != NULL)
		{
			this->_buttonBackgroundSkinned->setX(this->_buttonBackwardSkinned->getWidth());
			this->_buttonBackgroundSkinned->setSize(this->getWidth() - this->_buttonBackwardSkinned->getWidth() - this->_buttonForwardSkinned->getWidth(), this->getHeight());
			this->_buttonBackgroundSkinned->setAnchors(true, true, false, false);
		}
		if (this->_buttonSliderSkinned != NULL)
		{
			this->_buttonSliderSkinned->setAnchors(true, false, false, false);
		}
		if (this->_buttonForwardSkinned != NULL)
		{
			this->_buttonForwardSkinned->setX(this->getWidth() - this->_buttonForwardSkinned->getWidth());
			this->_buttonForwardSkinned->setAnchors(false, true, false, false);
		}
		if (this->_buttonBackwardSkinned != NULL)
		{
			this->_buttonBackwardSkinned->setAnchors(true, false, false, false);
		}
	}

	void ScrollBarH::_moveScrollBar(float x, float y)
	{
		ScrollBarButtonSlider* buttonSlider = this->_getButtonSlider();
		if (buttonSlider == NULL)
		{
			return;
		}
		ScrollBarButtonBackground* buttonBackground = this->_getButtonBackground();
		if (buttonBackground == NULL)
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
		area->setScrollOffsetX(hroundf(x * (area->getWidth() - parent->getWidth()) / (buttonBackground->getWidth() - buttonSlider->getWidth())));
		this->_updateBar();
	}

	void ScrollBarH::_updateBar()
	{
		if (this->_buttonSlider == NULL && this->_buttonSliderSkinned == NULL)
		{
			return;
		}
		ScrollBarButtonBackground* buttonBackground = this->_getButtonBackground();
		if (buttonBackground == NULL)
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
		float offset = buttonBackground->getX();
		float size = buttonBackground->getWidth();
		float areaSize = area->getWidth();
		float scrollSize = areaSize - parent->getWidth();
		float areaRatio = scrollSize / areaSize;
		float scrollOffsetRatio = area->getScrollOffsetX() / scrollSize;
		if (this->_buttonSlider != NULL)
		{
			if (areaRatio > 0.0f)
			{
				this->_buttonSlider->setX(hroundf(offset + scrollOffsetRatio * (size - this->_buttonSlider->getWidth())));
			}
			else
			{
				this->_buttonSlider->setX(hroundf(offset + (size - this->_buttonSlider->getWidth()) * 0.5f));
			}
		}
		if (this->_buttonSliderSkinned != NULL)
		{
			if (areaRatio > 0.0f)
			{
				if (this->useStretchedSlider)
				{
					this->_buttonSliderSkinned->setWidth(hclamp((1 - areaRatio) * size, 8.0f, size));
				}
				else
				{
					this->_buttonSliderSkinned->resizeToFitImage();
				}
				this->_buttonSliderSkinned->setX(hroundf(offset + scrollOffsetRatio * (size - this->_buttonSliderSkinned->getWidth())));
			}
			else
			{
				if (this->useStretchedSlider)
				{
					this->_buttonSliderSkinned->setWidth(size);
				}
				else
				{
					this->_buttonSliderSkinned->resizeToFitImage();
				}
				this->_buttonSliderSkinned->setX(offset);
			}
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
