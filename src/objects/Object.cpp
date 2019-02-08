/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/april.h>
#include <april/RenderSystem.h>
#include <april/Window.h>
#include <gtypes/Rectangle.h>
#include <gtypes/gtypesUtil.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Animators.h"
#include "aprilui.h"
#include "apriluiUtil.h"
#include "BaseImage.h"
#include "CallbackEvent.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "Exception.h"
#include "ObjectCallbackObject.h"

#define REMOVE_EXISTING_ANIMATORS(name) \
	int i ## name = 0; \
	Animators::name* removeAnimator ## name; \
	while (i ## name < this->dynamicAnimators.size()) \
	{ \
		removeAnimator ## name = dynamic_cast<Animators::name*>(this->dynamicAnimators[i ## name]); \
		if (removeAnimator ## name != NULL) \
		{ \
			delete removeAnimator ## name; \
			this->dynamicAnimators.removeAt(i ## name); \
		} \
		else \
		{ \
			++(i ## name); \
		} \
	}

#define CREATE_DELAYED_DYNAMIC_ANIMATOR(type, offset, target, speed, delay) \
	Animator* animator ## type = new Animators::type(april::generateName("dynamic_animator_")); \
	this->dynamicAnimators += animator ## type; \
	animator ## type->parent = this; \
	animator ## type->setSpeed(speed); \
	animator ## type->setPeriods(1.0f); \
	if (delay == 0.0f) \
	{ \
		animator ## type->setOffset(offset); \
		animator ## type->setAmplitude(target - offset); \
	} \
	else \
	{ \
		animator ## type->setTarget(target); \
		animator ## type->setUseTarget(true); \
		animator ## type->setInheritValue(true); \
		animator ## type->setDelay(delay); \
	}

#define CREATE_DYNAMIC_ANIMATOR(type, offset, target, speed) \
	CREATE_DELAYED_DYNAMIC_ANIMATOR(type, offset, target, speed, 0.0f);

#define CREATE_DYNAMIC_ANIMATE(type) \
	Animator* animator ## type = new Animators::type(april::generateName("dynamic_animator_")); \
	this->dynamicAnimators += animator ## type; \
	animator ## type->parent = this; \
	animator ## type->setOffset(offset); \
	animator ## type->setAmplitude(amplitude); \
	animator ## type->setAnimationFunction(function); \
	if (durationPeriods >= 0.0f) \
	{ \
		animator ## type->setSpeed(speed * durationPeriods); \
		animator ## type->setPeriods(startPeriods + durationPeriods); \
		animator ## type->setPeriodsTimer(startPeriods); \
	} \
	else \
	{ \
		animator ## type->setSpeed(speed); \
		animator ## type->setPeriods(-1.0f); \
		animator ## type->setPeriodsTimer(startPeriods); \
	} \
	animator ## type->setDelay(delay); \

#define DEFINE_DYNAMIC_ANIMATE(functionName, type) \
	Animator* Object::functionName(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay) \
	{ \
		CREATE_DYNAMIC_ANIMATE(type); \
		return animator ## type; \
	}

namespace aprilui
{
	HL_ENUM_CLASS_DEFINE(Object::HitTest,
	(
		HL_ENUM_DEFINE(Object::HitTest, Enabled);
		HL_ENUM_DEFINE(Object::HitTest, Disabled);
		HL_ENUM_DEFINE(Object::HitTest, DisabledRecursive);
	));

	hmap<hstr, PropertyDescription> Object::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> Object::_getters;
	hmap<hstr, PropertyDescription::Accessor*> Object::_setters;

	Object::Object(chstr name) :
		BaseObject(name)
	{
		this->color = april::Color::White;
		this->visible = true;
		this->scaleFactor.set(1.0f, 1.0f);
		this->angle = 0.0f;
		this->anchorLeft = true;
		this->anchorRight = false;
		this->anchorTop = true;
		this->anchorBottom = false;
		this->retainAnchorAspect = false;
		this->hitTest = HitTest::Enabled;
		this->clip = false;
		this->useClipRound = true;
		this->inheritAlpha = true;
		this->useDisabledAlpha = true;
		this->focusIndex = -1;
		this->customPointInsideCallback = NULL;
		this->debugColor.set(april::Color::Black, 32);
		this->_initialPivotX = true;
		this->_initialPivotY = true;
		this->_childUnderCursor = NULL;
		this->_checkedChildUnderCursor = false;
	}

	Object::Object(const Object& other) :
		BaseObject(other)
	{
		this->rect = other.rect;
		this->maxSize = other.maxSize;
		this->unlimitedSize = other.unlimitedSize;
		this->pivot = other.pivot;
		this->color = other.color;
		this->visible = other.visible;
		this->scaleFactor = other.scaleFactor;
		this->angle = other.angle;
		this->anchorLeft = other.anchorLeft;
		this->anchorRight = other.anchorRight;
		this->anchorTop = other.anchorTop;
		this->anchorBottom = other.anchorBottom;
		this->retainAnchorAspect = other.retainAnchorAspect;
		this->hitTest = other.hitTest;
		this->clip = other.clip;
		this->useClipRound = other.clip;
		this->inheritAlpha = other.inheritAlpha;
		this->useDisabledAlpha = other.useDisabledAlpha;
		this->focusIndex = other.focusIndex;
		this->customPointInsideCallback = other.customPointInsideCallback;
		this->debugColor = other.debugColor;
		HL_LAMBDA_CLASS(_cloneAnimators, Animator*, ((Animator* const& animator) { return animator->clone(); }));
		this->dynamicAnimators = other.dynamicAnimators.mapped(&_cloneAnimators::lambda);
		foreach (Animator*, it, this->dynamicAnimators)
		{
			(*it)->parent = this;
		}
		this->_initialPivotX = other._initialPivotX;
		this->_initialPivotY = other._initialPivotY;
		this->_childUnderCursor = NULL;
		this->_checkedChildUnderCursor = false;
	}

	Object::~Object()
	{
		foreach (Animator*, it, this->dynamicAnimators)
		{
			delete (*it);
		}
		if (this->isFocused())
		{
			this->setFocused(false);
		}
	}
	
	// when cloning a tree, cloned children first have to be attached to the clone, before they start creating their own clones
	Object* Object::cloneTree() const
	{
		Object* cloned = this->clone();
		cloned->_cloneChildren(this->childrenObjects, this->childrenAnimators);
		return cloned;
	}

	void Object::_cloneChildren(const harray<Object*>& objects, const harray<Animator*>& animators)
	{
		Object* object = NULL;
		foreachc (Object*, it, objects)
		{
			object = (*it)->clone();
			this->addChild(object);
			object->_cloneChildren((*it)->childrenObjects, (*it)->childrenAnimators);
		}
		foreachc (Animator*, it, animators)
		{
			this->addChild((*it)->clone());
		}
	}

	hmap<hstr, PropertyDescription>& Object::getPropertyDescriptions() const
	{
		if (Object::_propertyDescriptions.size() == 0)
		{
			Object::_propertyDescriptions = BaseObject::getPropertyDescriptions();
			Object::_propertyDescriptions["rect"] = PropertyDescription("rect", PropertyDescription::Type::Grectf);
			Object::_propertyDescriptions["position"] = PropertyDescription("position", PropertyDescription::Type::Gvec2f);
			Object::_propertyDescriptions["size"] = PropertyDescription("size", PropertyDescription::Type::Gvec2f);
			Object::_propertyDescriptions["x"] = PropertyDescription("x", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["y"] = PropertyDescription("y", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["w"] = PropertyDescription("w", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["h"] = PropertyDescription("h", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["max_size"] = PropertyDescription("max_size", PropertyDescription::Type::Gvec2f);
			Object::_propertyDescriptions["max_w"] = PropertyDescription("max_w", PropertyDescription::Type::Gvec2f);
			Object::_propertyDescriptions["max_h"] = PropertyDescription("max_h", PropertyDescription::Type::Gvec2f);
			Object::_propertyDescriptions["visible"] = PropertyDescription("visible", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["hit_test"] = PropertyDescription("hit_test", PropertyDescription::Type::Enum);
			Object::_propertyDescriptions["inherit_alpha"] = PropertyDescription("inherit_alpha", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["color"] = PropertyDescription("color", PropertyDescription::Type::Color);
			Object::_propertyDescriptions["red"] = PropertyDescription("red", PropertyDescription::Type::UChar);
			Object::_propertyDescriptions["green"] = PropertyDescription("green", PropertyDescription::Type::UChar);
			Object::_propertyDescriptions["blue"] = PropertyDescription("blue", PropertyDescription::Type::UChar);
			Object::_propertyDescriptions["alpha"] = PropertyDescription("alpha", PropertyDescription::Type::UChar);
			Object::_propertyDescriptions["angle"] = PropertyDescription("angle", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["scale"] = PropertyDescription("scale", PropertyDescription::Type::Gvec2f);
			Object::_propertyDescriptions["scale_x"] = PropertyDescription("scale_x", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["scale_y"] = PropertyDescription("scale_y", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["pivot"] = PropertyDescription("pivot", PropertyDescription::Type::Gvec2f);
			Object::_propertyDescriptions["pivot_x"] = PropertyDescription("pivot_x", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["pivot_y"] = PropertyDescription("pivot_y", PropertyDescription::Type::Float);
			Object::_propertyDescriptions["anchor_left"] = PropertyDescription("anchor_left", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["anchor_right"] = PropertyDescription("anchor_right", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["anchor_top"] = PropertyDescription("anchor_top", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["anchor_bottom"] = PropertyDescription("anchor_bottom", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["anchors"] = PropertyDescription("anchors", PropertyDescription::Type::String);
			Object::_propertyDescriptions["retain_anchor_aspect"] = PropertyDescription("retain_anchor_aspect", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["clip"] = PropertyDescription("clip", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["use_clip_round"] = PropertyDescription("use_clip_round", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["use_disabled_alpha"] = PropertyDescription("use_disabled_alpha", PropertyDescription::Type::Bool);
			Object::_propertyDescriptions["focus_index"] = PropertyDescription("focus_index", PropertyDescription::Type::Int);
		}
		return Object::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Object::_getGetters() const
	{
		if (Object::_getters.size() == 0)
		{
			Object::_getters = BaseObject::_getGetters();
			Object::_getters["rect"] = new PropertyDescription::GetGrectf<Object>(&Object::getRect);
			Object::_getters["position"] = new PropertyDescription::GetGvec2f<Object>(&Object::getPosition);
			Object::_getters["size"] = new PropertyDescription::GetGvec2f<Object>(&Object::getSize);
			Object::_getters["x"] = new PropertyDescription::Get<Object, float>(&Object::getX);
			Object::_getters["y"] = new PropertyDescription::Get<Object, float>(&Object::getY);
			Object::_getters["w"] = new PropertyDescription::Get<Object, float>(&Object::getWidth);
			Object::_getters["h"] = new PropertyDescription::Get<Object, float>(&Object::getHeight);
			Object::_getters["max_size"] = new PropertyDescription::GetGvec2f<Object>(&Object::getMaxSize);
			Object::_getters["max_w"] = new PropertyDescription::Get<Object, float>(&Object::getMaxWidth);
			Object::_getters["max_h"] = new PropertyDescription::Get<Object, float>(&Object::getMaxHeight);
			Object::_getters["visible"] = new PropertyDescription::Get<Object, bool>(&Object::getVisibilityFlag);
			Object::_getters["inherit_alpha"] = new PropertyDescription::Get<Object, bool>(&Object::isInheritAlpha);
			Object::_getters["color"] = new PropertyDescription::GetColor<Object>(&Object::getColor);
			Object::_getters["red"] = new PropertyDescription::GetUChar<Object>(&Object::getRed);
			Object::_getters["green"] = new PropertyDescription::GetUChar<Object>(&Object::getGreen);
			Object::_getters["blue"] = new PropertyDescription::GetUChar<Object>(&Object::getBlue);
			Object::_getters["alpha"] = new PropertyDescription::GetUChar<Object>(&Object::getAlpha);
			Object::_getters["angle"] = new PropertyDescription::Get<Object, float>(&Object::getAngle);
			Object::_getters["scale"] = new PropertyDescription::GetGvec2f<Object>(&Object::getScale);
			Object::_getters["scale_x"] = new PropertyDescription::Get<Object, float>(&Object::getScaleX);
			Object::_getters["scale_y"] = new PropertyDescription::Get<Object, float>(&Object::getScaleY);
			Object::_getters["pivot"] = new PropertyDescription::GetGvec2f<Object>(&Object::getPivot);
			Object::_getters["pivot_x"] = new PropertyDescription::Get<Object, float>(&Object::getPivotX);
			Object::_getters["pivot_y"] = new PropertyDescription::Get<Object, float>(&Object::getPivotY);
			Object::_getters["anchor_left"] = new PropertyDescription::Get<Object, bool>(&Object::isAnchorLeft);
			Object::_getters["anchor_right"] = new PropertyDescription::Get<Object, bool>(&Object::isAnchorRight);
			Object::_getters["anchor_top"] = new PropertyDescription::Get<Object, bool>(&Object::isAnchorTop);
			Object::_getters["anchor_bottom"] = new PropertyDescription::Get<Object, bool>(&Object::isAnchorBottom);
			Object::_getters["retain_anchor_aspect"] = new PropertyDescription::Get<Object, bool>(&Object::isRetainAnchorAspect);
			Object::_getters["clip"] = new PropertyDescription::Get<Object, bool>(&Object::isClip);
			Object::_getters["use_clip_round"] = new PropertyDescription::Get<Object, bool>(&Object::isUseClipRound);
			Object::_getters["use_disabled_alpha"] = new PropertyDescription::Get<Object, bool>(&Object::isUseDisabledAlpha);
			Object::_getters["focus_index"] = new PropertyDescription::Get<Object, int>(&Object::getFocusIndex);
		}
		return Object::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Object::_getSetters() const
	{
		if (Object::_setters.size() == 0)
		{
			Object::_setters = BaseObject::_getSetters();
			Object::_setters["rect"] = new PropertyDescription::SetGrectf<Object>(&Object::setRect);
			Object::_setters["position"] = new PropertyDescription::SetGvec2f<Object>(&Object::setPosition);
			Object::_setters["size"] = new PropertyDescription::SetGvec2f<Object>(&Object::setSize);
			Object::_setters["x"] = new PropertyDescription::Set<Object, float>(&Object::setX);
			Object::_setters["y"] = new PropertyDescription::Set<Object, float>(&Object::setY);
			Object::_setters["w"] = new PropertyDescription::Set<Object, float>(&Object::setWidth);
			Object::_setters["h"] = new PropertyDescription::Set<Object, float>(&Object::setHeight);
			Object::_setters["max_size"] = new PropertyDescription::SetGvec2f<Object>(&Object::setMaxSize);
			Object::_setters["max_w"] = new PropertyDescription::Set<Object, float>(&Object::setMaxWidth);
			Object::_setters["max_h"] = new PropertyDescription::Set<Object, float>(&Object::setMaxHeight);
			Object::_setters["visible"] = new PropertyDescription::Set<Object, bool>(&Object::setVisible);
			Object::_setters["inherit_alpha"] = new PropertyDescription::Set<Object, bool>(&Object::setInheritAlpha);
			Object::_setters["color"] = new PropertyDescription::Set<Object, hstr>(&Object::setSymbolicColor);
			Object::_setters["red"] = new PropertyDescription::SetUChar<Object>(&Object::setRed);
			Object::_setters["green"] = new PropertyDescription::SetUChar<Object>(&Object::setGreen);
			Object::_setters["blue"] = new PropertyDescription::SetUChar<Object>(&Object::setBlue);
			Object::_setters["alpha"] = new PropertyDescription::SetUChar<Object>(&Object::setAlpha);
			Object::_setters["angle"] = new PropertyDescription::Set<Object, float>(&Object::setAngle);
			Object::_setters["scale"] = new PropertyDescription::SetGvec2f<Object>(&Object::setScale);
			Object::_setters["scale_x"] = new PropertyDescription::Set<Object, float>(&Object::setScaleX);
			Object::_setters["scale_y"] = new PropertyDescription::Set<Object, float>(&Object::setScaleY);
			Object::_setters["pivot"] = new PropertyDescription::SetGvec2f<Object>(&Object::setPivot);
			Object::_setters["pivot_x"] = new PropertyDescription::Set<Object, float>(&Object::setPivotX);
			Object::_setters["pivot_y"] = new PropertyDescription::Set<Object, float>(&Object::setPivotY);
			Object::_setters["anchor_left"] = new PropertyDescription::Set<Object, bool>(&Object::setAnchorLeft);
			Object::_setters["anchor_right"] = new PropertyDescription::Set<Object, bool>(&Object::setAnchorRight);
			Object::_setters["anchor_top"] = new PropertyDescription::Set<Object, bool>(&Object::setAnchorTop);
			Object::_setters["anchor_bottom"] = new PropertyDescription::Set<Object, bool>(&Object::setAnchorBottom);
			Object::_setters["retain_anchor_aspect"] = new PropertyDescription::Set<Object, bool>(&Object::setRetainAnchorAspect);
			Object::_setters["clip"] = new PropertyDescription::Set<Object, bool>(&Object::setClip);
			Object::_setters["use_clip_round"] = new PropertyDescription::Set<Object, bool>(&Object::setUseClipRound);
			Object::_setters["use_disabled_alpha"] = new PropertyDescription::Set<Object, bool>(&Object::setUseDisabledAlpha);
			Object::_setters["focus_index"] = new PropertyDescription::Set<Object, int>(&Object::setFocusIndex);
		}
		return Object::_setters;
	}

	void Object::setRect(cgrectf value)
	{
		this->unlimitedSize = value.getSize();
		gvec2f correctedSize = this->_makeCorrectedSize(this->unlimitedSize);
		this->_updateChildrenHorizontal(correctedSize.x - this->rect.w);
		this->_updateChildrenVertical(correctedSize.y - this->rect.h);
		this->rect.set(value.getPosition(), correctedSize);
		this->notifyEvent(Event::PositionChanged, NULL);
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setRect(cgvec2f position, cgvec2f size)
	{
		this->unlimitedSize = size;
		gvec2f correctedSize = this->_makeCorrectedSize(this->unlimitedSize);
		this->_updateChildrenHorizontal(correctedSize.x - this->rect.w);
		this->_updateChildrenVertical(correctedSize.y - this->rect.h);
		this->rect.set(position, correctedSize);
		this->notifyEvent(Event::PositionChanged, NULL);
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setRect(cgvec2f position, const float& w, const float& h)
	{
		this->unlimitedSize.set(w, h);
		gvec2f correctedSize = this->_makeCorrectedSize(this->unlimitedSize);
		this->_updateChildrenHorizontal(correctedSize.x - this->rect.w);
		this->_updateChildrenVertical(correctedSize.y - this->rect.h);
		this->rect.set(position, correctedSize);
		this->notifyEvent(Event::PositionChanged, NULL);
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setRect(const float& x, const float& y, cgvec2f size)
	{
		this->unlimitedSize = size;
		gvec2f correctedSize = this->_makeCorrectedSize(this->unlimitedSize);
		this->_updateChildrenHorizontal(correctedSize.x - this->rect.w);
		this->_updateChildrenVertical(correctedSize.y - this->rect.h);
		this->rect.set(x, y, correctedSize);
		this->notifyEvent(Event::PositionChanged, NULL);
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setRect(const float& x, const float& y, const float& w, const float& h)
	{
		this->unlimitedSize.set(w, h);
		gvec2f correctedSize = this->_makeCorrectedSize(this->unlimitedSize);
		this->_updateChildrenHorizontal(correctedSize.x - this->rect.w);
		this->_updateChildrenVertical(correctedSize.y - this->rect.h);
		this->rect.set(x, y, correctedSize);
		this->notifyEvent(Event::PositionChanged, NULL);
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setX(const float& value)
	{
		this->rect.x = value;
		this->notifyEvent(Event::PositionChanged, NULL);
	}

	void Object::setY(const float& value)
	{
		this->rect.y = value;
		this->notifyEvent(Event::PositionChanged, NULL);
	}

	void Object::setWidth(const float& value)
	{
		this->unlimitedSize.x = value;
		float correctedValue = this->_makeCorrectedWidth(this->unlimitedSize.x);
		this->_updateChildrenHorizontal(correctedValue - this->rect.w);
		this->rect.w = correctedValue;
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setHeight(const float& value)
	{
		this->unlimitedSize.y = value;
		float correctedValue = this->_makeCorrectedHeight(this->unlimitedSize.y);
		this->_updateChildrenVertical(correctedValue - this->rect.h);
		this->rect.h = correctedValue;
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setPosition(cgvec2f value)
	{
		this->rect.setPosition(value);
		this->notifyEvent(Event::PositionChanged, NULL);
	}

	void Object::setPosition(const float& x, const float& y)
	{
		this->rect.setPosition(x, y);
		this->notifyEvent(Event::PositionChanged, NULL);
	}

	void Object::setSize(cgvec2f value)
	{
		this->unlimitedSize = value;
		gvec2f correctedSize = this->_makeCorrectedSize(this->unlimitedSize);
		this->_updateChildrenHorizontal(correctedSize.x - this->rect.w);
		this->_updateChildrenVertical(correctedSize.y - this->rect.h);
		this->rect.setSize(correctedSize);
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setSize(const float& w, const float& h)
	{
		this->unlimitedSize.set(w, h);
		gvec2f correctedSize = this->_makeCorrectedSize(this->unlimitedSize);
		this->_updateChildrenHorizontal(correctedSize.x - this->rect.w);
		this->_updateChildrenVertical(correctedSize.y - this->rect.h);
		this->rect.setSize(correctedSize);
		this->notifyEvent(Event::SizeChanged, NULL);
	}

	void Object::setMaxSize(cgvec2f value)
	{
		this->maxSize = value;
		if (this->retainAnchorAspect && (this->maxSize.x > 0.0f || this->maxSize.x > 0.0f))
		{
			hlog::warn(logTag, "Using any 'max_size' together with 'retain_anchor_aspect' cannot work properly due to them modifying the same values!");
		}
		gvec2f size = this->rect.getSize();
		gvec2f correctedSize = this->_makeCorrectedSize(this->unlimitedSize);
		if (size != correctedSize)
		{
			this->_updateChildrenHorizontal(correctedSize.x - this->rect.w);
			this->_updateChildrenVertical(correctedSize.y - this->rect.h);
			this->rect.setSize(correctedSize);
			this->notifyEvent(Event::SizeChanged, NULL);
		}
	}

	void Object::setMaxSize(const float& w, const float& h)
	{
		this->setMaxSize(gvec2f(w, h));
	}

	void Object::setMaxWidth(const float& value)
	{
		this->setMaxSize(gvec2f(value, this->maxSize.y));
	}

	void Object::setMaxHeight(const float& value)
	{
		this->setMaxSize(gvec2f(this->maxSize.x, value));
	}

	void Object::setSymbolicColor(chstr value)
	{
		this->setColor(aprilui::_makeColor(value));
	}

	void Object::setRetainAnchorAspect(const bool& value)
	{
		this->retainAnchorAspect = value;
		if (this->retainAnchorAspect && (this->maxSize.x > 0.0f || this->maxSize.x > 0.0f))
		{
			hlog::warn(logTag, "Using any 'max_size' together with 'retain_anchor_aspect' cannot work properly due to them modifying the same values!");
		}
	}

	bool Object::isDerivedVisible() const
	{
		return (this->isVisible() && (this->parent == NULL || this->parent->isDerivedVisible()));
	}

	bool Object::_isDerivedHitTestEnabled() const
	{
		if (this->hitTest != HitTest::Enabled)
		{
			return false;
		}
		Object* ancestor = this->parent;
		while (ancestor != NULL)
		{
			if (ancestor->hitTest == HitTest::DisabledRecursive)
			{
				return false;
			}
			ancestor = ancestor->getParent();
		}
		return true;
	}

	bool Object::isAnimated() const
	{
		HL_LAMBDA_CLASS(_isAnimated, bool, ((Animator* const& animator) { return animator->isAnimated(); }));
		return (this->dynamicAnimators + this->childrenAnimators).matchesAny(&_isAnimated::lambda);
	}

	bool Object::isWaitingAnimation() const
	{
		HL_LAMBDA_CLASS(_isWaitingAnimation, bool, ((Animator* const& animator) { return animator->isWaitingAnimation(); }));
		return (this->dynamicAnimators + this->childrenAnimators).matchesAny(&_isWaitingAnimation::lambda);
	}

	bool Object::hasDynamicAnimation() const
	{
		return (this->dynamicAnimators.size() > 0);
	}

	bool Object::isFocused() const
	{
		return (this->dataset != NULL && this->dataset->getFocusedObject() == this);
	}

	void Object::setFocused(bool value)
	{
		if (this->dataset != NULL)
		{
			if (value)
			{
				this->dataset->focus(this);
			}
			else if (this->dataset->getFocusedObject() == this)
			{
				this->dataset->removeFocus();
			}
		}
	}

	bool Object::isCursorInside() const
	{
		return this->isPointInside(aprilui::getCursorPosition());
	}

	harray<BaseImage*> Object::getUsedImages() const
	{
		harray<BaseImage*> images = this->_getUsedImages();
		images.removeAll(NULL);
		images.removeDuplicates();
		return images;
	}

	harray<BaseImage*> Object::_getUsedImages() const
	{
		return harray<BaseImage*>();
	}

	Object* Object::getChildUnderCursor()
	{
		if (!this->_checkedChildUnderCursor)
		{
			this->_childUnderCursor = this->getChildUnderPoint(aprilui::getCursorPosition());
			this->_checkedChildUnderCursor = true;
		}
		return this->_childUnderCursor;
	}

	void Object::update(float timeDelta)
	{
		if (this->_checkedChildUnderCursor)
		{
			this->clearChildUnderCursor();
		}
		BaseObject::update(timeDelta);
	}
	
	void Object::_update(float timeDelta)
	{
		BaseObject::_update(timeDelta);
		if (this->dynamicAnimators.size() > 0)
		{
			// because this list could change during the update() call
			this->_dynamicAnimators = this->dynamicAnimators;
			// first update the animators
			foreach (Animator*, it, this->_dynamicAnimators)
			{
				(*it)->update(timeDelta);
			}
			if (this->dynamicAnimators.size() > 0)
			{
				this->_dynamicAnimators = this->dynamicAnimators;
				this->dynamicAnimators.clear();
				foreach (Animator*, it, this->_dynamicAnimators)
				{
					if ((*it)->isExpired())
					{
						delete (*it);
					}
					else
					{
						this->dynamicAnimators += (*it);
					}
				}
			}
		}
	}

	void Object::draw()
	{
		if (!this->isVisible() || heqf(this->scaleFactor.x, 0.0f, 0.0001f) || heqf(this->scaleFactor.y, 0.0f, 0.0001f))
		{
			return;
		}
		this->_drawModelviewMatrix = april::rendersys->getModelviewMatrix();
		this->_drawClipped = (this->clip && this->parent != NULL);
		if (this->_drawClipped)
		{
			this->_drawProjectionMatrix = april::rendersys->getProjectionMatrix();
			this->_drawViewport = april::rendersys->getViewport();
			this->_drawViewportOffset = aprilui::getViewport().getPosition();
			this->_drawOriginalRect = this->parent->getBoundingRect() + this->_drawViewportOffset;
			this->_drawRatio = gvec2f(this->_drawViewport.getSize()) / april::rendersys->getOrthoProjection().getSize();
			this->_drawNewViewport.set((this->_drawOriginalRect.getPosition()) * this->_drawRatio, this->_drawOriginalRect.getSize() * this->_drawRatio);
			this->_drawNewViewport.clip(this->_drawViewport);
			if (!this->useClipRound)
			{
				if (this->_drawNewViewport.w <= 0.0f || this->_drawNewViewport.h <= 0.0f)
				{
					return;
				}
			}
			else if (this->_drawNewViewport.w < 0.5f || this->_drawNewViewport.h < 0.5f)
			{
				return;
			}
			this->_drawNewRect.set(this->_drawNewViewport.getPosition() / this->_drawRatio, this->_drawNewViewport.getSize() / this->_drawRatio);
			this->_drawOriginalRect.clip(this->_drawNewRect);
			this->_drawModelviewMatrixRect.set(this->_drawViewportOffset - this->_drawOriginalRect.getPosition(), this->_drawOriginalRect.getSize());
			april::rendersys->setOrthoProjection(this->_drawModelviewMatrixRect);
			if (!this->useClipRound)
			{
				april::rendersys->setViewport(this->_drawNewViewport);
			}
			else
			{
				this->_drawNewViewportRect.set(hround(this->_drawNewViewport.x), hround(this->_drawNewViewport.y), hround(this->_drawNewViewport.w), hround(this->_drawNewViewport.h));
				april::rendersys->setViewport(this->_drawNewViewportRect);
			}
		}
		this->_drawPosition = this->rect.getPosition() + this->pivot;
		if (this->_drawPosition.x != 0.0f || this->_drawPosition.y != 0.0f)
		{
			april::rendersys->translate(this->_drawPosition.x, this->_drawPosition.y);
		}
		if (this->angle != 0.0f)
		{
			april::rendersys->rotate(this->angle);
		}
		if (this->scaleFactor.x != 1.0f || this->scaleFactor.y != 1.0f)
		{
			april::rendersys->scale(this->scaleFactor.x, this->scaleFactor.y, 1.0f);
		}
		this->_draw();
		if (aprilui::isDebugEnabled())
		{
			this->_drawDebug();
		}
		if (this->pivot.x != 0.0f || this->pivot.y != 0.0f)
		{
			april::rendersys->translate(-this->pivot.x, -this->pivot.y);
		}
		foreach (Object*, it, this->childrenObjects)
		{
			(*it)->draw();
		}
		if (this->_drawClipped)
		{
			april::rendersys->setProjectionMatrix(this->_drawProjectionMatrix);
			april::rendersys->setViewport(this->_drawViewport);
		}
		april::rendersys->setModelviewMatrix(this->_drawModelviewMatrix);
	}

	void Object::_draw()
	{
	}

	void Object::_drawDebug()
	{
		this->_drawRect = this->_makeDrawRect();
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply);
		if (this->debugColor.a > 0)
		{
			april::rendersys->drawFilledRect(this->_drawRect, this->debugColor);
		}
		this->_drawFrameColor = april::Color::Green;
		if (this->hitTest == HitTest::Enabled)
		{
			this->_drawFrameColor = april::Color::Yellow;
		}
		else if (this->hitTest == HitTest::DisabledRecursive)
		{
			this->_drawFrameColor = april::Color::Red;
		}
		else if (!this->_isDerivedHitTestEnabled())
		{
			this->_drawFrameColor = april::Color::Cyan;
		}
		april::rendersys->drawRect(this->_drawRect, april::Color(this->_drawFrameColor, 224));
		static grectf whiteRect(-1.0f, -1.0f, 2.0f, 2.0f);
		static grectf greenRect(-3.0f, -3.0f, 6.0f, 6.0f);
		april::rendersys->drawRect(whiteRect, april::Color::White);
		april::rendersys->drawRect(greenRect, april::Color::Green);
	}

	hstr Object::getProperty(chstr name)
	{
		if (name == "hit_test")
		{
			if (this->hitTest == HitTest::Enabled)				return "enabled";
			if (this->hitTest == HitTest::Disabled)				return "disabled";
			if (this->hitTest == HitTest::DisabledRecursive)	return "disabled_recursive";
		}
		if (name == "anchors")
		{
			harray<hstr> result;
			if (this->anchorLeft)	result += "left";
			if (this->anchorRight)	result += "right";
			if (this->anchorTop)	result += "top";
			if (this->anchorBottom)	result += "bottom";
			return result.joined(',');
		}
		return BaseObject::getProperty(name);
	}

	bool Object::setProperty(chstr name, chstr value)
	{
		if (name == "hit_test")
		{
			if (value == "enabled")					this->setHitTest(HitTest::Enabled);
			else if (value == "disabled")			this->setHitTest(HitTest::Disabled);
			else if (value == "disabled_recursive")	this->setHitTest(HitTest::DisabledRecursive);
			else
			{
				hlog::warn(logTag, "'hit_test=' does not support value '" + value + "'.");
				return false;
			}
			return true;
		}
		if (name == "anchors")
		{
			harray<hstr> anchors = value.replaced(" ", "").lowered().split(",", -1, true);
			harray<hstr> testAnchors = anchors;
			testAnchors.removeAll("left");
			testAnchors.removeAll("right");
			testAnchors.removeAll("top");
			testAnchors.removeAll("bottom");
			testAnchors.removeAll("none");
			testAnchors.removeAll("all");
			if (testAnchors.size() > 0)
			{
				hlog::warn(logTag, "Found invalid values in 'anchors=' ('" + testAnchors.joined(",") + "').");
				return false;
			}
			this->setAnchorLeft(anchors.has("all") || anchors.has("left"));
			this->setAnchorRight(anchors.has("all") || anchors.has("right"));
			this->setAnchorTop(anchors.has("all") || anchors.has("top"));
			this->setAnchorBottom(anchors.has("all") || anchors.has("bottom"));
			return true;
		}
		return BaseObject::setProperty(name, value);
	}

	void Object::setAnchors(bool left, bool right, bool top, bool bottom)
	{
		this->anchorLeft = left;
		this->anchorRight = right;
		this->anchorTop = top;
		this->anchorBottom = bottom;
	}

	void Object::resetPivot()
	{
		this->pivot = this->rect.getSize() / 2;
	}

	harray<gvec2f> Object::transformToLocalSpace(const harray<gvec2f>& points, aprilui::Object* overrideRoot) const
	{
		harray<const Object*> sequence;
		const Object* current = this;
		while (current != NULL)
		{
			sequence += current;
			current = ((overrideRoot == NULL || overrideRoot != current) ? current->getParent() : NULL);
		}
		sequence.reverse();
		harray<gvec2f> result = points;
		gvec2f pivot;
		gvec2f scale;
		gvec2f position;
		float angle;
		foreach (const Object*, it, sequence)
		{
			pivot = (*it)->getPivot();
			scale = (*it)->getScale();
			position = (*it)->getPosition();
			angle = (*it)->getAngle();
			foreach (gvec2f, it2, result)
			{
				(*it2) -= pivot + position;
				(*it2).rotate(angle);
				(*it2) /= scale;
				(*it2) += pivot;
			}
		}
		return result;
	}
	
	gvec2f Object::transformToLocalSpace(cgvec2f point, aprilui::Object* overrideRoot) const
	{
		harray<const Object*> sequence;
		const Object* current = this;
		while (current != NULL)
		{
			sequence += current;
			current = ((overrideRoot == NULL || overrideRoot != current) ? current->getParent() : NULL);
		}
		sequence.reverse();
		gvec2f result = point;
		gvec2f pivot;
		foreach (const Object*, it, sequence)
		{
			pivot = (*it)->getPivot();
			result -= pivot + (*it)->getPosition();
			result.rotate((*it)->getAngle());
			result /= (*it)->getScale();
			result += pivot;
		}
		return result;
	}

	unsigned char Object::getDerivedAlpha(aprilui::Object* overrideRoot) const
	{
		// recursive function that combines all the alpha from the parents (if any)
		float factor = 1.0f;
		if (overrideRoot == this)
		{
			return this->getAlpha();
		}
		if (this->inheritAlpha && this->parent != NULL && this->parent != overrideRoot)
		{
			factor *= this->parent->getDerivedAlpha(overrideRoot) / 255.0f;
		}
		return (unsigned char)(this->getAlpha() * factor);
	}

	harray<gvec2f> Object::getDerivedPoints(const harray<gvec2f>& points, aprilui::Object* overrideRoot) const
	{
		const Object* current = this;
		harray<gvec2f> result = points;
		gvec2f pivot;
		gvec2f scale;
		gvec2f position;
		float angle;
		while (current != NULL)
		{
			pivot = current->getPivot();
			scale = current->getScale();
			position = current->getPosition();
			angle = current->getAngle();
			foreach (gvec2f, it, result)
			{
				(*it) -= pivot;
				(*it) *= scale;
				(*it).rotate(-angle);
				(*it) += pivot + position;
			}
			current = ((overrideRoot == NULL || overrideRoot != current) ? current->getParent() : NULL);
		}
		return result;
	}

	gvec2f Object::getDerivedPoint(cgvec2f point, aprilui::Object* overrideRoot) const
	{
		const Object* current = this;
		gvec2f result = point;
		gvec2f pivot;
		while (current != NULL)
		{
			pivot = current->getPivot();
			result -= pivot;
			result *= current->getScale();
			result.rotate(-current->getAngle());
			result += pivot + current->getPosition();
			current = ((overrideRoot == NULL || overrideRoot != current) ? current->getParent() : NULL);
		}
		return result;
	}

	grectf Object::getBoundingRect(aprilui::Object* overrideRoot) const
	{
		gvec2f max;
		gvec2f min;
		harray<gvec2f> corners = this->getDerivedCorners(overrideRoot);
		min = max = corners.removeFirst(); // guaranteed to return 4 corner points previously
		gvec2f corner;
		while (corners.size() > 0)
		{
			corner = corners.removeFirst();
			max.x = hmax(max.x, corner.x);
			max.y = hmax(max.y, corner.y);
			min.x = hmin(min.x, corner.x);
			min.y = hmin(min.y, corner.y);
		}
		return grectf(min, max - min);
	}

	harray<gvec2f> Object::getDerivedCorners(aprilui::Object* overrideRoot) const
	{
		harray<gvec2f> points;
		points += gvec2f(0.0f, 0.0f);
		points += gvec2f(0.0f, this->rect.h);
		points += gvec2f(this->rect.w, 0.0f);
		points += this->rect.getSize();
		return this->getDerivedPoints(points, overrideRoot);
	}

	gvec2f Object::getDerivedPosition(aprilui::Object* overrideRoot) const
	{
		return this->getDerivedPoint(gvec2f(0.0f, 0.0f), overrideRoot);
	}

	gvec2f Object::getDerivedSize(aprilui::Object* overrideRoot) const
	{
		return this->getBoundingRect(overrideRoot).getSize();
	}

	gvec2f Object::getDerivedPivot(aprilui::Object* overrideRoot) const
	{
		return this->getDerivedPoint(this->pivot, overrideRoot);
	}

	gvec2f Object::getDerivedScale(aprilui::Object* overrideRoot) const
	{
		if (overrideRoot == this)
		{
			return this->scaleFactor;
		}
		gvec2f scaleFactor = this->scaleFactor;
		if (this->parent != overrideRoot && this->parent != NULL)
		{
			scaleFactor *= this->parent->getDerivedScale(overrideRoot);
		}
		return scaleFactor;
	}

	float Object::getDerivedAngle(aprilui::Object* overrideRoot) const
	{
		if (overrideRoot == this)
		{
			return this->angle;
		}
		float angle = this->angle;
		if (this->parent != overrideRoot && this->parent != NULL)
		{
			angle += this->parent->getDerivedAngle(overrideRoot);
		}
		return angle;
	}

	float Object::_getDerivedAngle(aprilui::Object* overrideRoot) const
	{
		float angle = this->angle;
		if (overrideRoot == this)
		{
			return angle;
		}
		if (this->parent != overrideRoot && this->parent != NULL)
		{
			angle += this->parent->_getDerivedAngle(overrideRoot);
		}
		return angle;
	}

	bool Object::isPointInside(cgvec2f position) const
	{
		if (heqf(this->scaleFactor.x, 0.0f, 0.0001f) || heqf(this->scaleFactor.y, 0.0f, 0.0001f))
		{
			return false;
		}
		if (this->parent != NULL)
		{
			const Object* obj = this;
			const Object* ancestor = this->parent;
			while (ancestor != NULL)
			{
				if (obj->isClip() && !ancestor->getBoundingRect().isPointInside(position))
				{
					return false;
				}
				obj = ancestor;
				ancestor = obj->getParent();
			}
		}
		if (this->customPointInsideCallback != NULL)
		{
			return (*this->customPointInsideCallback)(this, position);
		}
		return grectf(0.0f, 0.0f, this->rect.getSize()).isPointInside(this->transformToLocalSpace(position));
	}

	Object* Object::getChildUnderPoint(cgvec2f point) const
	{
		if (!this->isVisible() || this->hitTest == HitTest::DisabledRecursive)
		{
			return NULL;
		}
		Object* object = NULL;
		foreachc_r (Object*, it, this->childrenObjects)
		{
			object = (*it)->getChildUnderPoint(point);
			if (object != NULL)
			{
				break;
			}
		}
		return (object == NULL && this->hitTest == HitTest::Enabled && this->isPointInside(point) ? (Object*)this : object);
	}

	Object* Object::getChildUnderPoint(float x, float y) const
	{
		return this->getChildUnderPoint(gvec2f(x, y));
	}
	
	void Object::clearChildUnderCursor()
	{
		this->_childUnderCursor = NULL;
		this->_checkedChildUnderCursor = false;
	}

	void Object::clearDescendantChildrenUnderCursor()
	{
		this->clearChildUnderCursor();
		foreach (Object*, it, this->childrenObjects)
		{
			(*it)->clearDescendantChildrenUnderCursor();
		}
	}

	void Object::addChild(BaseObject* obj)
	{
		if (obj == NULL)
		{
			// this exception cannot be disabled on purpose
			throw ObjectNotExistsException("Object", "NULL", this->name);
		}
		if (obj->getParent() != NULL)
		{
			__THROW_EXCEPTION(ObjectHasParentException(obj->getName(), this->getName()), aprilui::childManipulationDebugExceptionsEnabled, return);
		}
		Object* object = dynamic_cast<Object*>(obj);
		if (object != NULL)
		{
			this->childrenObjects += object;
			this->_sortChildren();
		}
		else
		{
			Animator* animator = dynamic_cast<Animator*>(obj);
			if (animator != NULL)
			{
				this->childrenAnimators += animator;
			}
		}
		obj->parent = this;
		obj->notifyEvent(Event::AttachedToObject, NULL);
	}

	void Object::removeChild(BaseObject* obj)
	{
		if (obj == NULL)
		{
			// this exception cannot be disabled on purpose
			throw ObjectNotExistsException("Object", "NULL", this->name);
		}
		if (obj->getParent() != this)
		{
			__THROW_EXCEPTION(ObjectNotChildException(obj->getName(), this->getName()), aprilui::childManipulationDebugExceptionsEnabled, return);
		}
		Object* root = this;
		while (root != NULL)
		{
			root->clearChildUnderCursor();
			root = root->getParent();
		}
		obj->notifyEvent(Event::DetachedFromObject, NULL);
		Object* object = dynamic_cast<Object*>(obj);
		if (object != NULL)
		{
			object->clearDescendantChildrenUnderCursor();
			this->childrenObjects -= object;
		}
		else
		{
			Animator* animator = dynamic_cast<Animator*>(obj);
			if (animator != NULL)
			{
				this->childrenAnimators -= animator;
			}
		}
		obj->parent = NULL;
	}

	void Object::registerChild(BaseObject* object)
	{
		if (this->dataset != NULL)
		{
			this->dataset->registerObjects(object, false);
		}
		this->addChild(object);
	}

	void Object::unregisterChild(BaseObject* object)
	{
		this->removeChild(object);
		if (this->dataset != NULL)
		{
			this->dataset->unregisterObjects(object);
		}
	}

	void Object::removeChildren(bool recursive)
	{
		if (recursive)
		{
			foreach (Object*, it, this->childrenObjects)
			{
				(*it)->removeChildren(recursive);
			}
		}
		foreach (Object*, it, this->childrenObjects)
		{
			(*it)->parent = NULL;
		}
		foreach (Animator*, it, this->childrenAnimators)
		{
			(*it)->parent = NULL;
		}
		this->childrenObjects.clear();
		this->childrenAnimators.clear();
	}

	void Object::destroyChildren()
	{
		while (this->childrenAnimators.size() > 0)
		{
			this->dataset->destroyObjects(this->childrenAnimators.first());
		}
		while (this->childrenObjects.size() > 0)
		{
			this->dataset->destroyObjects(this->childrenObjects.first());
		}
	}

	void Object::_updateChildrenHorizontal(float difference)
	{
		if (heqf(difference, 0.0f))
		{
			return;
		}
		if (this->_initialPivotX)
		{
			this->_initialPivotX = false;
			this->pivot.x = difference * 0.5f;
		}
		gvec2f size;
		gvec2f unlimitedSize;
		float differenceAlt = 0.0f;
		float sizeDifference = 0.0f;
		float width = 0.0f;
		foreach (Object*, it, this->childrenObjects)
		{
			size = (*it)->getSize();
			unlimitedSize = (*it)->getUnlimitedSize();
			if (!(*it)->isAnchorLeft())
			{
				if ((*it)->isAnchorRight())
				{
					(*it)->setX((*it)->getX() + difference);
				}
				else
				{
					(*it)->setX((*it)->getX() + difference * 0.5f);
				}
			}
			else if ((*it)->isAnchorRight())
			{
				(*it)->setWidth(unlimitedSize.x + difference);
				width = (*it)->getWidth(); // required due to max height parameter
				if (size.x != 0.0f)
				{
					(*it)->setPivotX((*it)->getPivotX() * width / size.x);
				}
				sizeDifference = size.x + difference - width;
				if (sizeDifference != 0.0f)
				{
					(*it)->setX((*it)->getX() + sizeDifference * 0.5f);
				}
				if ((*it)->isRetainAnchorAspect())
				{
					differenceAlt = difference * size.y / size.x;
					(*it)->setHeight(unlimitedSize.y + differenceAlt);
					if ((*it)->isAnchorTop() == (*it)->isAnchorBottom())
					{
						(*it)->setY((*it)->getY() - differenceAlt * 0.5f);
					}
					else if ((*it)->isAnchorBottom())
					{
						(*it)->setY((*it)->getY() - differenceAlt);
					}
					if (unlimitedSize.y != 0.0f)
					{
						(*it)->setPivotY((*it)->getPivotY() * (unlimitedSize.y + differenceAlt) / unlimitedSize.y);
					}
				}
			}
		}
	}

	void Object::_updateChildrenVertical(float difference)
	{
		if (heqf(difference, 0.0f))
		{
			return;
		}
		if (this->_initialPivotY)
		{
			this->_initialPivotY = false;
			this->pivot.y = difference * 0.5f;
		}
		gvec2f size;
		gvec2f unlimitedSize;
		float differenceAlt = 0.0f;
		float sizeDifference = 0.0f;
		float height = 0.0f;
		foreach (Object*, it, this->childrenObjects)
		{
			size = (*it)->getSize();
			unlimitedSize = (*it)->getUnlimitedSize();
			if (!(*it)->isAnchorTop())
			{
				if ((*it)->isAnchorBottom())
				{
					(*it)->setY((*it)->getY() + difference);
				}
				else
				{
					(*it)->setY((*it)->getY() + difference * 0.5f);
				}
			}
			else if ((*it)->isAnchorBottom())
			{
				(*it)->setHeight(unlimitedSize.y + difference);
				height = (*it)->getHeight(); // required due to max height parameter
				if (size.y != 0.0f)
				{
					(*it)->setPivotY((*it)->getPivotY() * height / size.y);
				}
				sizeDifference = size.y + difference - height;
				if (sizeDifference != 0.0f)
				{
					(*it)->setY((*it)->getY() + sizeDifference * 0.5f);
				}
				if ((*it)->isRetainAnchorAspect())
				{
					differenceAlt = difference * size.x / size.y;
					(*it)->setWidth(unlimitedSize.x + differenceAlt);
					if ((*it)->isAnchorLeft() == (*it)->isAnchorRight())
					{
						(*it)->setX((*it)->getX() - differenceAlt * 0.5f);
					}
					else if ((*it)->isAnchorRight())
					{
						(*it)->setX((*it)->getX() - differenceAlt);
					}
					if (unlimitedSize.x != 0.0f)
					{
						(*it)->setPivotX((*it)->getPivotX() * (unlimitedSize.x + differenceAlt) / unlimitedSize.x);
					}
				}
			}
		}
	}

	april::Color Object::_makeDrawColor() const
	{
		april::Color color = this->color;
		if (this->inheritAlpha)
		{
			color.a = 255; // required, because this->_makeDrawColor(color) will already apply the derived object alpha
		}
		return this->_makeDrawColor(color);
	}
	
	april::Color Object::_makeDrawColor(const april::Color& color) const
	{
		float alpha = (float)color.a;
		if (this->inheritAlpha)
		{
			alpha *= this->getDerivedAlpha() / 255.0f;
		}
		if (this->useDisabledAlpha && !this->isDerivedEnabled())
		{
			alpha *= 0.5f;
		}
		return april::Color(color, (unsigned char)alpha);
	}
	
	grectf Object::_makeDrawRect() const
	{
		return grectf(-this->pivot, this->rect.getSize());
	}

	gvec2f Object::_makeCorrectedSize(cgvec2f size)
	{
		return gvec2f(this->_makeCorrectedWidth(size.x), this->_makeCorrectedHeight(size.y));
	}

	float Object::_makeCorrectedWidth(const float& width)
	{
		return (this->maxSize.x > 0.0f ? hmin(width, this->maxSize.x) : width);
	}

	float Object::_makeCorrectedHeight(const float& height)
	{
		return (this->maxSize.y > 0.0f ? hmin(height, this->maxSize.y) : height);
	}

	bool Object::onMouseDown(april::Key keyCode)
	{
		if (this->hitTest == HitTest::DisabledRecursive || !this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		if (this->hitTest == HitTest::Enabled && this->dataset != NULL)
		{
			this->dataset->removeFocus();
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onMouseDown(keyCode))
			{
				return true;
			}
		}
		return this->_mouseDown(keyCode);
	}

	bool Object::_mouseDown(april::Key keyCode)
	{
		return false;
	}

	bool Object::onMouseUp(april::Key keyCode)
	{
		if (this->hitTest == HitTest::DisabledRecursive || !this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		harray<Object*> canceledObjects;
		Object* object = NULL;
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		for_iter_r (i, objects.size(), 0)
		{
			if (objects[i]->onMouseUp(keyCode))
			{
				object = objects[i];
				++i;
				if (i < objects.size())
				{
					canceledObjects += objects(i, -1);
				}
				break;
			}
			canceledObjects += objects[i];
		}
		if (object != NULL)
		{
			// does not call onMouseCancel() on self, because it would affect all children, not just a select few
			this->_mouseCancel(keyCode);
			foreach (Object*, it, canceledObjects)
			{
				(*it)->onMouseCancel(keyCode);
			}
			return true;
		}
		return this->_mouseUp(keyCode);
	}

	bool Object::_mouseUp(april::Key keyCode)
	{
		return false;
	}

	bool Object::onMouseCancel(april::Key keyCode)
	{
		harray<Object*> objects = this->childrenObjects;
		this->_mouseCancel(keyCode); // _mouseCancel() is the only one that is first called on this object and then on all children afterwards
		foreach_r (Object*, it, objects)
		{
			(*it)->onMouseCancel(keyCode);
		}
		return false;
	}

	void Object::_mouseCancel(april::Key keyCode)
	{
	}

	bool Object::onMouseMove()
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onMouseMove())
			{
				return true;
			}
		}
		return this->_mouseMove();
	}

	bool Object::_mouseMove()
	{
		return false;
	}

	bool Object::onMouseScroll(float x, float y)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onMouseScroll(x, y))
			{
				return true;
			}
		}
		return this->_mouseScroll(x, y);
	}

	bool Object::_mouseScroll(float x, float y)
	{
		return false;
	}

	bool Object::onKeyDown(april::Key keyCode)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onKeyDown(keyCode))
			{
				return true;
			}
		}
		return this->_keyDown(keyCode);
	}

	bool Object::_keyDown(april::Key keyCode)
	{
		return false;
	}

	bool Object::onKeyUp(april::Key keyCode)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onKeyUp(keyCode))
			{
				return true;
			}
		}
		return this->_keyUp(keyCode);
	}
	
	bool Object::_keyUp(april::Key keyCode)
	{
		return false;
	}
	
	bool Object::onChar(unsigned int charCode)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onChar(charCode))
			{
				return true;
			}
		}
		return this->_char(charCode); // charcoal
	}

	bool Object::_char(unsigned int charCode)
	{
		return false;
	}

	bool Object::onTouch(const harray<gvec2f>& touches)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onTouch(touches))
			{
				return true;
			}
		}
		return this->_touch(touches);
	}

	bool Object::_touch(const harray<gvec2f>& touches)
	{
		return false;
	}

	bool Object::onButtonDown(april::Button buttonCode)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onButtonDown(buttonCode))
			{
				return true;
			}
		}
		return this->_buttonDown(buttonCode);
	}

	bool Object::_buttonDown(april::Button buttonCode)
	{
		return false;
	}

	bool Object::onButtonUp(april::Button buttonCode)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> objects = this->childrenObjects;
		foreach_r (Object*, it, objects)
		{
			if ((*it)->onButtonUp(buttonCode))
			{
				return true;
			}
		}
		return this->_buttonUp(buttonCode);
	}

	bool Object::_buttonUp(april::Button buttonCode)
	{
		return false;
	}

	Animator* Object::moveX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		CREATE_DYNAMIC_ANIMATOR(MoverX, this->rect.x, x, speed);
		return animatorMoverX;
	}

	Animator* Object::moveY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverY, this->rect.y, y, speed);
		return animatorMoverY;
	}

	Animator* Object::scaleX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, this->scaleFactor.x, x, speed);
		return animatorScalerX;
	}

	Animator* Object::scaleY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, this->scaleFactor.y, y, speed);
		return animatorScalerY;
	}

	Animator* Object::resizeX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, this->rect.w, x, speed);
		return animatorResizerX;
	}

	Animator* Object::resizeY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, this->rect.h, y, speed);
		return animatorResizerY;
	}

	Animator* Object::rotate(float angle, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(Rotator);
		CREATE_DYNAMIC_ANIMATOR(Rotator, this->angle, angle, speed);
		return animatorRotator;
	}

	Animator* Object::movePivotX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverX);
		CREATE_DYNAMIC_ANIMATOR(PivotMoverX, this->pivot.x, x, speed);
		return animatorPivotMoverX;
	}

	Animator* Object::movePivotY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverY);
		CREATE_DYNAMIC_ANIMATOR(PivotMoverY, this->pivot.y, y, speed);
		return animatorPivotMoverY;
	}

	Animator* Object::fadeRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)this->color.r, (float)r, speed);
		return animatorRedChanger;
	}

	Animator* Object::fadeGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)this->color.g, (float)g, speed);
		return animatorGreenChanger;
	}

	Animator* Object::fadeBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)this->color.b, (float)b, speed);
		return animatorBlueChanger;
	}

	Animator* Object::fadeAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->color.a, (float)a, speed);
		return animatorAlphaChanger;
	}

	Animator* Object::changeZOrder(int z, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ZOrderChanger);
		CREATE_DYNAMIC_ANIMATOR(ZOrderChanger, (float)this->zOrder, (float)z, speed);
		return animatorZOrderChanger;
	}

	void Object::move(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverX, this->rect.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(MoverY, this->rect.y, y, speed);
	}

	void Object::move(cgvec2f position, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverX, this->rect.x, position.x, speed);
		CREATE_DYNAMIC_ANIMATOR(MoverY, this->rect.y, position.y, speed);
	}

	void Object::scale(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, this->scaleFactor.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, this->scaleFactor.y, y, speed);
	}

	void Object::scale(cgvec2f scale, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, this->scaleFactor.x, scale.x, speed);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, this->scaleFactor.y, scale.y, speed);
	}

	void Object::resize(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, this->rect.w, x, speed);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, this->rect.h, y, speed);
	}

	void Object::resize(cgvec2f size, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, this->rect.w, size.x, speed);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, this->rect.h, size.y, speed);
	}

	void Object::movePivot(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverX);
		REMOVE_EXISTING_ANIMATORS(PivotMoverY);
		CREATE_DYNAMIC_ANIMATOR(PivotMoverX, this->pivot.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(PivotMoverY, this->pivot.y, y, speed);
	}

	void Object::movePivot(cgvec2f pivot, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverX);
		REMOVE_EXISTING_ANIMATORS(PivotMoverY);
		CREATE_DYNAMIC_ANIMATOR(PivotMoverX, this->pivot.x, pivot.x, speed);
		CREATE_DYNAMIC_ANIMATOR(PivotMoverY, this->pivot.y, pivot.y, speed);
	}

	void Object::fadeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)this->color.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)this->color.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)this->color.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->color.a, (float)a, speed);
	}
	
	void Object::fadeColor(const april::Color& color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)this->color.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)this->color.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)this->color.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->color.a, (float)color.a, speed);
	}

	Animator* Object::moveXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, this->rect.x, x, speed, delay);
		return animatorMoverX;
	}

	Animator* Object::moveYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, this->rect.y, y, speed, delay);
		return animatorMoverY;
	}

	Animator* Object::scaleXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, this->scaleFactor.x, x, speed, delay);
		return animatorScalerX;
	}

	Animator* Object::scaleYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, this->scaleFactor.y, y, speed, delay);
		return animatorScalerY;
	}

	Animator* Object::resizeXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, this->rect.w, x, speed, delay);
		return animatorResizerX;
	}

	Animator* Object::resizeYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, this->rect.h, y, speed, delay);
		return animatorResizerY;
	}

	Animator* Object::rotateQueue(float angle, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(Rotator, this->angle, angle, speed, delay);
		return animatorRotator;
	}

	Animator* Object::movePivotXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(PivotMoverX, this->pivot.x, x, speed, delay);
		return animatorPivotMoverX;
	}

	Animator* Object::movePivotYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(PivotMoverY, this->pivot.y, y, speed, delay);
		return animatorPivotMoverY;
	}

	Animator* Object::fadeRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)this->color.r, (float)r, speed, delay);
		return animatorRedChanger;
	}

	Animator* Object::fadeGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)this->color.g, (float)g, speed, delay);
		return animatorGreenChanger;
	}

	Animator* Object::fadeBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)this->color.b, (float)b, speed, delay);
		return animatorBlueChanger;
	}

	Animator* Object::fadeAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->color.a, (float)a, speed, delay);
		return animatorAlphaChanger;
	}

	Animator* Object::changeZOrderQueue(int z, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ZOrderChanger, (float)this->zOrder, (float)z, speed, delay);
		return animatorZOrderChanger;
	}

	void Object::moveQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, this->rect.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, this->rect.y, y, speed, delay);
	}

	void Object::moveQueue(cgvec2f position, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, this->rect.x, position.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, this->rect.y, position.y, speed, delay);
	}

	void Object::scaleQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, this->scaleFactor.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, this->scaleFactor.y, y, speed, delay);
	}

	void Object::scaleQueue(cgvec2f scale, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, this->scaleFactor.x, scale.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, this->scaleFactor.y, scale.y, speed, delay);
	}

	void Object::resizeQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, this->rect.w, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, this->rect.h, y, speed, delay);
	}

	void Object::resizeQueue(cgvec2f size, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, this->rect.w, size.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, this->rect.h, size.y, speed, delay);
	}

	void Object::movePivotQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(PivotMoverX, this->pivot.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(PivotMoverY, this->pivot.y, y, speed, delay);
	}

	void Object::movePivotQueue(cgvec2f pivot, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(PivotMoverX, this->pivot.x, pivot.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(PivotMoverY, this->pivot.y, pivot.y, speed, delay);
	}

	void Object::fadeColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)this->color.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)this->color.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)this->color.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->color.a, (float)a, speed, delay);
	}

	void Object::fadeColorQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)this->color.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)this->color.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)this->color.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->color.a, (float)color.a, speed, delay);
	}
	
	DEFINE_DYNAMIC_ANIMATE(animateX, MoverX);
	DEFINE_DYNAMIC_ANIMATE(animateY, MoverY);
	DEFINE_DYNAMIC_ANIMATE(animateScaleX, ScalerX);
	DEFINE_DYNAMIC_ANIMATE(animateScaleY, ScalerY);
	DEFINE_DYNAMIC_ANIMATE(animateWidth, ResizerX);
	DEFINE_DYNAMIC_ANIMATE(animateHeight, ResizerY);
	DEFINE_DYNAMIC_ANIMATE(animateAngle, Rotator);
	DEFINE_DYNAMIC_ANIMATE(animatePivotX, PivotMoverX);
	DEFINE_DYNAMIC_ANIMATE(animatePivotY, PivotMoverY);
	DEFINE_DYNAMIC_ANIMATE(animateRed, RedChanger);
	DEFINE_DYNAMIC_ANIMATE(animateGreen, GreenChanger);
	DEFINE_DYNAMIC_ANIMATE(animateBlue, BlueChanger);
	DEFINE_DYNAMIC_ANIMATE(animateAlpha, AlphaChanger);
	DEFINE_DYNAMIC_ANIMATE(animateZOrder, ZOrderChanger);

	harray<Animator*> Object::animatePosition(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(MoverX);
		CREATE_DYNAMIC_ANIMATE(MoverY);
		result += animatorMoverX;
		result += animatorMoverY;
		return result;
	}

	harray<Animator*> Object::animateScale(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(ScalerX);
		CREATE_DYNAMIC_ANIMATE(ScalerY);
		result += animatorScalerX;
		result += animatorScalerY;
		return result;
	}

	harray<Animator*> Object::animateSize(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(ResizerX);
		CREATE_DYNAMIC_ANIMATE(ResizerY);
		result += animatorResizerX;
		result += animatorResizerY;
		return result;
	}

	harray<Animator*> Object::animatePivot(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(PivotMoverX);
		CREATE_DYNAMIC_ANIMATE(PivotMoverY);
		result += animatorPivotMoverX;
		result += animatorPivotMoverY;
		return result;
	}

	harray<Animator*> Object::animateColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(RedChanger);
		CREATE_DYNAMIC_ANIMATE(GreenChanger);
		CREATE_DYNAMIC_ANIMATE(BlueChanger);
		CREATE_DYNAMIC_ANIMATE(AlphaChanger);
		result += animatorRedChanger;
		result += animatorGreenChanger;
		result += animatorBlueChanger;
		result += animatorAlphaChanger;
		return result;
	}

	void Object::animateStopX()
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
	}

	void Object::animateStopY()
	{
		REMOVE_EXISTING_ANIMATORS(MoverY);
	}

	void Object::animateStopScaleX()
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
	}

	void Object::animateStopScaleY()
	{
		REMOVE_EXISTING_ANIMATORS(ScalerY);
	}

	void Object::animateStopWidth()
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
	}

	void Object::animateStopHeight()
	{
		REMOVE_EXISTING_ANIMATORS(ResizerY);
	}

	void Object::animateStopAngle()
	{
		REMOVE_EXISTING_ANIMATORS(Rotator);
	}

	void Object::animateStopPivotX()
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverX);
	}

	void Object::animateStopPivotY()
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverY);
	}

	void Object::animateStopRed()
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
	}

	void Object::animateStopGreen()
	{
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
	}

	void Object::animateStopBlue()
	{
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
	}

	void Object::animateStopAlpha()
	{
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
	}

	void Object::animateStopZOrder()
	{
		REMOVE_EXISTING_ANIMATORS(ZOrderChanger);
	}

	void Object::animateStopMove()
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		REMOVE_EXISTING_ANIMATORS(MoverY);
	}

	void Object::animateStopScale()
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		REMOVE_EXISTING_ANIMATORS(ScalerY);
	}

	void Object::animateStopSize()
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		REMOVE_EXISTING_ANIMATORS(ResizerY);
	}

	void Object::animateStopPivot()
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverX);
		REMOVE_EXISTING_ANIMATORS(PivotMoverY);
	}

	void Object::animateStopColor()
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
	}

	void Object::animateStopAll()
	{
		foreach (Animator*, it, this->dynamicAnimators)
		{
			delete (*it);
		}
		this->dynamicAnimators.clear();
	}
	
}
