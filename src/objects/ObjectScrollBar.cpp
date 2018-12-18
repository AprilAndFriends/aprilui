/// @file
/// @version 5.1
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
	bool ScrollBar::useBackgroundInstantScroll = false;

	hmap<hstr, PropertyDescription> ScrollBar::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ScrollBar::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ScrollBar::_setters;

	ScrollBar::ScrollBar(chstr name) :
		Object(name)
	{
		this->gridSize = ScrollBar::defaultGridSize;
		this->scrollDistance = ScrollBar::defaultScrollDistance;
		this->useFading = true;
		this->heightHide = true;
		this->useStretchedSlider = true;
		this->disabledWhileScrolling = false;
		this->maxGridScroll = 0;
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

	ScrollBar::ScrollBar(const ScrollBar& other) :
		Object(other)
	{
		this->skinName = other.skinName;
		this->gridSize = other.gridSize;
		this->scrollDistance = other.scrollDistance;
		this->useFading = other.useFading;
		this->heightHide = other.heightHide;
		this->useStretchedSlider = other.useStretchedSlider;
		this->disabledWhileScrolling = other.disabledWhileScrolling;
		this->maxGridScroll = other.maxGridScroll;
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

	hmap<hstr, PropertyDescription>& ScrollBar::getPropertyDescriptions() const
	{
		if (ScrollBar::_propertyDescriptions.size() == 0)
		{
			ScrollBar::_propertyDescriptions = Object::getPropertyDescriptions();
			ScrollBar::_propertyDescriptions["skin"] = PropertyDescription("skin", PropertyDescription::Type::String);
			ScrollBar::_propertyDescriptions["grid_size"] = PropertyDescription("grid_size", PropertyDescription::Type::Float);
			ScrollBar::_propertyDescriptions["scroll_distance"] = PropertyDescription("scroll_distance", PropertyDescription::Type::Float);
			ScrollBar::_propertyDescriptions["use_fading"] = PropertyDescription("use_fading", PropertyDescription::Type::Bool);
			ScrollBar::_propertyDescriptions["height_hide"] = PropertyDescription("height_hide", PropertyDescription::Type::Bool);
			ScrollBar::_propertyDescriptions["use_stretched_slider"] = PropertyDescription("use_stretched_slider", PropertyDescription::Type::Bool);
			ScrollBar::_propertyDescriptions["disabled_while_scrolling"] = PropertyDescription("disabled_while_scrolling", PropertyDescription::Type::Bool);
			ScrollBar::_propertyDescriptions["max_grid_scroll"] = PropertyDescription("max_grid_scroll", PropertyDescription::Type::Int);
		}
		return ScrollBar::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ScrollBar::_getGetters() const
	{
		if (ScrollBar::_getters.size() == 0)
		{
			ScrollBar::_getters = Object::_getGetters();
			ScrollBar::_getters["skin"] = new PropertyDescription::Get<ScrollBar, hstr>(&ScrollBar::getSkinName);
			ScrollBar::_getters["grid_size"] = new PropertyDescription::Get<ScrollBar, float>(&ScrollBar::getGridSize);
			ScrollBar::_getters["scroll_distance"] = new PropertyDescription::Get<ScrollBar, float>(&ScrollBar::getScrollDistance);
			ScrollBar::_getters["use_fading"] = new PropertyDescription::Get<ScrollBar, bool>(&ScrollBar::isUseFading);
			ScrollBar::_getters["height_hide"] = new PropertyDescription::Get<ScrollBar, bool>(&ScrollBar::isHeightHide);
			ScrollBar::_getters["use_stretched_slider"] = new PropertyDescription::Get<ScrollBar, bool>(&ScrollBar::isUseStretchedSlider);
			ScrollBar::_getters["disabled_while_scrolling"] = new PropertyDescription::Get<ScrollBar, bool>(&ScrollBar::isDisabledWhileScrolling);
			ScrollBar::_getters["max_grid_scroll"] = new PropertyDescription::Get<ScrollBar, int>(&ScrollBar::getMaxGridScroll);
		}
		return ScrollBar::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ScrollBar::_getSetters() const
	{
		if (ScrollBar::_setters.size() == 0)
		{
			ScrollBar::_setters = Object::_getSetters();
			ScrollBar::_setters["skin"] = new PropertyDescription::Set<ScrollBar, hstr>(&ScrollBar::setSkinName);
			ScrollBar::_setters["grid_size"] = new PropertyDescription::Set<ScrollBar, float>(&ScrollBar::setGridSize);
			ScrollBar::_setters["scroll_distance"] = new PropertyDescription::Set<ScrollBar, float>(&ScrollBar::setScrollDistance);
			ScrollBar::_setters["use_fading"] = new PropertyDescription::Set<ScrollBar, bool>(&ScrollBar::setUseFading);
			ScrollBar::_setters["height_hide"] = new PropertyDescription::Set<ScrollBar, bool>(&ScrollBar::setHeightHide);
			ScrollBar::_setters["use_stretched_slider"] = new PropertyDescription::Set<ScrollBar, bool>(&ScrollBar::setUseStretchedSlider);
			ScrollBar::_setters["disabled_while_scrolling"] = new PropertyDescription::Set<ScrollBar, bool>(&ScrollBar::setDisabledWhileScrolling);
			ScrollBar::_setters["max_grid_scroll"] = new PropertyDescription::Set<ScrollBar, int>(&ScrollBar::setMaxGridScroll);
		}
		return ScrollBar::_setters;
	}

	void ScrollBar::setSkinName(chstr value)
	{
		if (this->skinName != value)
		{
			this->skinName = value;
			this->notifyEvent(Event::ScrollSkinChanged, NULL);
		}
	}

	ScrollBarButtonBackground* ScrollBar::_getButtonBackground() const
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

	ScrollBarButtonSlider* ScrollBar::_getButtonSlider() const
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

	ScrollBarButtonForward* ScrollBar::_getButtonForward() const
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

	ScrollBarButtonBackward* ScrollBar::_getButtonBackward() const
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
				if ((!this->useFading || this->_retainTime > 0.0f) && (!this->heightHide || this->_checkAreaSize()))
				{
					april::Color drawColor = this->_makeDrawColor();
					if (this->useFading && this->_retainTime < FADE_OUT_TIME)
					{
						drawColor.a = (unsigned char)hclamp(drawColor.a * this->_retainTime / FADE_OUT_TIME, 0.0f, 255.0f);
					}
					april::rendersys->setBlendMode(april::BlendMode::Alpha);
					april::rendersys->setColorMode(april::ColorMode::Multiply);
					april::rendersys->drawFilledRect(this->_getBarDrawRect(), drawColor);
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
				this->_updateButtonBackgroundSkinned();
				this->_updateButtonSliderSkinned();
				this->_updateButtonForwardkinned();
				this->_updateButtonBackwardSkinned();
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

	void ScrollBar::_updateButtonBackgroundSkinned()
	{
		hstr imageName = this->skinName + "/" + this->_getSkinNameBackground();
		if (aprilui::hasImage(imageName, this->dataset))
		{
			if (this->_buttonBackgroundSkinned == NULL)
			{
				this->_buttonBackgroundSkinned = new ScrollBarButtonBackground(april::generateName("aprilui::ScrollButtonBackground"));
				this->registerChild(this->_buttonBackgroundSkinned);
			}
			this->_buttonBackgroundSkinned->trySetImageByName(imageName);
			this->_buttonBackgroundSkinned->trySetHoverImageByName(imageName);
			this->_buttonBackgroundSkinned->trySetPushedImageByName(imageName);
			this->_buttonBackgroundSkinned->resizeToFitImage();
			this->_buttonBackgroundSkinned->setVisible(this->_buttonBackground == NULL);
		}
		else if (this->_buttonBackgroundSkinned != NULL)
		{
			this->unregisterChild(this->_buttonBackgroundSkinned);
			delete this->_buttonBackgroundSkinned;
			this->_buttonBackgroundSkinned = NULL;
		}
	}

	void ScrollBar::_updateButtonSliderSkinned()
	{
		hstr imageName = this->skinName + "/" + this->_getSkinNameSliderNormal();
		if (aprilui::hasImage(imageName, this->dataset))
		{
			if (this->_buttonSliderSkinned == NULL)
			{
				this->_buttonSliderSkinned = new ScrollBarButtonSlider(april::generateName("aprilui::ScrollButtonSlider"));
				this->registerChild(this->_buttonSliderSkinned);
			}
			this->_buttonSliderSkinned->trySetImageByName(imageName);
			hstr hoverImageName = this->skinName + "/" + this->_getSkinNameSliderHover();
			this->_buttonSliderSkinned->trySetHoverImageByName(aprilui::hasImage(hoverImageName, this->dataset) ? hoverImageName : "");
			hstr pushedImageName = this->skinName + "/" + this->_getSkinNameSliderPushed();
			this->_buttonSliderSkinned->trySetPushedImageByName(aprilui::hasImage(pushedImageName, this->dataset) ? pushedImageName : "");
			this->_buttonSliderSkinned->resizeToFitImage();
			this->_buttonSliderSkinned->setVisible(this->_buttonSlider == NULL);
		}
		else if (this->_buttonSliderSkinned != NULL)
		{
			this->unregisterChild(this->_buttonSliderSkinned);
			delete this->_buttonSliderSkinned;
			this->_buttonSliderSkinned = NULL;
		}
	}

	void ScrollBar::_updateButtonForwardkinned()
	{
		hstr imageName = this->skinName + "/" + this->_getSkinNameForwardNormal();
		if (aprilui::hasImage(imageName, this->dataset))
		{
			if (this->_buttonForwardSkinned == NULL)
			{
				this->_buttonForwardSkinned = new ScrollBarButtonForward(april::generateName("aprilui::ScrollButtonForward"));
				this->registerChild(this->_buttonForwardSkinned);
			}
			this->_buttonForwardSkinned->trySetImageByName(imageName);
			hstr hoverImageName = this->skinName + "/" + this->_getSkinNameForwardHover();
			this->_buttonForwardSkinned->trySetHoverImageByName(aprilui::hasImage(hoverImageName, this->dataset) ? hoverImageName : "");
			hstr pushedImageName = this->skinName + "/" + this->_getSkinNameForwardPushed();
			this->_buttonForwardSkinned->trySetPushedImageByName(aprilui::hasImage(pushedImageName, this->dataset) ? pushedImageName : "");
			this->_buttonForwardSkinned->resizeToFitImage();
			this->_buttonForwardSkinned->setVisible(this->_buttonForward == NULL);
		}
		else if (this->_buttonForwardSkinned != NULL)
		{
			this->unregisterChild(this->_buttonForwardSkinned);
			delete this->_buttonForwardSkinned;
			this->_buttonForwardSkinned = NULL;
		}
	}

	void ScrollBar::_updateButtonBackwardSkinned()
	{
		hstr imageName = this->skinName + "/" + this->_getSkinNameBackwardNormal();
		if (aprilui::hasImage(imageName, this->dataset))
		{
			if (this->_buttonBackwardSkinned == NULL)
			{
				this->_buttonBackwardSkinned = new ScrollBarButtonBackward(april::generateName("aprilui::ScrollButtonBackward"));
				this->registerChild(this->_buttonBackwardSkinned);
			}
			this->_buttonBackwardSkinned->trySetImageByName(imageName);
			hstr hoverImageName = this->skinName + "/" + this->_getSkinNameBackwardHover();
			this->_buttonBackwardSkinned->trySetHoverImageByName(aprilui::hasImage(hoverImageName, this->dataset) ? hoverImageName : "");
			hstr pushedImageName = this->skinName + "/" + this->_getSkinNameBackwardPushed();
			this->_buttonBackwardSkinned->trySetPushedImageByName(aprilui::hasImage(pushedImageName, this->dataset) ? pushedImageName : "");
			this->_buttonBackwardSkinned->resizeToFitImage();
			this->_buttonBackwardSkinned->setVisible(this->_buttonBackward == NULL);
		}
		else if (this->_buttonBackwardSkinned != NULL)
		{
			this->unregisterChild(this->_buttonBackwardSkinned);
			delete this->_buttonBackwardSkinned;
			this->_buttonBackwardSkinned = NULL;
		}
	}

	bool ScrollBar::_mouseMove()
	{
		ScrollBarButtonSlider* buttonSlider = this->_getButtonSlider();
		if (buttonSlider != NULL && buttonSlider->isPushed())
		{
			ScrollBarButtonBackground* buttonBackground = this->_getButtonBackground();
			if (buttonBackground != NULL)
			{
				gvec2f position = buttonBackground->transformToLocalSpace(aprilui::getCursorPosition()) - this->_clickPosition;
				this->_moveScrollBar(position.x, position.y);
			}
		}
		return Object::_mouseMove();
	}

	bool ScrollBar::_canAddScrollValue() const
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
			this->addScrollValue(-hmax(habs(this->gridSize), (float)(int)(habs(this->scrollDistance) * multiplier)));
		}
	}

	void ScrollBar::addScrollValueForward(float multiplier)
	{
		if (this->_canAddScrollValue())
		{
			this->addScrollValue(hmax(habs(this->gridSize), (float)(int)(habs(this->scrollDistance) * multiplier)));
		}
	}

}
