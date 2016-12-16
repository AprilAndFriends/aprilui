/// @file
/// @version 4.1
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
	hstr ScrollBarH::skinNameHBackground = "h_background";
	hstr ScrollBarH::skinNameHSliderNormal = "h_slider_normal";
	hstr ScrollBarH::skinNameHSliderHover = "h_slider_hover";
	hstr ScrollBarH::skinNameHSliderPushed = "h_slider_pushed";
	hstr ScrollBarH::skinNameHForwardNormal = "right_normal";
	hstr ScrollBarH::skinNameHForwardHover = "right_hover";
	hstr ScrollBarH::skinNameHForwardPushed = "right_pushed";
	hstr ScrollBarH::skinNameHBackwardNormal = "left_normal";
	hstr ScrollBarH::skinNameHBackwardHover = "left_hover";
	hstr ScrollBarH::skinNameHBackwardPushed = "left_pushed";

	ScrollBarH::ScrollBarH(chstr name) : ScrollBar(name)
	{
	}

	ScrollBarH::ScrollBarH(const ScrollBarH& other) : ScrollBar(other)
	{
	}

	ScrollBarH::~ScrollBarH()
	{
	}

	Object* ScrollBarH::createInstance(chstr name)
	{
		return new ScrollBarH(name);
	}

	grect ScrollBarH::_getBarDrawRect() const
	{
		grect result = this->_makeDrawRect();
		if (this->skinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL && parent->scrollArea != NULL)
			{
				float range = this->getWidth();
				float factor = parent->scrollArea->getWidth();
				float ratio = (factor - parent->getWidth()) / factor;
				if (ratio > 0.0f)
				{
					result.x += (float)(int)(-parent->scrollArea->getX() / factor * range);
					result.w = hclamp((1 - ratio) * range, 8.0f, range);
				}
			}
		}
		return result;
	}

	void ScrollBarH::addScrollValue(float value)
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL || parent->scrollArea == NULL)
		{
			return;
		}
		float inertia = parent->scrollArea->getInertia();
		if (inertia <= 0.0f)
		{
			parent->scrollArea->setScrollOffsetX(parent->scrollArea->getScrollOffsetX() + value);
		}
		else
		{
			this->_initAreaDragging();
			if (parent->scrollArea->_dragSpeed.x != 0.0f)
			{
				float time = habs(parent->scrollArea->_dragSpeed.x / inertia);
				float distance = parent->scrollArea->_dragSpeed.x * parent->scrollArea->_dragTimer.x - hsgn(parent->scrollArea->_dragSpeed.x) *
					inertia * parent->scrollArea->_dragTimer.x * parent->scrollArea->_dragTimer.x * 0.5f;
				value -= hroundf(hsgn(parent->scrollArea->_dragSpeed.x) * inertia * time * time * 0.5f - distance);
				parent->scrollArea->_lastScrollOffset.x = parent->scrollArea->getScrollOffsetX();
				parent->scrollArea->_dragTimer.x = 0.0f;
			}
			else if (this->gridSize > 0.0f && habs(value) < this->gridSize)
			{
				value = hsgn(value) * this->gridSize;
			}
			parent->scrollArea->_dragSpeed.x = -hsgn(value) * hsqrt(2 * inertia * habs(value));
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
		if (parent == NULL || parent->scrollArea == NULL)
		{
			return 0.0f;
		}
		float offset = parent->scrollArea->getScrollOffsetX();
		float result = 0.0f;
		if (x / size.x * parent->scrollArea->getWidth() < offset)
		{
			result = hmax(-parent->getWidth(), -offset);
		}
		else
		{
			result = hmin(parent->getWidth(), parent->scrollArea->getWidth() - parent->getWidth() - offset);
		}
		return result;
	}

	float ScrollBarH::_calcScrollMove(float x, float y)
	{
		return hroundf(x * this->scrollDistance);
	}

	void ScrollBarH::notifyEvent(chstr type, EventArgs* args)
	{
		ScrollBar::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->scrollBarH =this;
			}
		}
		else if (type == Event::DetachedFromObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->scrollBarH = NULL;
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
		if (parent == NULL || parent->scrollArea == NULL)
		{
			return;
		}
		parent->scrollArea->setScrollOffsetX(hroundf(x * (parent->scrollArea->getWidth() - parent->getWidth()) / (buttonBackground->getWidth() - buttonSlider->getWidth())));
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
		if (parent == NULL || parent->scrollArea == NULL)
		{
			return;
		}
		float offset = buttonBackground->getX();
		float size = buttonBackground->getWidth();
		float areaSize = parent->scrollArea->getWidth();
		float scrollSize = areaSize - parent->getWidth();
		float areaRatio = scrollSize / areaSize;
		float scrollOffsetRatio = parent->scrollArea->getScrollOffsetX() / scrollSize;
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
		if (parent == NULL || parent->scrollArea == NULL)
		{
			return;
		}
		float inertia = parent->scrollArea->getInertia();
		if (inertia <= 0.0f)
		{
			return;
		}
		float s = 0.0f;
		if (parent->scrollArea->_dragSpeed.x != 0.0f)
		{
			// s0 = v0 ^ 2 / (2 * a)
			s = -hsgn(parent->scrollArea->_dragSpeed.x) * parent->scrollArea->_dragSpeed.x * parent->scrollArea->_dragSpeed.x * 0.5f / inertia;
		}
		// limiting scrolling movement
		if (this->maxGridScroll > 0)
		{
			s = hsgn(s) * hmin(habs(s), this->maxGridScroll * this->gridSize);
		}
		// calculating the final scroll movement distance
		float difference = parent->scrollArea->_lastScrollOffset.x - hroundf(parent->scrollArea->_lastScrollOffset.x / this->gridSize) * this->gridSize;
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
		parent->scrollArea->_dragSpeed.x = -hsgn(s) * hsqrt(2 * inertia * habs(s));
	}

	bool ScrollBarH::_checkAreaSize()
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL || parent->scrollArea == NULL)
		{
			return true;
		}
		return (parent->scrollArea->getWidth() > parent->getWidth());
	}

}
