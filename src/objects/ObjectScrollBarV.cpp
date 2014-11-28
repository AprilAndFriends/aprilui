/// @file
/// @version 4.0
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
#include "ObjectScrollBarV.h"

namespace aprilui
{
	hstr ScrollBarV::SkinNameVBackground = "v_background";
	hstr ScrollBarV::SkinNameVSliderNormal = "v_slider_normal";
	hstr ScrollBarV::SkinNameVSliderHover = "v_slider_hover";
	hstr ScrollBarV::SkinNameVSliderPushed = "v_slider_pushed";
	hstr ScrollBarV::SkinNameVForwardNormal = "down_normal";
	hstr ScrollBarV::SkinNameVForwardHover = "down_hover";
	hstr ScrollBarV::SkinNameVForwardPushed = "down_pushed";
	hstr ScrollBarV::SkinNameVBackwardNormal = "up_normal";
	hstr ScrollBarV::SkinNameVBackwardHover = "up_hover";
	hstr ScrollBarV::SkinNameVBackwardPushed = "up_pushed";

	ScrollBarV::ScrollBarV(chstr name) : ScrollBar(name)
	{
	}

	ScrollBarV::ScrollBarV(const ScrollBarV& other) : ScrollBar(other)
	{
	}

	ScrollBarV::~ScrollBarV()
	{
	}

	Object* ScrollBarV::createInstance(chstr name)
	{
		return new ScrollBarV(name);
	}

	grect ScrollBarV::_getBarDrawRect()
	{
		grect result = this->_getDrawRect();
		if (this->skinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL && parent->scrollArea != NULL)
			{
				float range = this->getHeight();
				float factor = parent->scrollArea->getHeight();
				float ratio = (factor - parent->getHeight()) / factor;
				if (ratio > 0.0f)
				{
					result.y += (float)(int)(-parent->scrollArea->getY() / factor * range);
					result.h = hclamp((1 - ratio) * range, 8.0f, range);
				}
			}
		}
		return result;
	}

	void ScrollBarV::addScrollValue(float value)
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL || parent->scrollArea == NULL)
		{
			return;
		}
		float inertia = parent->scrollArea->getInertia();
		if (inertia <= 0.0f)
		{
			parent->scrollArea->setScrollOffsetY(parent->scrollArea->getScrollOffsetY() + value);
		}
		else
		{
			this->_initAreaDragging();
			if (parent->scrollArea->_dragSpeed.y != 0.0f)
			{
				float time = habs(parent->scrollArea->_dragSpeed.y / inertia);
				float distance = parent->scrollArea->_dragSpeed.y * parent->scrollArea->_dragTimer.y - hsgn(parent->scrollArea->_dragSpeed.y) *
					inertia * parent->scrollArea->_dragTimer.y * parent->scrollArea->_dragTimer.y * 0.5f;
				value -= hroundf(hsgn(parent->scrollArea->_dragSpeed.y) * inertia * time * time * 0.5f - distance);
				parent->scrollArea->_lastScrollOffset.y = parent->scrollArea->getScrollOffsetY();
				parent->scrollArea->_dragTimer.y = 0.0f;
			}
			else if (this->gridSize > 0.0f && habs(value) < this->gridSize)
			{
				value = hsgn(value) * this->gridSize;
			}
			parent->scrollArea->_dragSpeed.y = -hsgn(value) * hsqrt(2 * inertia * habs(value));
			this->_adjustDragSpeed();
		}
		this->_updateBar();
	}

	float ScrollBarV::_calcScrollJump(float x, float y, gvec2 size)
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
		float offset = parent->scrollArea->getScrollOffsetY();
		float result = 0.0f;
		if (y / size.y * parent->scrollArea->getHeight() < offset)
		{
			result = hmax(-parent->getHeight(), -offset);
		}
		else
		{
			result = hmin(parent->getHeight(), parent->scrollArea->getHeight() - parent->getHeight() - offset);
		}
		return result;
	}

	float ScrollBarV::_calcScrollMove(float x, float y)
	{
		return hroundf(y * ScrollBar::ScrollDistance);
	}

	void ScrollBarV::notifyEvent(chstr type, EventArgs* args)
	{
		ScrollBar::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->scrollBarV = this;
			}
		}
		else if (type == Event::DetachedFromObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->scrollBarV = NULL;
			}
		}
	}

	void ScrollBarV::_updateChildren()
	{
		if (this->_buttonBackgroundSkinned != NULL)
		{
			this->_buttonBackgroundSkinned->setY(this->_buttonBackwardSkinned->getHeight());
			this->_buttonBackgroundSkinned->setSize(this->getWidth(), this->getHeight() - this->_buttonBackwardSkinned->getHeight() - this->_buttonForwardSkinned->getHeight());
			this->_buttonBackgroundSkinned->setAnchors(false, false, true, true);
		}
		if (this->_buttonSliderSkinned != NULL)
		{
			this->_buttonSliderSkinned->setAnchors(false, false, true, false);
		}
		if (this->_buttonForwardSkinned != NULL)
		{
			this->_buttonForwardSkinned->setY(this->getHeight() - this->_buttonForwardSkinned->getHeight());
			this->_buttonForwardSkinned->setAnchors(false, false, false, true);
		}
		if (this->_buttonBackwardSkinned != NULL)
		{
			this->_buttonBackwardSkinned->setAnchors(false, false, true, false);
		}
	}

	void ScrollBarV::_moveScrollBar(float x, float y)
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
		parent->scrollArea->setScrollOffsetY(hroundf(y * (parent->scrollArea->getHeight() - parent->getHeight()) / (buttonBackground->getHeight() - buttonSlider->getHeight())));
		this->_updateBar();
	}

	void ScrollBarV::_updateBar()
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
		float offset = buttonBackground->getY();
		float size = buttonBackground->getHeight();
		float areaSize = parent->scrollArea->getHeight();
		float scrollSize = areaSize - parent->getHeight();
		float areaRatio = scrollSize / areaSize;
		float scrollOffsetRatio = parent->scrollArea->getScrollOffsetY() / scrollSize;
		if (this->_buttonSlider != NULL)
		{
			if (areaRatio > 0.0f)
			{
				this->_buttonSlider->setY(hroundf(offset + scrollOffsetRatio * (size - this->_buttonSlider->getHeight())));
			}
			else
			{
				this->_buttonSlider->setY(hroundf(offset + (size - this->_buttonSlider->getHeight()) * 0.5f));
			}
		}
		if (this->_buttonSliderSkinned != NULL)
		{
			if (areaRatio > 0.0f)
			{
				if (this->useStretchedSlider)
				{
					this->_buttonSliderSkinned->setHeight(hclamp((1 - areaRatio) * size, 8.0f, size));
				}
				else
				{
					this->_buttonSliderSkinned->resizeToFitImage();
				}
				this->_buttonSliderSkinned->setY(hroundf(offset + scrollOffsetRatio * (size - this->_buttonSliderSkinned->getHeight())));
			}
			else
			{
				if (this->useStretchedSlider)
				{
					this->_buttonSliderSkinned->setHeight(size);
				}
				else
				{
					this->_buttonSliderSkinned->resizeToFitImage();
				}
				this->_buttonSliderSkinned->setY(offset);
			}
		}
	}

	void ScrollBarV::_adjustDragSpeed()
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
		if (parent->scrollArea->_dragSpeed.y != 0.0f)
		{
			// s0 = v0 ^ 2 / (2 * a)
			s = -hsgn(parent->scrollArea->_dragSpeed.y) * parent->scrollArea->_dragSpeed.y * parent->scrollArea->_dragSpeed.y * 0.5f / inertia;
		}
		float difference = parent->scrollArea->_lastScrollOffset.y - hroundf(parent->scrollArea->_lastScrollOffset.y / this->gridSize) * this->gridSize;
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
		parent->scrollArea->_dragSpeed.y = -hsgn(s) * hsqrt(2 * inertia * habs(s));
	}

	bool ScrollBarV::_checkAreaSize()
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL || parent->scrollArea == NULL)
		{
			return true;
		}
		return (parent->scrollArea->getHeight() > parent->getHeight());
	}

}
