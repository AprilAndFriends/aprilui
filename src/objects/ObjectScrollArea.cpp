/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <april/Keys.h>
#include <april/Window.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectContainer.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBar.h"
#include "ObjectScrollBarH.h"
#include "ObjectScrollBarV.h"

namespace aprilui
{
	float ScrollArea::defaultInertia = 2000.0f;
	gvec2 ScrollArea::defaultDragThreshold(16.0f, 16.0f);
	float ScrollArea::defaultDragMaxSpeed = 0.0f;

	harray<PropertyDescription> ScrollArea::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ScrollArea::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ScrollArea::_setters;

	ScrollArea::ScrollArea(chstr name) : Object(name), ButtonBase()
	{
		this->clip = true;
		this->allowDrag = false;
		this->inertia = ScrollArea::defaultInertia;
		this->dragThreshold = ScrollArea::defaultDragThreshold;
		this->dragMaxSpeed = ScrollArea::defaultDragMaxSpeed;
		this->swapScrollWheels = false;
		this->oobChildrenFadeSizeFactor.set(-1.0f, -1.0f);
		this->oobChildrenFadeOffsetFactor.set(0.0f, 0.0f);
		this->optimizeOobChildrenVisible = false;
		this->optimizeOobChildrenAwake = false;
		this->dragging = false;
		this->debugColor.set(april::Color::Yellow, 32);
		this->_overrideHoverMode = false;
	}

	ScrollArea::ScrollArea(const ScrollArea& other) : Object(other), ButtonBase(other)
	{
		this->allowDrag = other.allowDrag;
		this->inertia = other.inertia;
		this->dragThreshold = other.dragThreshold;
		this->dragMaxSpeed = other.dragMaxSpeed;
		this->swapScrollWheels = other.swapScrollWheels;
		this->oobChildrenFadeSizeFactor = other.oobChildrenFadeSizeFactor;
		this->oobChildrenFadeOffsetFactor = other.oobChildrenFadeOffsetFactor;
		this->optimizeOobChildrenVisible = other.optimizeOobChildrenVisible;
		this->optimizeOobChildrenAwake = other.optimizeOobChildrenAwake;
		this->dragging = false;
		this->_overrideHoverMode = false;
	}

	ScrollArea::~ScrollArea()
	{
	}

	Object* ScrollArea::createInstance(chstr name)
	{
		return new ScrollArea(name);
	}

	harray<PropertyDescription> ScrollArea::getPropertyDescriptions() const
	{
		if (ScrollArea::_propertyDescriptions.size() == 0)
		{
			ScrollArea::_propertyDescriptions = Object::getPropertyDescriptions() + ButtonBase::getPropertyDescriptions();
			ScrollArea::_propertyDescriptions += PropertyDescription("allow_drag", PropertyDescription::Type::Bool);
			ScrollArea::_propertyDescriptions += PropertyDescription("inertia", PropertyDescription::Type::Float);
			ScrollArea::_propertyDescriptions += PropertyDescription("drag_threshold", PropertyDescription::Type::Gvec2);
			ScrollArea::_propertyDescriptions += PropertyDescription("drag_threshold_x", PropertyDescription::Type::Float);
			ScrollArea::_propertyDescriptions += PropertyDescription("drag_threshold_y", PropertyDescription::Type::Float);
			ScrollArea::_propertyDescriptions += PropertyDescription("drag_max_speed", PropertyDescription::Type::Float);
			ScrollArea::_propertyDescriptions += PropertyDescription("swap_scroll_wheels", PropertyDescription::Type::Bool);
			ScrollArea::_propertyDescriptions += PropertyDescription("oob_children_fade_size_factor", PropertyDescription::Type::Gvec2);
			ScrollArea::_propertyDescriptions += PropertyDescription("oob_children_fade_size_factor_x", PropertyDescription::Type::Float);
			ScrollArea::_propertyDescriptions += PropertyDescription("oob_children_fade_size_factor_y", PropertyDescription::Type::Float);
			ScrollArea::_propertyDescriptions += PropertyDescription("oob_children_fade_offset_factor", PropertyDescription::Type::Gvec2);
			ScrollArea::_propertyDescriptions += PropertyDescription("oob_children_fade_offset_factor_x", PropertyDescription::Type::Float);
			ScrollArea::_propertyDescriptions += PropertyDescription("oob_children_fade_offset_factor_y", PropertyDescription::Type::Float);
			ScrollArea::_propertyDescriptions += PropertyDescription("optimize_oob_children_visible", PropertyDescription::Type::Bool);
			ScrollArea::_propertyDescriptions += PropertyDescription("optimize_oob_children_awake", PropertyDescription::Type::Bool);
		}
		return ScrollArea::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ScrollArea::_getGetters() const
	{
		if (ScrollArea::_getters.size() == 0)
		{
			ScrollArea::_getters = Object::_getGetters() + ButtonBase::_getGetters();
			ScrollArea::_getters["allow_drag"] = new PropertyDescription::Get<ScrollArea, bool>(&ScrollArea::isAllowDrag);
			ScrollArea::_getters["inertia"] = new PropertyDescription::Get<ScrollArea, float>(&ScrollArea::getInertia);
			ScrollArea::_getters["drag_threshold"] = new PropertyDescription::GetGvec2<ScrollArea>(&ScrollArea::getDragThreshold);
			ScrollArea::_getters["drag_threshold_x"] = new PropertyDescription::Get<ScrollArea, float>(&ScrollArea::getDragThresholdX);
			ScrollArea::_getters["drag_threshold_y"] = new PropertyDescription::Get<ScrollArea, float>(&ScrollArea::getDragThresholdY);
			ScrollArea::_getters["drag_max_speed"] = new PropertyDescription::Get<ScrollArea, float>(&ScrollArea::getDragMaxSpeed);
			ScrollArea::_getters["swap_scroll_wheels"] = new PropertyDescription::Get<ScrollArea, bool>(&ScrollArea::isSwapScrollWheels);
			ScrollArea::_getters["oob_children_fade_size_factor"] = new PropertyDescription::GetGvec2<ScrollArea>(&ScrollArea::getOobChildrenFadeSizeFactor);
			ScrollArea::_getters["oob_children_fade_size_factor_x"] = new PropertyDescription::Get<ScrollArea, float>(&ScrollArea::getOobChildrenFadeSizeFactorX);
			ScrollArea::_getters["oob_children_fade_size_factor_y"] = new PropertyDescription::Get<ScrollArea, float>(&ScrollArea::getOobChildrenFadeSizeFactorY);
			ScrollArea::_getters["oob_children_fade_offset_factor"] = new PropertyDescription::GetGvec2<ScrollArea>(&ScrollArea::getOobChildrenFadeOffsetFactor);
			ScrollArea::_getters["oob_children_fade_offset_factor_x"] = new PropertyDescription::Get<ScrollArea, float>(&ScrollArea::getOobChildrenFadeOffsetFactorX);
			ScrollArea::_getters["oob_children_fade_offset_factor_y"] = new PropertyDescription::Get<ScrollArea, float>(&ScrollArea::getOobChildrenFadeOffsetFactorY);
			ScrollArea::_getters["optimize_oob_children_visible"] = new PropertyDescription::Get<ScrollArea, bool>(&ScrollArea::isOptimizeOobChildrenVisible);
			ScrollArea::_getters["optimize_oob_children_awake"] = new PropertyDescription::Get<ScrollArea, bool>(&ScrollArea::isOptimizeOobChildrenAwake);
		}
		return ScrollArea::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ScrollArea::_getSetters() const
	{
		if (ScrollArea::_setters.size() == 0)
		{
			ScrollArea::_setters = Object::_getSetters() + ButtonBase::_getSetters();
			ScrollArea::_setters["allow_drag"] = new PropertyDescription::Set<ScrollArea, bool>(&ScrollArea::setAllowDrag);
			ScrollArea::_setters["inertia"] = new PropertyDescription::Set<ScrollArea, float>(&ScrollArea::setInertia);
			ScrollArea::_setters["drag_threshold"] = new PropertyDescription::SetGvec2<ScrollArea>(&ScrollArea::setDragThreshold);
			ScrollArea::_setters["drag_threshold_x"] = new PropertyDescription::Set<ScrollArea, float>(&ScrollArea::setDragThresholdX);
			ScrollArea::_setters["drag_threshold_y"] = new PropertyDescription::Set<ScrollArea, float>(&ScrollArea::setDragThresholdY);
			ScrollArea::_setters["drag_max_speed"] = new PropertyDescription::Set<ScrollArea, float>(&ScrollArea::setDragMaxSpeed);
			ScrollArea::_setters["swap_scroll_wheels"] = new PropertyDescription::Set<ScrollArea, bool>(&ScrollArea::setSwapScrollWheels);
			ScrollArea::_setters["oob_children_fade_size_factor"] = new PropertyDescription::SetGvec2<ScrollArea>(&ScrollArea::setOobChildrenFadeSizeFactor);
			ScrollArea::_setters["oob_children_fade_size_factor_x"] = new PropertyDescription::Set<ScrollArea, float>(&ScrollArea::setOobChildrenFadeSizeFactorX);
			ScrollArea::_setters["oob_children_fade_size_factor_y"] = new PropertyDescription::Set<ScrollArea, float>(&ScrollArea::setOobChildrenFadeSizeFactorY);
			ScrollArea::_setters["oob_children_fade_offset_factor"] = new PropertyDescription::SetGvec2<ScrollArea>(&ScrollArea::setOobChildrenFadeOffsetFactor);
			ScrollArea::_setters["oob_children_fade_offset_factor_x"] = new PropertyDescription::Set<ScrollArea, float>(&ScrollArea::setOobChildrenFadeOffsetFactorX);
			ScrollArea::_setters["oob_children_fade_offset_factor_y"] = new PropertyDescription::Set<ScrollArea, float>(&ScrollArea::setOobChildrenFadeOffsetFactorY);
			ScrollArea::_setters["optimize_oob_children_visible"] = new PropertyDescription::Set<ScrollArea, bool>(&ScrollArea::setOptimizeOobChildrenVisible);
			ScrollArea::_setters["optimize_oob_children_awake"] = new PropertyDescription::Set<ScrollArea, bool>(&ScrollArea::setOptimizeOobChildrenAwake);
		}
		return ScrollArea::_setters;
	}

	hstr ScrollArea::getName() const
	{
		return Object::getName();
	}

	int ScrollArea::getFocusIndex() const
	{
		return Object::getFocusIndex();
	}

	bool ScrollArea::isCursorInside() const
	{
		return Object::isCursorInside();
	}

	Object* ScrollArea::getParent() const
	{
		return Object::getParent();
	}

	Dataset* ScrollArea::getDataset() const
	{
		return Object::getDataset();
	}

	bool ScrollArea::isScrolling() const
	{
		return (!this->pushed && (this->_dragSpeed.x != 0.0f || this->_dragSpeed.y != 0.0f));
	}
	
	bool ScrollArea::isScrollable() const
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		return (parent != NULL && (this->rect.w > parent->rect.w || this->rect.h > parent->rect.h));
	}
	
	bool ScrollArea::isScrollableX() const
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		return (parent != NULL && this->rect.w > parent->rect.w);
	}
	
	bool ScrollArea::isScrollableY() const
	{
		Container* parent = dynamic_cast<Container*>(this->parent);
		return (parent != NULL && this->rect.h > parent->rect.h);
	}

	gvec2 ScrollArea::getScrollOffset() const
	{
		return -this->getPosition();
	}

	void ScrollArea::setScrollOffset(cgvec2 value)
	{
		this->setScrollOffsetX(value.x);
		this->setScrollOffsetY(value.y);
	}

	void ScrollArea::setScrollOffset(float x, float y)
	{
		this->setScrollOffsetX(x);
		this->setScrollOffsetY(y);
	}

	float ScrollArea::getScrollOffsetX() const
	{
		return -this->getX();
	}

	void ScrollArea::setScrollOffsetX(float value)
	{
		this->setX(this->parent != NULL ? hclamp(-value, hmin(this->parent->getWidth() - this->getWidth(), 0.0f), 0.0f) : -value);
		this->_updateOobChildren();
	}

	float ScrollArea::getScrollOffsetY() const
	{
		return -this->getY();
	}

	void ScrollArea::setScrollOffsetY(float value)
	{
		this->setY(this->parent != NULL ? hclamp(-value, hmin(this->parent->getHeight() - this->getHeight(), 0.0f), 0.0f) : -value);
		this->_updateOobChildren();
	}

	void ScrollArea::_update(float timeDelta)
	{
		this->_updateOobChildren();
		Object::_update(timeDelta);
		if (this->allowDrag && this->parent != NULL)
		{
			ButtonBase::_update(timeDelta);
			if (timeDelta > 0.0f) // don't update this if no time has passed
			{
				gvec2 position = this->rect.getPosition() + this->transformToLocalSpace(aprilui::getCursorPosition());
				if (this->pushed)
				{
					if (!this->dragging && (this->_dragSpeed.x != 0.0f || this->_dragSpeed.y != 0.0f ||
						!heqf(this->_clickPosition.x, position.x, this->dragThreshold.x) || !heqf(this->_clickPosition.y, position.y, this->dragThreshold.y)) && this->isScrollable())
					{
						this->dragging = true;
						this->_clickScrollOffset = this->getScrollOffset();
						this->_lastPosition = position;
						foreach (Object*, it, this->childrenObjects)
						{
							(*it)->onMouseCancel(april::Key::None);
						}
					}
					else
					{
						this->_dragSpeed.set(0.0f, 0.0f);
						this->snapScrollOffset();
					}
				}
				if (this->dragging)
				{
					// some devices have a bad digitizer so this serves as special hack to smoothen the dragging movement on the scroll-area
					if (april::window->getInputMode() == april::InputMode::Touch && this->_lastPosition != position)
					{
						position = this->_lastPosition + (position - this->_lastPosition) * 0.5f;
					}
					this->setScrollOffset(this->_clickScrollOffset + this->_clickPosition - position);
					this->_dragSpeed = (position - this->_lastPosition) / timeDelta;
					if (this->dragMaxSpeed > 0.0f)
					{
						float length = this->_dragSpeed.length();
						if (length > 0.0f && length > this->dragMaxSpeed)
						{
							this->_dragSpeed *= this->dragMaxSpeed / length;
						}
					}
					this->_lastPosition = position;
					this->_lastScrollOffset = this->getScrollOffset();
					this->_dragTimer.set(0.0f, 0.0f);
					this->_adjustDragSpeed();
				}
			}
		}
		if (!this->dragging && this->inertia > 0.0f && this->isScrolling())
		{
			this->_dragTimer.x += timeDelta;
			this->_dragTimer.y += timeDelta;
			gvec2 inertiaTime(habs(this->_dragSpeed.x) / this->inertia, habs(this->_dragSpeed.y) / this->inertia);
			gvec2 distance;
			if (this->_dragSpeed.x != 0.0f)
			{
				if (this->_dragTimer.x < inertiaTime.x)
				{
					distance.x = this->_dragSpeed.x * this->_dragTimer.x - hsgn(this->_dragSpeed.x) * this->inertia * (this->_dragTimer.x * this->_dragTimer.x * 0.5f);
				}
				else
				{
					this->_lastScrollOffset.x -= hsgn(this->_dragSpeed.x) * (this->inertia * (inertiaTime.x * inertiaTime.x * 0.5f));
					this->_dragSpeed.x = 0.0f;
					this->_dragTimer.x = 0.0f;
				}
			}
			if (this->_dragSpeed.y != 0.0f)
			{
				if (this->_dragTimer.y < inertiaTime.y)
				{
					distance.y = this->_dragSpeed.y * this->_dragTimer.y - hsgn(this->_dragSpeed.y) * this->inertia * (this->_dragTimer.y * this->_dragTimer.y * 0.5f);
				}
				else
				{
					this->_lastScrollOffset.y -= hsgn(this->_dragSpeed.y) * (this->inertia * (inertiaTime.y * inertiaTime.y * 0.5f));
					this->_dragSpeed.y = 0.0f;
					this->_dragTimer.y = 0.0f;
				}
			}
			gvec2 offset = this->getScrollOffset();
			this->setScrollOffset(this->_lastScrollOffset - distance);
			this->snapScrollOffset();
			gvec2 newOffset = this->getScrollOffset();
			if (offset.x == newOffset.x)
			{
				this->_lastScrollOffset.x = offset.x;
				this->_dragSpeed.x = 0.0f;
				this->_dragTimer.x = 0.0f;
			}
			if (offset.y == newOffset.y)
			{
				this->_lastScrollOffset.y = offset.y;
				this->_dragSpeed.y = 0.0f;
				this->_dragTimer.y = 0.0f;
			}
		}
		else
		{
			this->_dragTimer.set(0.0f, 0.0f);
		}
	}

	void ScrollArea::_updateOobChildren()
	{
		if (this->parent != NULL && (this->optimizeOobChildrenVisible || this->optimizeOobChildrenAwake || this->oobChildrenFadeSizeFactor.x > 0.0f || this->oobChildrenFadeSizeFactor.y > 0.0f))
		{
			grect rect(0.0f, 0.0f, this->parent->getSize());
			grect adjustedRect;
			grect boundingRect;
			float alpha = 1.0f;
			float ratio = 1.0f;
			foreach (Object*, it, this->childrenObjects)
			{
				boundingRect = (*it)->getBoundingRect(this);
				if (this->optimizeOobChildrenVisible || this->optimizeOobChildrenAwake)
				{
					if (rect.intersects(boundingRect))
					{
						if (this->optimizeOobChildrenVisible)
						{
							(*it)->setVisible(true);
						}
						if (this->optimizeOobChildrenAwake)
						{
							(*it)->setAwake(true);
						}
					}
					else
					{
						if (this->optimizeOobChildrenVisible)
						{
							(*it)->setVisible(false);
						}
						if (this->optimizeOobChildrenAwake)
						{
							(*it)->setAwake(false);
						}
					}
				}
				if (this->oobChildrenFadeSizeFactor.x > 0.0f || this->oobChildrenFadeSizeFactor.y > 0.0f)
				{
					alpha = 1.0f;
					ratio = 1.0f;
					if (this->oobChildrenFadeSizeFactor.x > 0.0f)
					{
						adjustedRect.set(rect.x + boundingRect.w, rect.y - boundingRect.h, rect.w - boundingRect.w * 2.0f, rect.h + boundingRect.h * 2.0f);
						if (adjustedRect.w > 0.0f)
						{
							ratio = boundingRect.clipped(adjustedRect).getArea() / boundingRect.getArea() - this->oobChildrenFadeOffsetFactor.x;
							alpha *= hclamp(ratio / this->oobChildrenFadeSizeFactor.x, 0.0f, 1.0f);
						}
						else
						{
							alpha = 0.0f;
						}
					}
					if (alpha > 0.0f && this->oobChildrenFadeSizeFactor.y > 0.0f)
					{
						adjustedRect.set(rect.x - boundingRect.w, rect.y + boundingRect.h, rect.w + boundingRect.w * 2.0f, rect.h - boundingRect.h * 2.0f);
						if (adjustedRect.h > 0.0f)
						{
							ratio = boundingRect.clipped(adjustedRect).getArea() / boundingRect.getArea() - this->oobChildrenFadeOffsetFactor.y;
							alpha *= hclamp(ratio / this->oobChildrenFadeSizeFactor.y, 0.0f, 1.0f);
						}
						else
						{
							alpha = 0.0f;
						}
					}
					(*it)->setAlpha((unsigned char)(alpha * 255.0f));
				}
			}
		}
	}

	void ScrollArea::snapScrollOffset()
	{
		gvec2 offset = this->getScrollOffset();
		this->setScrollOffset(hroundf(offset.x), hroundf(offset.y));
	}

	void ScrollArea::snapScrollOffsetX()
	{
		this->setScrollOffsetX(hroundf(this->getScrollOffsetX()));
	}

	void ScrollArea::snapScrollOffsetY()
	{
		this->setScrollOffsetY(hroundf(this->getScrollOffsetY()));
	}

	void ScrollArea::stopScrolling()
	{
		this->dragging = false;
		this->pushed = false;
		this->_dragSpeed.set(0.0f, 0.0f);
		this->_dragTimer.set(0.0f, 0.0f);
		this->snapScrollOffset();
	}

	bool ScrollArea::_isScrollableScrollArea(aprilui::Object* object) const
    {
        ScrollArea* scrollArea = dynamic_cast<ScrollArea*>(object);
		return (scrollArea != NULL && scrollArea->isScrollable());
	}
    
	aprilui::Object* ScrollArea::_findHoverObject()
	{
		if (this->dragging)
		{
			return (this->isCursorInside() ? this : NULL);
		}
		aprilui::Object* child = ButtonBase::_findHoverObject();
		if (this->_overrideHoverMode && child != this && !this->_isScrollableScrollArea(child) && child != NULL)
		{
			aprilui::Object* parent = child->getParent();
			while (parent != NULL)
			{
				if (parent == this)
				{
					return (this->isCursorInside() ? this : NULL);
				}
                if (this->_isScrollableScrollArea(parent) && parent->getChildUnderCursor() == child)
				{
					return child;
				}
				parent = parent->getParent();
			}
		}
		return child;
	}

	void ScrollArea::notifyEvent(chstr type, EventArgs* args)
	{
		Object::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->scrollArea = this;
			}
		}
		else if (type == Event::DetachedFromObject)
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				parent->scrollArea = NULL;
			}
		}
		else if (type == Event::Resized)
		{
			this->snapScrollOffset();
			this->_updateOobChildren();
		}
	}

	bool ScrollArea::triggerEvent(chstr type, april::Key keyCode)
	{
		return Object::triggerEvent(type, keyCode);
	}

	bool ScrollArea::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Object::triggerEvent(type, keyCode, string);
	}

	bool ScrollArea::triggerEvent(chstr type, april::Key keyCode, cgvec2 position, chstr string, void* userData)
	{
		return Object::triggerEvent(type, keyCode, position, string, userData);
	}

	bool ScrollArea::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return Object::triggerEvent(type, buttonCode, string, userData);
	}

	bool ScrollArea::triggerEvent(chstr type, chstr string, void* userData)
	{
		return Object::triggerEvent(type, string, userData);
	}

	bool ScrollArea::triggerEvent(chstr type, void* userData)
	{
		return Object::triggerEvent(type, userData);
	}

	bool ScrollArea::onMouseDown(april::Key keyCode)
	{
		// has to override its children which is why onMouseDown() is overriden and not _mouseDown()
		if (this->allowDrag && this->hitTest != HitTest::DisabledRecursive && this->isVisible() && this->isDerivedEnabled())
		{
			this->_overrideHoverMode = true;
			bool result = ButtonBase::_mouseDown(keyCode);
			this->_overrideHoverMode = false;
			if (result)
			{
				this->_clickPosition = this->rect.getPosition() + this->transformToLocalSpace(aprilui::getCursorPosition());
			}
		}
		return Object::onMouseDown(keyCode);
	}

	bool ScrollArea::onMouseUp(april::Key keyCode)
	{
		// has to override its children which is why onMouseUp() is overriden and not _mouseUp()
		if (this->allowDrag && this->hitTest != HitTest::DisabledRecursive && this->isVisible() && this->isDerivedEnabled())
		{
			this->dragging = false;
			this->_adjustDragSpeed();
			if (ButtonBase::_mouseUp(keyCode))
			{
				return true;
			}
		}
		return Object::onMouseUp(keyCode);
	}
	
	bool ScrollArea::onMouseScroll(float x, float y)
	{
		// has to override its children which is why onMouseScroll() is overriden and not _mouseScroll()
		if (this->hitTest != HitTest::DisabledRecursive && this->isVisible() && this->isDerivedEnabled())
		{
			Container* parent = dynamic_cast<Container*>(this->parent);
			if (parent != NULL)
			{
				if (this->swapScrollWheels)
				{
					hswap(x, y);
				}
				if (this->_executeScroll(x, y, parent))
				{
					return true;
				}
			}
			else
			{
				hlog::errorf(logTag, "ScrollArea '%s' attached to object '%s' which is not a Container!", this->name.cStr(), this->parent->getName().cStr());
			}
		}
		return Object::onMouseScroll(x, y);
	}

	void ScrollArea::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		Object::_mouseCancel(keyCode);
	}

	bool ScrollArea::_mouseMove()
	{
		if (this->allowDrag)
		{
			return ButtonBase::_mouseMove();
		}
		return Object::_mouseMove();
	}

	bool ScrollArea::_buttonDown(april::Button buttonCode)
	{
		return (ButtonBase::_buttonDown(buttonCode) || Object::_buttonDown(buttonCode));
	}

	bool ScrollArea::_buttonUp(april::Button buttonCode)
	{
		return (ButtonBase::_buttonUp(buttonCode) || Object::_buttonUp(buttonCode));
	}
	
	bool ScrollArea::_executeScroll(float x, float y, Container* parentContainer)
	{
		this->_overrideHoverMode = true;
		bool result = (this->_findHoverObject() == this);
		this->_overrideHoverMode = false;
		if (result)
		{
			result = false;
			if (parentContainer->scrollBarV != NULL)
			{
				parentContainer->scrollBarV->addScrollValue(parentContainer->scrollBarV->_calcScrollMove(x, y));
				result = true;
			}
			if (parentContainer->scrollBarH != NULL)
			{
				parentContainer->scrollBarH->addScrollValue(parentContainer->scrollBarH->_calcScrollMove(x, y));
				result = true;
			}
		}
		return result;
	}

	void ScrollArea::_adjustDragSpeed()
	{
		this->_lastScrollOffset = this->getScrollOffset();
		this->_dragTimer.set(0.0f, 0.0f);
		Container* parent = dynamic_cast<Container*>(this->parent);
		if (parent == NULL)
		{
			return;
		}
		if (parent->scrollBarH != NULL)
		{
			parent->scrollBarH->_adjustDragSpeed();
		}
		if (parent->scrollBarV != NULL)
		{
			parent->scrollBarV->_adjustDragSpeed();
		}
	}
	
}
