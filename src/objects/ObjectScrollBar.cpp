/// @file
/// @author  Boris Mikic
/// @version 3.14
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "EventArgs.h"
#include "EventUtils.h"
#include "ObjectContainer.h"
#include "ObjectImageButton.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBar.h"
#include "ObjectScrollBarButtonBackground.h"
#include "ObjectScrollBarButtonBackward.h"
#include "ObjectScrollBarButtonForward.h"
#include "ObjectScrollBarButtonSlider.h"

#define RETAIN_TIME 1.0f
#define FADE_OUT_TIME 0.25f

namespace aprilui
{
	float ScrollBar::ScrollDistance = 32.0f;
	float ScrollBar::GridSize = 0.0f;

	ScrollBar::ScrollBar(chstr name, grect rect) : Object(name, rect)
	{
		this->gridSize = GridSize;
		this->useFading = true;
		this->heightHide = true;
		this->buttonBackground = NULL;
		this->buttonSlider = NULL;
		this->buttonBackward = NULL;
		this->buttonForward = NULL;
		this->skinButtonBackground = false;
		this->skinButtonSlider = false;
		this->skinButtonForward = false;
		this->skinButtonBackward = false;
		this->_clickPosition.set(0.0f, 0.0f);
		this->_retainTime = 0.0f;
	}

	ScrollBar::~ScrollBar()
	{
	}

	void ScrollBar::setSkinName(chstr value)
	{
		this->skinName = value;
		this->notifyEvent("SkinChange", NULL);
	}

	void ScrollBar::_setButtonBackground(ScrollBarButtonBackground* button)
	{
		this->buttonBackground = button;
	}

	void ScrollBar::_setButtonSlider(ScrollBarButtonSlider* button)
	{
		this->buttonSlider = button;
	}

	void ScrollBar::_setButtonForward(ScrollBarButtonForward* button)
	{
		this->buttonForward = button;
	}

	void ScrollBar::_setButtonBackward(ScrollBarButtonBackward* button)
	{
		this->buttonBackward = button;
	}

	void ScrollBar::update(float k)
	{
		Object::update(k);
		this->_updateBar();
		if (this->skinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL && this->_retainTime > 0.0f)
				{
					this->_retainTime -= k;
				}
			}
		}
	}

	void ScrollBar::_initAreaDragging()
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent != NULL)
		{
			ScrollArea* area = parent->_getScrollArea();
			if (area != NULL)
			{
				if (area->_dragSpeed.x == 0.0f)
				{
					area->_lastScrollOffset.x = area->getScrollOffsetX();
					area->_dragTimer.x = 0.0f;
				}
				if (area->_dragSpeed.y == 0.0f)
				{
					area->_lastScrollOffset.y = area->getScrollOffsetY();
					area->_dragTimer.y = 0.0f;
				}
			}
		}
	}

	void ScrollBar::OnDraw()
	{
		Object::OnDraw();
		if (this->skinName == "")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL)
				{
					if (area->isDragging() || area->isScrolling())
					{
						this->_retainTime = RETAIN_TIME;
					}
					if ((!this->useFading || this->_retainTime > 0.0f) &&
						(!this->heightHide || this->_checkAreaSize()))
					{
						april::Color color = this->_getDrawColor();
						if (this->useFading && this->_retainTime < FADE_OUT_TIME)
						{
							color.a = (unsigned char)hclamp(color.a * this->_retainTime / FADE_OUT_TIME, 0.0f, 255.0f);
						}
						april::rendersys->drawFilledRect(this->_getBarDrawRect(), color);
					}
				}
			}
		}
	}

	void ScrollBar::notifyEvent(chstr name, void* params)
	{
		Object::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent == NULL)
			{
				hlog::warnf(aprilui::logTag, "ScrollBar '%s' not attached to object of class Container!", this->name.c_str());
			}
		}
		else if (name == "SkinChange")
		{
			if (this->skinName != "")
			{
				if (this->buttonBackground == NULL)
				{
					this->skinButtonBackground = true;
					this->buttonBackground = new ScrollBarButtonBackground(aprilui::generateName("aprilui::ScrollButtonBackground"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					this->registerChild(this->buttonBackground);
				}
				if (this->buttonSlider == NULL)
				{
					this->skinButtonSlider = true;
					this->buttonSlider = new ScrollBarButtonSlider(aprilui::generateName("aprilui::ScrollButtonSlider"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					this->registerChild(this->buttonSlider);
				}
				if (this->buttonForward == NULL)
				{
					this->skinButtonForward = true;
					this->buttonForward = new ScrollBarButtonForward(aprilui::generateName("aprilui::ScrollButtonForward"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					this->registerChild(this->buttonForward);
				}
				if (this->buttonBackward == NULL)
				{
					this->skinButtonBackward = true;
					this->buttonBackward = new ScrollBarButtonBackward(aprilui::generateName("aprilui::ScrollButtonBackward"), grect(0.0f, 0.0f, -1.0f, -1.0f));
					this->registerChild(this->buttonBackward);
				}
				this->buttonBackground->trySetImageByName(this->skinName + "/" + this->_getSkinNameBackground());
				this->buttonBackground->trySetHoverImageByName(this->skinName + "/" + this->_getSkinNameBackground());
				this->buttonBackground->trySetPushedImageByName(this->skinName + "/" + this->_getSkinNameBackground());
				this->buttonBackground->resizeToFitImage();
				this->buttonSlider->trySetImageByName(this->skinName + "/" + this->_getSkinNameSliderNormal());
				this->buttonSlider->trySetHoverImageByName(this->skinName + "/" + this->_getSkinNameSliderHover());
				this->buttonSlider->trySetPushedImageByName(this->skinName + "/" + this->_getSkinNameSliderPushed());
				this->buttonSlider->resizeToFitImage();
				this->buttonForward->trySetImageByName(this->skinName + "/" + this->_getSkinNameForwardNormal());
				this->buttonForward->trySetHoverImageByName(this->skinName + "/" + this->_getSkinNameForwardHover());
				this->buttonForward->trySetPushedImageByName(this->skinName + "/" + this->_getSkinNameForwardPushed());
				this->buttonForward->resizeToFitImage();
				this->buttonBackward->trySetImageByName(this->skinName + "/" + this->_getSkinNameBackwardNormal());
				this->buttonBackward->trySetHoverImageByName(this->skinName + "/" + this->_getSkinNameBackwardHover());
				this->buttonBackward->trySetPushedImageByName(this->skinName + "/" + this->_getSkinNameBackwardPushed());
				this->buttonBackward->resizeToFitImage();
				this->_updateChildren();
				this->_updateBar();
			}
			else
			{
				if (this->skinButtonBackground)
				{
					this->skinButtonBackground = false;
					this->unregisterChild(this->buttonBackground);
					delete this->buttonBackground;
					this->buttonBackground = NULL;
				}
				if (this->skinButtonSlider)
				{
					this->skinButtonSlider = false;
					this->unregisterChild(this->buttonSlider);
					delete this->buttonSlider;
					this->buttonSlider = NULL;
				}
				if (this->skinButtonForward)
				{
					this->skinButtonForward = false;
					this->unregisterChild(this->buttonForward);
					delete this->buttonForward;
					this->buttonForward = NULL;
				}
				if (this->skinButtonBackward)
				{
					this->skinButtonBackward = false;
					this->unregisterChild(this->buttonBackward);
					delete this->buttonBackward;
					this->buttonBackward = NULL;
				}
			}
		}
	}

	hstr ScrollBar::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "skin")			return this->getSkinName();
		if (name == "grid_size")	return this->getGridSize();
		if (name == "use_fading")	return this->isUseFading();
		if (name == "height_hide")	return this->isHeightHide();
		return Object::getProperty(name, propertyExists);
	}

	bool ScrollBar::setProperty(chstr name, chstr value)
	{
		if		(name == "skin")		this->setSkinName(value);
		else if	(name == "grid_size")	this->setGridSize(value);
		else if	(name == "use_fading")	this->setUseFading(value);
		else if	(name == "height_hide")	this->setHeightHide(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ScrollBar::onMouseMove()
	{
		if (Object::onMouseMove())
		{
			return true;
		}
		if (this->buttonSlider != NULL && this->buttonSlider->isPushed())
		{
			gvec2 position = aprilui::getCursorPosition() / this->getDerivedScale() - this->_clickPosition;
			this->_moveScrollBar(position.x, position.y);
		}
		return false;
	}

	bool ScrollBar::onMouseScroll(float x, float y)
	{
		if (Object::onMouseScroll(x, y))
		{
			return true;
		}
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent != NULL && (parent->isCursorInside() || this->isCursorInside()))
		{
			ScrollArea* area = parent->_getScrollArea();
			if (area != NULL && area->isSwapScrollWheels())
			{
				hswap(x, y);
			}
			this->addScrollValue(this->_calcScrollMove(x, y));
		}
		return false;
	}

	void ScrollBar::addScrollValueBackward(float multiplier)
	{
		this->addScrollValue(-hmax(habs(this->gridSize), (float)(int)(habs(ScrollBar::ScrollDistance) * multiplier)));
	}

	void ScrollBar::addScrollValueForward(float multiplier)
	{
		this->addScrollValue(hmax(habs(this->gridSize), (float)(int)(habs(ScrollBar::ScrollDistance) * multiplier)));
	}

}
