/// @file
/// @author  Boris Mikic
/// @version 1.5
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
		grect result = _getDrawRect();
		if (mSkinName == "")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL)
				{
					float range = getHeight();
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

	void ScrollBarV::addScrollValue(float value)
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
			area->setScrollOffsetY(area->getScrollOffsetY() + value);
		}
		else
		{
			if (!area->isScrolling())
			{
				area->_mDragTimer = 0.0f;
			}
			if (area->_mDragSpeed.x == 0.0f)
			{
				area->_mLastScrollOffset.x = area->getScrollOffsetX();
			}
			if (area->_mDragSpeed.y == 0.0f)
			{
				area->_mLastScrollOffset.y = area->getScrollOffsetY();
			}
			else
			{
				// s0 = v0 ^ 2 / (2 * a)
				value -= sgn(area->_mDragSpeed.y) * area->_mDragSpeed.y * area->_mDragSpeed.y * 0.5f / inertia;
			}
			// v = sqrt(2 * a * s)
			area->_mDragSpeed.y = -sgn(value) * sqrt(2 * inertia * fabs(value));
		}
		_updateBar();
	}

	float ScrollBarV::_calcScrollJump(float x, float y)
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
		return (y + mButtonBegin->getY() < mButtonBar->getY() ? -parent->getHeight() : parent->getHeight());
	}

	void ScrollBarV::notifyEvent(chstr name, void* params)
	{
		ScrollBar::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollBarV(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
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
		mButtonEnd->setY(getHeight() - mButtonEnd->getHeight());
		mButtonEnd->setAnchorTop(false);
		mButtonEnd->setAnchorBottom(true);
		mButtonEnd->setAnchorLeft(false);
		mButtonEnd->setAnchorRight(false);
		mButtonBack->setY(mButtonBegin->getHeight());
		mButtonBack->setSize(getWidth(), getHeight() - mButtonBegin->getHeight() - mButtonEnd->getHeight());
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
		area->setScrollOffsetY(hroundf(y * parent->getHeight() / mButtonBar->getHeight()));
		_updateBar();
	}

	void ScrollBarV::_updateBar()
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
		float range = getHeight() - mButtonBegin->getHeight() - mButtonEnd->getHeight();
		float factor = area->getHeight();
		float ratio = (factor - parent->getHeight()) / factor;
		if (ratio > 0.0f)
		{
			mButtonBar->setHeight(hclamp((1 - ratio) * range, 8.0f, range));
			mButtonBar->setY(hroundf(mButtonBegin->getHeight() - area->getY() / factor * range));
		}
		else
		{
			mButtonBar->setHeight(range);
			mButtonBar->setY(mButtonBegin->getHeight());
		}
	}

	void ScrollBarV::_adjustDragSpeed()
	{
		return;
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

		float s = 0.0f;
		float inertia = area->getInertia();
		if (area->_mDragSpeed.y != 0.0f && inertia > 0.0f)
		{
			// s0 = v0 ^ 2 / (2 * a)
			s = sgn(area->_mDragSpeed.y) * area->_mDragSpeed.y * area->_mDragSpeed.y * 0.5f / inertia;
		}
		float s0 = s;
		float oy = area->getScrollOffsetY();
		float difference = sgn(oy) * (hroundf(oy / mGridSize) * mGridSize - oy);
		float offset = (s - hroundf(s / mGridSize) * mGridSize);
		s = s - offset - difference;
		//s = ;
		//s = target - oy;
		//s += ;
		//s = hroundf(s / mGridSize) * mGridSize;
		// v = sqrt(2 * a * s)
		area->_mDragSpeed.y = sgn(s) * sqrt(2 * inertia * fabs(s));

		//aprilui::log(hsprintf("%5.2f %5.2f - %5.2f - %5.2f %5.2f   %6.2f", s0, s, offset, oy, difference, area->_mDragSpeed.y));
		//return value;


		/*
			float gridSize = scrollBarV->getGridSize();
			if (gridSize > 0.0f)
			{

			}
		float length = _mDragSpeed.length();
		float newLength = length - k * mInertia;
		if (fabs(newLength) < fabs(length) && sgn(newLength) == sgn(length))
		{
			gvec2 oldSpeed = _mDragSpeed;
			_mDragSpeed *= newLength / length;
			gvec2 averageSpeed = (oldSpeed + _mDragSpeed) * 0.5f;
			gvec2 offset = getScrollOffset();
			setScrollOffset(offset - averageSpeed * k);
			if (getScrollOffset() == offset)
			{
				_mDragSpeed.set(0.0f, 0.0f);
			}
		}
		else
		{
			_mDragSpeed.set(0.0f, 0.0f);
		}
		*/
		//return value;
	}

}
