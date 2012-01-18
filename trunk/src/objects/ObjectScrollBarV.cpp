/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

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

	void ScrollBarV::_addScrollValue(float value)
	{
		if (mButtonBar != NULL)
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL)
				{
					area->setY(hclamp(area->getY() - value, parent->getHeight() - area->getHeight(), 0.0f));
					_updateBar();
				}
			}
		}
	}

	float ScrollBarV::_calcScrollJump(float x, float y)
	{
		if (mButtonBar != NULL)
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				return (y + mButtonBegin->getY() < mButtonBar->getY() ? -parent->getHeight() : parent->getHeight());
			}
		}
		return 0.0f;
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

	void ScrollBarV::_updateBar()
	{
		if (mButtonBar != NULL)
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL)
				{
					float range = getHeight() - mButtonBegin->getHeight() - mButtonEnd->getHeight();
					float factor = area->getWidth();
					float ratio = (factor - parent->getHeight()) / factor;
					float size = hclamp((1 - ratio) * range, 8.0f, range);
					if (ratio > 0)
					{
						mButtonBar->setHeight(size);
						mButtonBar->setY(mButtonBegin->getHeight() - area->getY() / (ratio * factor) * (ratio * range));
					}
					else
					{
						mButtonBar->setHeight(range);
						mButtonBar->setY(mButtonBegin->getHeight());
					}
				}
			}
		}
	}

}
