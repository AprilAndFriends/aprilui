/// @file
/// @version 4.0
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
#include "CallbackEvent.h"
#include "EventArgs.h"
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
	float ScrollBar::defaultScrollDistance = 32.0f;
	float ScrollBar::defaultGridSize = 0.0f;

	harray<PropertyDescription> ScrollBar::_propertyDescriptions;

	ScrollBar::ScrollBar(chstr name) : Object(name)
	{
		this->gridSize = ScrollBar::defaultGridSize;
		this->useFading = true;
		this->heightHide = true;
		this->useStretchedSlider = true;
		this->disabledWhileScrolling = false;
		this->_buttonBackground = NULL;
		this->_buttonSlider = NULL;
		this->_buttonBackward = NULL;
		this->_buttonForward = NULL;
		this->_buttonBackgroundSkinned = NULL;
		this->_buttonSliderSkinned = NULL;
		this->_buttonBackwardSkinned = NULL;
		this->_buttonForwardSkinned = NULL;
		this->_retainTime = 0.0f;
		this->_scrolling = false;
	}

	ScrollBar::ScrollBar(const ScrollBar& other) : Object(other)
	{
		this->skinName = other.skinName;
		this->gridSize = other.gridSize;
		this->useFading = other.useFading;
		this->heightHide = other.heightHide;
		this->useStretchedSlider = other.useStretchedSlider;
		this->disabledWhileScrolling = other.disabledWhileScrolling;
		this->_buttonBackground = (other._buttonBackground != NULL ? other._buttonBackground->clone() : NULL);
		this->_buttonSlider = (other._buttonSlider != NULL ? other._buttonSlider->clone() : NULL);
		this->_buttonBackward = (other._buttonBackward != NULL ? other._buttonBackward->clone() : NULL);
		this->_buttonForward = (other._buttonForward != NULL ? other._buttonForward->clone() : NULL);
		this->_buttonBackgroundSkinned = (other._buttonBackgroundSkinned != NULL ? other._buttonBackgroundSkinned->clone() : NULL);
		this->_buttonSliderSkinned = (other._buttonSliderSkinned != NULL ? other._buttonSliderSkinned->clone() : NULL);
		this->_buttonBackwardSkinned = (other._buttonBackwardSkinned != NULL ? other._buttonBackwardSkinned->clone() : NULL);
		this->_buttonForwardSkinned = (other._buttonForwardSkinned != NULL ? other._buttonForwardSkinned->clone() : NULL);
		this->_retainTime = 0.0f;
		this->_scrolling = false;
	}

	ScrollBar::~ScrollBar()
	{
	}

	void ScrollBar::setSkinName(chstr value)
	{
		this->skinName = value;
		this->notifyEvent(Event::ScrollSkinChanged, NULL);
	}

	harray<PropertyDescription> ScrollBar::getPropertyDescriptions()
	{
		if (ScrollBar::_propertyDescriptions.size() == 0)
		{
			ScrollBar::_propertyDescriptions += PropertyDescription("skin", PropertyDescription::STRING);
			ScrollBar::_propertyDescriptions += PropertyDescription("grid_size", PropertyDescription::FLOAT);
			ScrollBar::_propertyDescriptions += PropertyDescription("use_fading", PropertyDescription::BOOL);
			ScrollBar::_propertyDescriptions += PropertyDescription("height_hide", PropertyDescription::BOOL);
			ScrollBar::_propertyDescriptions += PropertyDescription("use_stretched_slider", PropertyDescription::BOOL);
			ScrollBar::_propertyDescriptions += PropertyDescription("disabled_while_scrolling", PropertyDescription::BOOL);
		}
		return (Object::getPropertyDescriptions() + ScrollBar::_propertyDescriptions);
	}

	ScrollBarButtonBackground* ScrollBar::_getButtonBackground()
	{
		return (this->_buttonBackground != NULL ? this->_buttonBackground : this->_buttonBackgroundSkinned);
	}

	void ScrollBar::_setButtonBackground(ScrollBarButtonBackground* button)
	{
		if (this->_buttonBackgroundSkinned != button)
		{
			this->_buttonBackground = button;
		}
		if (this->_buttonBackgroundSkinned != NULL)
		{
			this->_buttonBackgroundSkinned->setVisible(this->_buttonBackground == NULL);
		}
	}

	void ScrollBar::_unsetButtonBackground(ScrollBarButtonBackground* button)
	{
		if (this->_buttonBackgroundSkinned != button)
		{
			this->_buttonBackground = NULL;
		}
		if (this->_buttonBackgroundSkinned != NULL)
		{
			this->_buttonBackgroundSkinned->setVisible(this->_buttonBackground == NULL);
		}
	}

	ScrollBarButtonSlider* ScrollBar::_getButtonSlider()
	{
		return (this->_buttonSlider != NULL ? this->_buttonSlider : this->_buttonSliderSkinned);
	}

	void ScrollBar::_setButtonSlider(ScrollBarButtonSlider* button)
	{
		if (this->_buttonSliderSkinned != button)
		{
			this->_buttonSlider = button;
		}
		if (this->_buttonSliderSkinned != NULL)
		{
			this->_buttonSliderSkinned->setVisible(this->_buttonSlider == NULL);
		}
	}

	void ScrollBar::_unsetButtonSlider(ScrollBarButtonSlider* button)
	{
		if (this->_buttonSliderSkinned != button)
		{
			this->_buttonSlider = NULL;
		}
		if (this->_buttonSliderSkinned != NULL)
		{
			this->_buttonSliderSkinned->setVisible(this->_buttonSlider == NULL);
		}
	}

	ScrollBarButtonForward* ScrollBar::_getButtonForward()
	{
		return (this->_buttonForward != NULL ? this->_buttonForward : this->_buttonForwardSkinned);
	}

	void ScrollBar::_setButtonForward(ScrollBarButtonForward* button)
	{
		if (this->_buttonForwardSkinned != button)
		{
			this->_buttonForward = button;
		}
		if (this->_buttonForwardSkinned != NULL)
		{
			this->_buttonForwardSkinned->setVisible(this->_buttonForward == NULL);
		}
	}

	void ScrollBar::_unsetButtonForward(ScrollBarButtonForward* button)
	{
		if (this->_buttonForwardSkinned != button)
		{
			this->_buttonForward = NULL;
		}
		if (this->_buttonForwardSkinned != NULL)
		{
			this->_buttonForwardSkinned->setVisible(this->_buttonForward == NULL);
		}
	}

	ScrollBarButtonBackward* ScrollBar::_getButtonBackward()
	{
		return (this->_buttonBackward != NULL ? this->_buttonBackward : this->_buttonBackwardSkinned);
	}

	void ScrollBar::_setButtonBackward(ScrollBarButtonBackward* button)
	{
		if (this->_buttonBackwardSkinned != button)
		{
			this->_buttonBackward = button;
		}
		if (this->_buttonBackwardSkinned != NULL)
		{
			this->_buttonBackwardSkinned->setVisible(this->_buttonBackward == NULL);
		}
	}

	void ScrollBar::_unsetButtonBackward(ScrollBarButtonBackward* button)
	{
		if (this->_buttonBackwardSkinned != button)
		{
			this->_buttonBackward = NULL;
		}
		if (this->_buttonBackwardSkinned != NULL)
		{
			this->_buttonBackwardSkinned->setVisible(this->_buttonBackward == NULL);
		}
	}

	void ScrollBar::_update(float timeDelta)
	{
		Object::_update(timeDelta);
		this->_updateBar();
		if (this->skinName == "" && this->_buttonSlider == NULL)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL && parent->scrollArea != NULL && this->_retainTime > 0.0f)
			{
				this->_retainTime -= timeDelta;
			}
		}
	}

	void ScrollBar::_initAreaDragging()
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent != NULL && parent->scrollArea != NULL)
		{
			if (parent->scrollArea->_dragSpeed.x == 0.0f)
			{
				parent->scrollArea->_lastScrollOffset.x = parent->scrollArea->getScrollOffsetX();
				parent->scrollArea->_dragTimer.x = 0.0f;
			}
			if (parent->scrollArea->_dragSpeed.y == 0.0f)
			{
				parent->scrollArea->_lastScrollOffset.y = parent->scrollArea->getScrollOffsetY();
				parent->scrollArea->_dragTimer.y = 0.0f;
			}
		}
	}

	void ScrollBar::_draw()
	{
		Object::_draw();
		if (this->skinName == "" && this->_buttonSlider == NULL)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL && parent->scrollArea != NULL)
			{
				if (parent->scrollArea->isDragging() || parent->scrollArea->isScrolling())
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

	void ScrollBar::notifyEvent(chstr type, EventArgs* args)
	{
		Object::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent == NULL)
			{
				hlog::warnf(logTag, "ScrollBar '%s' not attached to object of class Container!", this->name.cStr());
			}
		}
		else if (type == Event::ScrollSkinChanged)
		{
			if (this->skinName != "")
			{
				if (this->_buttonBackgroundSkinned == NULL)
				{
					this->_buttonBackgroundSkinned = new ScrollBarButtonBackground(april::generateName("aprilui::ScrollButtonBackground"));
					this->registerChild(this->_buttonBackgroundSkinned);
				}
				if (this->_buttonSliderSkinned == NULL)
				{
					this->_buttonSliderSkinned = new ScrollBarButtonSlider(april::generateName("aprilui::ScrollButtonSlider"));
					this->registerChild(this->_buttonSliderSkinned);
				}
				if (this->_buttonForwardSkinned == NULL)
				{
					this->_buttonForwardSkinned = new ScrollBarButtonForward(april::generateName("aprilui::ScrollButtonForward"));
					this->registerChild(this->_buttonForwardSkinned);
				}
				if (this->_buttonBackwardSkinned == NULL)
				{
					this->_buttonBackwardSkinned = new ScrollBarButtonBackward(april::generateName("aprilui::ScrollButtonBackward"));
					this->registerChild(this->_buttonBackwardSkinned);
				}
				this->_buttonBackgroundSkinned->trySetImageByName(this->skinName + "/" + this->_getSkinNameBackground());
				this->_buttonBackgroundSkinned->trySetHoverImageByName(this->skinName + "/" + this->_getSkinNameBackground());
				this->_buttonBackgroundSkinned->trySetPushedImageByName(this->skinName + "/" + this->_getSkinNameBackground());
				this->_buttonBackgroundSkinned->resizeToFitImage();
				this->_buttonBackgroundSkinned->setVisible(this->_buttonBackground == NULL);
				this->_buttonSliderSkinned->trySetImageByName(this->skinName + "/" + this->_getSkinNameSliderNormal());
				this->_buttonSliderSkinned->trySetHoverImageByName(this->skinName + "/" + this->_getSkinNameSliderHover());
				this->_buttonSliderSkinned->trySetPushedImageByName(this->skinName + "/" + this->_getSkinNameSliderPushed());
				this->_buttonSliderSkinned->resizeToFitImage();
				this->_buttonSliderSkinned->setVisible(this->_buttonSlider == NULL);
				this->_buttonForwardSkinned->trySetImageByName(this->skinName + "/" + this->_getSkinNameForwardNormal());
				this->_buttonForwardSkinned->trySetHoverImageByName(this->skinName + "/" + this->_getSkinNameForwardHover());
				this->_buttonForwardSkinned->trySetPushedImageByName(this->skinName + "/" + this->_getSkinNameForwardPushed());
				this->_buttonForwardSkinned->resizeToFitImage();
				this->_buttonForwardSkinned->setVisible(this->_buttonForward == NULL);
				this->_buttonBackwardSkinned->trySetImageByName(this->skinName + "/" + this->_getSkinNameBackwardNormal());
				this->_buttonBackwardSkinned->trySetHoverImageByName(this->skinName + "/" + this->_getSkinNameBackwardHover());
				this->_buttonBackwardSkinned->trySetPushedImageByName(this->skinName + "/" + this->_getSkinNameBackwardPushed());
				this->_buttonBackwardSkinned->resizeToFitImage();
				this->_buttonBackwardSkinned->setVisible(this->_buttonBackward == NULL);
				this->_updateChildren();
				this->_updateBar();
			}
			else
			{
				if (this->_buttonBackgroundSkinned != NULL)
				{
					this->unregisterChild(this->_buttonBackgroundSkinned);
					delete this->_buttonBackgroundSkinned;
					this->_buttonBackgroundSkinned = NULL;
				}
				if (this->_buttonSliderSkinned != NULL)
				{
					this->unregisterChild(this->_buttonSliderSkinned);
					delete this->_buttonSliderSkinned;
					this->_buttonSliderSkinned = NULL;
				}
				if (this->_buttonForwardSkinned != NULL)
				{
					this->unregisterChild(this->_buttonForwardSkinned);
					delete this->_buttonForwardSkinned;
					this->_buttonForwardSkinned = NULL;
				}
				if (this->_buttonBackwardSkinned != NULL)
				{
					this->unregisterChild(this->_buttonBackwardSkinned);
					delete this->_buttonBackwardSkinned;
					this->_buttonBackwardSkinned = NULL;
				}
			}
		}
	}

	hstr ScrollBar::getProperty(chstr name)
	{
		if (name == "skin")						return this->getSkinName();
		if (name == "grid_size")				return this->getGridSize();
		if (name == "use_fading")				return this->isUseFading();
		if (name == "height_hide")				return this->isHeightHide();
		if (name == "use_stretched_slider")		return this->isUseStretchedSlider();
		if (name == "disabled_while_scrolling")	return this->isDisabledWhileScrolling();
		return Object::getProperty(name);
	}

	bool ScrollBar::setProperty(chstr name, chstr value)
	{
		if		(name == "skin")						this->setSkinName(value);
		else if	(name == "grid_size")					this->setGridSize(value);
		else if	(name == "use_fading")					this->setUseFading(value);
		else if	(name == "height_hide")					this->setHeightHide(value);
		else if	(name == "use_stretched_slider")		this->setUseStretchedSlider(value);
		else if	(name == "disabled_while_scrolling")	this->setDisabledWhileScrolling(value);
		else return Object::setProperty(name, value);
		return true;
	}

	bool ScrollBar::_mouseMove()
	{
		ScrollBarButtonSlider* buttonSlider = this->_getButtonSlider();
		if (buttonSlider != NULL && buttonSlider->isPushed())
		{
			ScrollBarButtonBackground* buttonBackground = this->_getButtonBackground();
			if (buttonBackground != NULL)
			{
				gvec2 position = buttonBackground->transformToLocalSpace(aprilui::getCursorPosition()) - this->_clickPosition;
				this->_moveScrollBar(position.x, position.y);
			}
		}
		return Object::_mouseMove();
	}

	bool ScrollBar::_canAddScrollValue()
	{
		if (!this->disabledWhileScrolling)
		{
			return true;
		}
		Container* parent = dynamic_cast<Container*>(this->parent);
		return (parent != NULL && !parent->scrollArea->isScrolling());
	}

	void ScrollBar::addScrollValueBackground(float value)
	{
		if (this->_canAddScrollValue())
		{
			this->addScrollValue(value);
		}
	}

	void ScrollBar::addScrollValueBackward(float multiplier)
	{
		if (this->_canAddScrollValue())
		{
			this->addScrollValue(-hmax(habs(this->gridSize), (float)(int)(habs(ScrollBar::defaultScrollDistance) * multiplier)));
		}
	}

	void ScrollBar::addScrollValueForward(float multiplier)
	{
		if (this->_canAddScrollValue())
		{
			this->addScrollValue(hmax(habs(this->gridSize), (float)(int)(habs(ScrollBar::defaultScrollDistance) * multiplier)));
		}
	}

}
