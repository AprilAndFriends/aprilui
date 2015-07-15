/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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

// DEPRECATED
#define CREATE_DELAYED_DYNAMIC_ANIMATOR_F(type, offset, amplitude, speed, function, startPeriods, durationPeriods, delay) \
	Animator* animator ## type = new Animators::type(april::generateName("dynamic_animator_")); \
	this->dynamicAnimators += animator ## type; \
	animator ## type->parent = this; \
	animator ## type->setOffset(offset); \
	animator ## type->setAmplitude(amplitude); \
	animator ## type->setAnimationFunction(function); \
	animator ## type->setSpeed(speed * durationPeriods); \
	animator ## type->setPeriods(startPeriods + durationPeriods); \
	animator ## type->setTimer(startPeriods / (speed * durationPeriods)); \
	if (delay > 0.0f) \
	{ \
		animator ## type->setInheritValue(true); \
		animator ## type->setDelay(delay); \
	}
// DEPRECATED
#define CREATE_DYNAMIC_ANIMATOR_F(type, offset, amplitude, speed, function, startPeriods, durationPeriods) \
	CREATE_DELAYED_DYNAMIC_ANIMATOR_F(type, offset, amplitude, speed, function, startPeriods, durationPeriods, 0.0f);
// DEPRECATED
#define DEFINE_ANIMATOR_F(functionName, animatorName) \
	Animator* Object::functionName ## F(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods) \
	{ \
		REMOVE_EXISTING_ANIMATORS(animatorName); \
		CREATE_DYNAMIC_ANIMATOR_F(animatorName, offset, amplitude, speed, function, startPeriods, durationPeriods); \
		return animator ## animatorName; \
	}
// DEPRECATED
#define DEFINE_ANIMATOR_F_DELAYED(functionName, animatorName) \
	Animator* Object::functionName ## QueueF(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay) \
	{ \
		CREATE_DYNAMIC_ANIMATOR_F(animatorName, offset, amplitude, speed, function, startPeriods, durationPeriods); \
		return animator ## animatorName; \
	}

namespace aprilui
{
	bool Object::isClickThrough() // DEPRECATED
	{
		if (this->hitTest == HIT_TEST_DISABLED_RECURSIVE)
		{
			return true;
		}
		if (this->hitTest == HIT_TEST_DISABLED)
		{
			hlog::warn(logTag, "'hitTest' value is 'HIT_TEST_DISABLED', but accessing isClickThrough(), defaulting to false!");
		}
		return false;
	}

	DEFINE_ANIMATOR_F(moveX, MoverX); // DEPRECATED
	DEFINE_ANIMATOR_F(moveY, MoverY); // DEPRECATED
	DEFINE_ANIMATOR_F(scaleX, ScalerX); // DEPRECATED
	DEFINE_ANIMATOR_F(scaleY, ScalerY); // DEPRECATED
	DEFINE_ANIMATOR_F(resizeX, ResizerX); // DEPRECATED
	DEFINE_ANIMATOR_F(resizeY, ResizerY); // DEPRECATED
	DEFINE_ANIMATOR_F(rotate, Rotator); // DEPRECATED
	DEFINE_ANIMATOR_F(movePivotX, PivotMoverX); // DEPRECATED
	DEFINE_ANIMATOR_F(movePivotY, PivotMoverY); // DEPRECATED
	DEFINE_ANIMATOR_F(fadeRed, RedChanger); // DEPRECATED
	DEFINE_ANIMATOR_F(fadeGreen, GreenChanger); // DEPRECATED
	DEFINE_ANIMATOR_F(fadeBlue, BlueChanger); // DEPRECATED
	DEFINE_ANIMATOR_F(fadeAlpha, AlphaChanger); // DEPRECATED
	DEFINE_ANIMATOR_F(changeZOrder, ZOrderChanger); // DEPRECATED

	DEFINE_ANIMATOR_F_DELAYED(moveX, MoverX); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(moveY, MoverY); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(scaleX, ScalerX); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(scaleY, ScalerY); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(resizeX, ResizerX); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(resizeY, ResizerY); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(rotate, Rotator); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(movePivotX, PivotMoverX); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(movePivotY, PivotMoverY); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(fadeRed, RedChanger); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(fadeGreen, GreenChanger); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(fadeBlue, BlueChanger); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(fadeAlpha, AlphaChanger); // DEPRECATED
	DEFINE_ANIMATOR_F_DELAYED(changeZOrder, ZOrderChanger); // DEPRECATED

	static Animator* _map_clonedAnimators(Animator* animator)
	{
		return animator->clone();
	}
	
	static bool _filter_isAnimated(Animator* animator)
	{
		return animator->isAnimated();
	}

	static bool _filter_isWaitingAnimation(Animator* animator)
	{
		return animator->isWaitingAnimation();
	}

	harray<PropertyDescription> Object::_propertyDescriptions;

	Object::Object(chstr name) : BaseObject(name)
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
		this->hitTest = HIT_TEST_ENABLED;
		this->clip = false;
		this->inheritAlpha = true;
		this->useDisabledAlpha = true;
		this->focusIndex = -1;
		this->customPointInsideCallback = NULL;
		this->debugColor = april::Color(april::Color::Black, 32);
		this->_childUnderCursor = NULL;
		this->_checkedChildUnderCursor = false;
	}

	Object::Object(const Object& other) : BaseObject(other)
	{
		this->rect = other.rect;
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
		this->inheritAlpha = other.inheritAlpha;
		this->useDisabledAlpha = other.useDisabledAlpha;
		this->focusIndex = other.focusIndex;
		this->customPointInsideCallback = other.customPointInsideCallback;
		this->debugColor = other.debugColor;
		this->dynamicAnimators = other.dynamicAnimators.mapped(&_map_clonedAnimators);
		this->_childUnderCursor = NULL;
		this->_checkedChildUnderCursor = false;
	}

	Object::~Object()
	{
		foreach (Animator*, it, this->dynamicAnimators)
		{
			delete (*it);
		}
		this->dynamicAnimators.clear();
		if (this->isFocused())
		{
			this->setFocused(false);
		}
	}
	
	// when cloning a tree, cloned children first have to be attached to the clone, before they start creating their own clones
	Object* Object::cloneTree()
	{
		Object* cloned = this->clone();
		cloned->_cloneChildren(this->childrenObjects, this->childrenAnimators);
		return cloned;
	}

	void Object::_cloneChildren(harray<Object*>& objects, harray<Animator*>& animators)
	{
		Object* object = NULL;
		foreach (Object*, it, objects)
		{
			object = (*it)->clone();
			this->addChild(object);
			object->_cloneChildren((*it)->childrenObjects, (*it)->childrenAnimators);
		}
		foreach (Animator*, it, animators)
		{
			this->addChild((*it)->clone());
		}
	}

	harray<PropertyDescription> Object::getPropertyDescriptions()
	{
		if (Object::_propertyDescriptions.size() == 0)
		{
			Object::_propertyDescriptions += PropertyDescription("rect", PropertyDescription::GRECT);
			Object::_propertyDescriptions += PropertyDescription("position", PropertyDescription::GVEC2);
			Object::_propertyDescriptions += PropertyDescription("size", PropertyDescription::GVEC2);
			Object::_propertyDescriptions += PropertyDescription("x", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("y", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("w", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("h", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("visible", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("hit_test", PropertyDescription::ENUM);
			Object::_propertyDescriptions += PropertyDescription("click_through", PropertyDescription::BOOL); // DEPRECATED
			Object::_propertyDescriptions += PropertyDescription("inherit_alpha", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("red", PropertyDescription::UCHAR);
			Object::_propertyDescriptions += PropertyDescription("green", PropertyDescription::UCHAR);
			Object::_propertyDescriptions += PropertyDescription("blue", PropertyDescription::UCHAR);
			Object::_propertyDescriptions += PropertyDescription("alpha", PropertyDescription::UCHAR);
			Object::_propertyDescriptions += PropertyDescription("color", PropertyDescription::HEXCOLOR);
			Object::_propertyDescriptions += PropertyDescription("angle", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("scale", PropertyDescription::GVEC2);
			Object::_propertyDescriptions += PropertyDescription("scale_x", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("scale_y", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("pivot", PropertyDescription::GVEC2);
			Object::_propertyDescriptions += PropertyDescription("pivot_x", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("pivot_y", PropertyDescription::FLOAT);
			Object::_propertyDescriptions += PropertyDescription("center", PropertyDescription::GVEC2); // DEPRECATED
			Object::_propertyDescriptions += PropertyDescription("center_x", PropertyDescription::FLOAT); // DEPRECATED
			Object::_propertyDescriptions += PropertyDescription("center_y", PropertyDescription::FLOAT); // DEPRECATED
			Object::_propertyDescriptions += PropertyDescription("anchor_left", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("anchor_right", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("anchor_top", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("anchor_bottom", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("retain_anchor_aspect", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("clip", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("use_disabled_alpha", PropertyDescription::BOOL);
			Object::_propertyDescriptions += PropertyDescription("focus_index", PropertyDescription::INT);
		}
		return (BaseObject::getPropertyDescriptions() + Object::_propertyDescriptions);
	}

	void Object::addChild(BaseObject* obj)
	{
		if (obj->getParent() != NULL)
		{
			throw ObjectHasParentException(obj->getName(), this->getName());
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
		if (obj->getParent() != this)
		{
			throw ObjectNotChildException(obj->getName(), this->getName());
		}
		obj->notifyEvent(Event::DetachedFromObject, NULL);
		Object* object = dynamic_cast<Object*>(obj);
		if (object != NULL)
		{
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
			this->dataset->registerObjects(object);
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
		if (this->rect.w <= 0.0f)
		{
			this->pivot.x = difference * 0.5f;
		}
		float width = 0.0f;
		float height = 0.0f;
		float differenceAlt = 0.0f;
		foreach (Object*, it, this->childrenObjects)
		{
			width = (*it)->getWidth();
			height = (*it)->getHeight();
			differenceAlt = difference * height / width;
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
				(*it)->setWidth(width + difference);
				if (width != 0.0f)
				{
					(*it)->setPivotX((*it)->getPivotX() * (width + difference) / width);
				}
				if ((*it)->isRetainAnchorAspect())
				{
					(*it)->setHeight(height + differenceAlt);
					if ((*it)->isAnchorTop() == (*it)->isAnchorBottom())
					{
						(*it)->setY((*it)->getY() - differenceAlt * 0.5f);
					}
					else if ((*it)->isAnchorBottom())
					{
						(*it)->setY((*it)->getY() - differenceAlt);
					}
					if (height != 0.0f)
					{
						(*it)->setPivotY((*it)->getPivotY() * (height + differenceAlt) / height);
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
		if (this->rect.h <= 0.0f)
		{
			this->pivot.y = difference * 0.5f;
		}
		float width = 0.0f;
		float height = 0.0f;
		float differenceAlt = 0.0f;
		foreach (Object*, it, this->childrenObjects)
		{
			width = (*it)->getWidth();
			height = (*it)->getHeight();
			differenceAlt = difference * width / height;
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
				(*it)->setHeight(height + difference);
				if (height != 0.0f)
				{
					(*it)->setPivotY((*it)->getPivotY() * (height + difference) / height);
				}
				if ((*it)->isRetainAnchorAspect())
				{
					(*it)->setWidth(width + differenceAlt);
					if ((*it)->isAnchorLeft() == (*it)->isAnchorRight())
					{
						(*it)->setX((*it)->getX() - differenceAlt * 0.5f);
					}
					else if ((*it)->isAnchorRight())
					{
						(*it)->setX((*it)->getX() - differenceAlt);
					}
					if (width != 0.0f)
					{
						(*it)->setPivotX((*it)->getPivotX() * (width + differenceAlt) / width);
					}
				}
			}
		}
	}

	void Object::setRect(grect value)
	{
		this->_updateChildrenHorizontal(value.w - this->rect.w);
		this->_updateChildrenVertical(value.h - this->rect.h);
		this->rect = value;
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setRect(gvec2 position, gvec2 size)
	{
		this->_updateChildrenHorizontal(size.x - this->rect.w);
		this->_updateChildrenVertical(size.y - this->rect.h);
		this->rect.set(position, size);
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setRect(gvec2 position, float w, float h)
	{
		this->_updateChildrenHorizontal(w - this->rect.w);
		this->_updateChildrenVertical(h - this->rect.h);
		this->rect.set(position, w, h);
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setRect(float x, float y, gvec2 size)
	{
		this->_updateChildrenHorizontal(size.x - this->rect.w);
		this->_updateChildrenVertical(size.y - this->rect.h);
		this->rect.set(x, y, size);
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setRect(float x, float y, float w, float h)
	{
		this->_updateChildrenHorizontal(w - this->rect.w);
		this->_updateChildrenVertical(h - this->rect.h);
		this->rect.set(x, y, w, h);
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setWidth(float value)
	{
		this->_updateChildrenHorizontal(value - this->rect.w);
		this->rect.w = value;
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setHeight(float value)
	{
		this->_updateChildrenVertical(value - this->rect.h);
		this->rect.h = value;
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setSize(gvec2 value)
	{
		this->_updateChildrenHorizontal(value.x - this->rect.w);
		this->_updateChildrenVertical(value.y - this->rect.h);
		this->rect.setSize(value);
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setSize(float w, float h)
	{
		this->_updateChildrenHorizontal(w - this->rect.w);
		this->_updateChildrenVertical(h - this->rect.h);
		this->rect.setSize(w, h);
		this->notifyEvent(Event::Resized, NULL);
	}

	void Object::setAnchors(bool left, bool right, bool top, bool bottom)
	{
		this->anchorLeft = left;
		this->anchorRight = right;
		this->anchorTop = top;
		this->anchorBottom = bottom;
	}

	unsigned char Object::getDerivedAlpha(aprilui::Object* overrideRoot)
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

	float Object::_getDerivedAngle(aprilui::Object* overrideRoot)
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

	bool Object::isAnimated()
	{
		return (this->dynamicAnimators + this->childrenAnimators).matchesAny(&_filter_isAnimated);
	}

	bool Object::isWaitingAnimation()
	{
		return (this->dynamicAnimators + this->childrenAnimators).matchesAny(&_filter_isWaitingAnimation);
	}

	bool Object::hasDynamicAnimation()
	{
		return (this->dynamicAnimators.size() > 0);
	}

	void Object::draw()
	{
		if (!this->isVisible() || heqf(this->scaleFactor.x, 0.0f, 0.0001f) || heqf(this->scaleFactor.y, 0.0f, 0.0001f))
		{
			return;
		}
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		grect viewport;
		grect orthoProjection;
		bool clipped = (this->clip && this->parent != NULL);
		if (clipped)
		{
			orthoProjection = april::rendersys->getOrthoProjection();
			viewport = april::rendersys->getViewport();
			gvec2 ratio = viewport.getSize() / orthoProjection.getSize();
			grect originalRect = this->parent->getBoundingRect() + orthoProjection.getPosition();
			grect newViewport(originalRect.getPosition() * ratio, originalRect.getSize() * ratio);
			newViewport.clip(viewport);
			if (newViewport.w <= 0.0f || newViewport.h <= 0.0f)
			{
				return;
			}
			grect newRect(newViewport.getPosition() / ratio, newViewport.getSize() / ratio);
			originalRect.clip(newRect);
			april::rendersys->setOrthoProjection(grect(orthoProjection.getPosition() - originalRect.getPosition(), originalRect.getSize()));
			april::rendersys->setViewport(newViewport);
		}
		gvec2 position = this->rect.getPosition() + this->pivot;
		if (position.x != 0.0f || position.y != 0.0f)
		{
			april::rendersys->translate(position.x, position.y);
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
		if (clipped)
		{
			april::rendersys->setOrthoProjection(orthoProjection);
			april::rendersys->setViewport(viewport);
		}
		april::rendersys->setModelviewMatrix(originalMatrix);
	}
	
	void Object::_update(float timeDelta)
	{
		if (this->_checkedChildUnderCursor)
		{
			this->clearChildUnderCursor();
		}
		BaseObject::_update(timeDelta);
		// because this list could change during the update() call
		harray<Animator*> animators = this->dynamicAnimators;
		// first update the animators
		foreach (Animator*, it, animators)
		{
			(*it)->update(timeDelta);
		}
		animators = this->dynamicAnimators;
		this->dynamicAnimators.clear();
		foreach (Animator*, it, animators)
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

	void Object::_draw()
	{
	}

	void Object::_drawDebug()
	{
		grect rect = this->_makeDrawRect();
		if (this->debugColor.a > 0)
		{
			april::rendersys->drawFilledRect(rect, this->debugColor);
		}
		april::Color frameColor = april::Color::Green;
		if (this->hitTest == HIT_TEST_DISABLED)
		{
			frameColor = april::Color::Yellow;
		}
		else if (this->hitTest == HIT_TEST_DISABLED_RECURSIVE)
		{
			frameColor = april::Color::Red;
		}
		else if (!this->_isDerivedHitTestEnabled())
		{
			frameColor = april::Color::Cyan;
		}
		april::rendersys->drawRect(rect, april::Color(frameColor, 224));
		april::rendersys->drawRect(grect(-1.0f, -1.0f, 2.0f, 2.0f), april::Color::White);
		april::rendersys->drawRect(grect(-3.0f, -3.0f, 6.0f, 6.0f), april::Color::Green);
	}

	bool Object::isCursorInside()
	{
		return this->isPointInside(aprilui::getCursorPosition());
	}
	
	bool Object::isPointInside(gvec2 position)
	{
		if (heqf(this->scaleFactor.x, 0.0f, 0.0001f) || heqf(this->scaleFactor.y, 0.0f, 0.0001f))
		{
			return false;
		}
		if (this->parent != NULL)
		{
			Object* obj = this;
			Object* ancestor = this->parent;
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
		if (this->customPointInsideCallback)
		{
			return this->customPointInsideCallback(this, position);
		}
		return grect(0.0f, 0.0f, this->rect.getSize()).isPointInside(this->transformToLocalSpace(position));
	}

	bool Object::onMouseDown(april::Key keyCode)
	{
		if (this->hitTest == HIT_TEST_DISABLED_RECURSIVE || !this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		if (this->hitTest == HIT_TEST_ENABLED && this->dataset != NULL)
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
		if (this->hitTest == HIT_TEST_DISABLED_RECURSIVE || !this->isVisible() || !this->isDerivedEnabled())
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

	bool Object::onTouch(const harray<gvec2>& touches)
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

	bool Object::_touch(const harray<gvec2>& touches)
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

	void Object::resetPivot()
	{
		this->pivot = this->rect.getSize() / 2;
	}

	bool Object::isDerivedVisible()
	{
		return (this->isVisible() && (this->parent == NULL || this->parent->isDerivedVisible()));
	}
	
	bool Object::_isDerivedHitTestEnabled()
	{
		if (this->hitTest != HIT_TEST_ENABLED)
		{
			return false;
		}
		Object* ancestor = this->parent;
		while (ancestor != NULL)
		{
			if (ancestor->hitTest == HIT_TEST_DISABLED_RECURSIVE)
			{
				return false;
			}
			ancestor = ancestor->getParent();
		}
		return true;
	}
	
	hstr Object::getProperty(chstr name)
	{
		if (name == "rect")					return april::grectToHstr(this->getRect());
		if (name == "position")				return april::gvec2ToHstr(this->getPosition());
		if (name == "size")					return april::gvec2ToHstr(this->getSize());
		if (name == "x")					return this->getX();
		if (name == "y")					return this->getY();
		if (name == "w")					return this->getWidth();
		if (name == "h")					return this->getHeight();
		if (name == "visible")				return this->getVisibilityFlag();
		if (name == "hit_test")
		{
			if (this->hitTest == HIT_TEST_ENABLED)				return "enabled";
			if (this->hitTest == HIT_TEST_DISABLED)				return "disabled";
			if (this->hitTest == HIT_TEST_DISABLED_RECURSIVE)	return "disabled_recursive";
		}
		if (name == "click_through")
		{
			hlog::warn(logTag, "'click_through' is deprecated. Use 'hit_test' instead."); // DEPRECATED
			return (this->getHitTest() == HIT_TEST_DISABLED_RECURSIVE);
		}
		if (name == "inherit_alpha")		return this->isInheritAlpha();
		if (name == "red")					return this->getRed();
		if (name == "green")				return this->getGreen();
		if (name == "blue")					return this->getBlue();
		if (name == "alpha")				return this->getAlpha();
		if (name == "color")				return this->getColor().hex();
		if (name == "angle")				return this->getAngle();
		if (name == "scale")				return april::gvec2ToHstr(this->getScale());
		if (name == "scale_x")				return this->getScaleX();
		if (name == "scale_y")				return this->getScaleY();
		if (name == "pivot")				return april::gvec2ToHstr(this->getPivot());
		if (name == "pivot_x")				return this->getPivotX();
		if (name == "pivot_y")				return this->getPivotY();
		if (name == "center")
		{
			hlog::warn(logTag, "'center' is deprecated. Use 'pivot_x' instead."); // DEPRECATED
			return april::gvec2ToHstr(this->getPivot());
		}
		if (name == "center_x")
		{
			hlog::warn(logTag, "'center_x' is deprecated. Use 'pivot_x' instead."); // DEPRECATED
			return this->getPivotX();
		}
		if (name == "center_y")
		{
			hlog::warn(logTag, "'center_y' is deprecated. Use 'pivot_y' instead."); // DEPRECATED
			return this->getPivotY();
		}
		if (name == "anchor_left")			return this->isAnchorLeft();
		if (name == "anchor_right")			return this->isAnchorRight();
		if (name == "anchor_top")			return this->isAnchorTop();
		if (name == "anchor_bottom")		return this->isAnchorBottom();
		if (name == "retain_anchor_aspect")	return this->isRetainAnchorAspect();
		if (name == "clip")					return this->isClip();
		if (name == "use_disabled_alpha")	return this->isUseDisabledAlpha();
		if (name == "focus_index")			return this->getFocusIndex();
		return BaseObject::getProperty(name);
	}
	
	bool Object::setProperty(chstr name, chstr value)
	{
		if		(name == "rect")					this->setRect(april::hstrToGrect(value));
		else if	(name == "position")				this->setPosition(april::hstrToGvec2(value));
		else if	(name == "size")					this->setSize(april::hstrToGvec2(value));
		else if	(name == "x")						this->setX(value);
		else if	(name == "y")						this->setY(value);
		else if	(name == "w")						this->setWidth(value);
		else if	(name == "h")						this->setHeight(value);
		else if	(name == "visible")					this->setVisible(value);
		else if	(name == "hit_test")
		{
			if (value == "enabled")					this->setHitTest(HIT_TEST_ENABLED);
			else if (value == "disabled")			this->setHitTest(HIT_TEST_DISABLED);
			else if (value == "disabled_recursive")	this->setHitTest(HIT_TEST_DISABLED_RECURSIVE);
			else
			{
				hlog::warn(logTag, "'hit_test=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (name == "click_through")
		{
			if (value)
			{
				hlog::warn(logTag, "'click_through=\"1\"' is deprecated. Use 'hit_test=\"disabled_recursive\"' instead."); // DEPRECATED
			}
			else
			{
				hlog::warn(logTag, "'click_through=\"0\"' is deprecated. Use 'hit_test=\"enabled\"' instead."); // DEPRECATED
			}
			this->setHitTest(value ? HIT_TEST_DISABLED_RECURSIVE : HIT_TEST_ENABLED);
		}
		else if (name == "inherit_alpha")			this->setInheritAlpha(value);
		else if	(name == "red")						this->setRed((int)value);
		else if	(name == "green")					this->setGreen((int)value);
		else if	(name == "blue")					this->setBlue((int)value);
		else if	(name == "alpha")					this->setAlpha((int)value);
		else if	(name == "color")					this->setColor(value);
		else if	(name == "angle")					this->setAngle(value);
		else if	(name == "scale")					this->setScale(april::hstrToGvec2(value));
		else if	(name == "scale_x")					this->setScaleX(value);
		else if	(name == "scale_y")					this->setScaleY(value);
		else if (name == "pivot")					this->setPivot(april::hstrToGvec2(value));
		else if (name == "pivot_x")					this->setPivotX(value);
		else if (name == "pivot_y")					this->setPivotY(value);
		else if (name == "center")
		{
			hlog::warn(logTag, "'center=' is deprecated. Use 'pivot=' instead."); // DEPRECATED
			this->setPivot(april::hstrToGvec2(value));
		}
		else if (name == "center_x")
		{
			hlog::warn(logTag, "'center_x=' is deprecated. Use 'pivot_x=' instead."); // DEPRECATED
			this->setPivotX(value);
		}
		else if (name == "center_y")
		{
			hlog::warn(logTag, "'center_y=' is deprecated. Use 'pivot_y=' instead."); // DEPRECATED
			this->setPivotY(value);
		}
		else if (name == "anchor_left")				this->setAnchorLeft(value);
		else if	(name == "anchor_right")			this->setAnchorRight(value);
		else if	(name == "anchor_top")				this->setAnchorTop(value);
		else if	(name == "anchor_bottom")			this->setAnchorBottom(value);
		else if	(name == "retain_anchor_aspect")	this->setRetainAnchorAspect(value);
		else if	(name == "anchors")
		{
			harray<hstr> anchors = value.replaced(" ", "").lowered().split(",", -1, true);
			this->setAnchorLeft(anchors.has("all") || anchors.has("left"));
			this->setAnchorRight(anchors.has("all") || anchors.has("right"));
			this->setAnchorTop(anchors.has("all") || anchors.has("top"));
			this->setAnchorBottom(anchors.has("all") || anchors.has("bottom"));
			anchors.removeAll("left");
			anchors.removeAll("right");
			anchors.removeAll("top");
			anchors.removeAll("bottom");
			anchors.removeAll("none");
			anchors.removeAll("all");
			if (anchors.size() > 0)
			{
				hlog::warn(logTag, "'anchors=' does not support values '" + anchors.joined(",") + "'.");
				return false;
			}
		}
		else if	(name == "clip")					this->setClip(value);
		else if	(name == "use_disabled_alpha")		this->setUseDisabledAlpha(value);
		else if	(name == "focus_index")				this->setFocusIndex(value);
		else return BaseObject::setProperty(name, value);
		return true;
	}

	bool Object::angleEquals(float angle)
	{
		float s1 = (float)dsin(angle);
		float c1 = (float)dcos(angle);
		float s2 = (float)dsin(angle);
		float c2 = (float)dcos(angle);
		return (heqf(s1, s2, (float)HL_E_TOLERANCE) && heqf(c1, c2, (float)HL_E_TOLERANCE));
	}

	Object* Object::getChildUnderPoint(gvec2 pos)
	{
		if (!this->isVisible() || this->hitTest == HIT_TEST_DISABLED_RECURSIVE)
		{
			return NULL;
		}
		Object* object = NULL;
		foreach_r (Object*, it, this->childrenObjects)
		{
			object = (*it)->getChildUnderPoint(pos);
			if (object != NULL)
			{
				break;
			}
		}
		return (object == NULL && this->hitTest == HIT_TEST_ENABLED && this->isPointInside(pos) ? this : object);
	}

	Object* Object::getChildUnderPoint(float x, float y)
	{
		return this->getChildUnderPoint(gvec2(x, y));
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
	
	void Object::clearChildUnderCursor()
	{
		this->_childUnderCursor = NULL;
		this->_checkedChildUnderCursor = false;
	}
	
	harray<gvec2> Object::transformToLocalSpace(harray<gvec2> points, aprilui::Object* overrideRoot)
	{
		harray<Object*> sequence;
		Object* current = this;
		while (current != NULL)
		{
			sequence += current;
			current = ((overrideRoot == NULL || overrideRoot != current) ? current->getParent() : NULL);
		}
		sequence.reverse();
		gvec2 pivot;
		gvec2 scale;
		gvec2 position;
		float angle;
		foreach (Object*, it, sequence)
		{
			pivot = (*it)->getPivot();
			scale = (*it)->getScale();
			position = (*it)->getPosition();
			angle = (*it)->getAngle();
			foreach (gvec2, it2, points)
			{
				(*it2) -= pivot + position;
				(*it2).rotate(angle);
				(*it2) /= scale;
				(*it2) += pivot;
			}
		}
		return points;
	}
	
	bool Object::isFocused()
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

	gvec2 Object::transformToLocalSpace(gvec2 point, aprilui::Object* overrideRoot)
	{
		harray<Object*> sequence;
		Object* current = this;
		while (current != NULL)
		{
			sequence += current;
			current = ((overrideRoot == NULL || overrideRoot != current) ? current->getParent() : NULL);
		}
		sequence.reverse();
		gvec2 pivot;
		foreach (Object*, it, sequence)
		{
			pivot = (*it)->getPivot();
			point -= pivot + (*it)->getPosition();
			point.rotate((*it)->getAngle());
			point /= (*it)->getScale();
			point += pivot;
		}
		return point;
	}

	harray<gvec2> Object::getDerivedPoints(harray<gvec2> points, aprilui::Object* overrideRoot)
	{
		Object* current = this;
		gvec2 pivot;
		gvec2 scale;
		gvec2 position;
		float angle;
		while (current != NULL)
		{
			pivot = current->getPivot();
			scale = current->getScale();
			position = current->getPosition();
			angle = current->getAngle();
			foreach (gvec2, it, points)
			{
				(*it) -= pivot;
				(*it) *= scale;
				(*it).rotate(-angle);
				(*it) += pivot + position;
			}
			current = ((overrideRoot == NULL || overrideRoot != current) ? current->getParent() : NULL);
		}
		return points;
	}

	gvec2 Object::getDerivedPoint(gvec2 point, aprilui::Object* overrideRoot)
	{
		Object* current = this;
		gvec2 pivot;
		while (current != NULL)
		{
			pivot = current->getPivot();
			point -= pivot;
			point *= current->getScale();
			point.rotate(-current->getAngle());
			point += pivot + current->getPosition();
			current = ((overrideRoot == NULL || overrideRoot != current) ? current->getParent() : NULL);
		}
		return point;
	}

	grect Object::getBoundingRect(aprilui::Object* overrideRoot)
	{
		gvec2 max;
		gvec2 min;
		harray<gvec2> corners = this->getDerivedCorners(overrideRoot);
		min = max = corners.removeFirst(); // guaranteed to return 4 corner points previously
		gvec2 corner;
		while (corners.size() > 0)
		{
			corner = corners.removeFirst();
			max.x = hmax(max.x, corner.x);
			max.y = hmax(max.y, corner.y);
			min.x = hmin(min.x, corner.x);
			min.y = hmin(min.y, corner.y);
		}
		return grect(min, max - min);
	}
	
	harray<gvec2> Object::getDerivedCorners(aprilui::Object* overrideRoot)
	{
		harray<gvec2> points;
		points += gvec2(0.0f, 0.0f);
		points += gvec2(0.0f, this->rect.h);
		points += gvec2(this->rect.w, 0.0f);
		points += gvec2(this->rect.w, this->rect.h);
		return this->getDerivedPoints(points, overrideRoot);
	}
	
	gvec2 Object::getDerivedPosition(aprilui::Object* overrideRoot)
	{
		return this->getDerivedPoint(gvec2(0.0f, 0.0f), overrideRoot);
	}
	
	gvec2 Object::getDerivedSize(aprilui::Object* overrideRoot)
	{
		return this->getBoundingRect(overrideRoot).getSize();
	}

	gvec2 Object::getDerivedPivot(aprilui::Object* overrideRoot)
	{
		return this->getDerivedPoint(this->pivot, overrideRoot);
	}
	
	gvec2 Object::getDerivedScale(aprilui::Object* overrideRoot)
	{
		if (overrideRoot == this)
		{
			return this->scaleFactor;
		}
		gvec2 scaleFactor = this->scaleFactor;
		if (this->parent != overrideRoot && this->parent != NULL)
		{
			scaleFactor *= this->parent->getDerivedScale(overrideRoot);
		}
		return scaleFactor;
	}
	
	float Object::getDerivedAngle(aprilui::Object* overrideRoot)
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
	
	harray<BaseImage*> Object::getUsedImages()
	{
		harray<BaseImage*> images = this->_getUsedImages();
		images.removeAll(NULL);
		images.removeDuplicates();
		return images;
	}

	harray<BaseImage*> Object::_getUsedImages()
	{
		return harray<BaseImage*>();
	}

	grect Object::_makeDrawRect()
	{
		return grect(-this->pivot, this->rect.getSize());
	}

	april::Color Object::_makeDrawColor()
	{
		float alpha = (float)this->color.a;
		if (this->inheritAlpha)
		{
			alpha *= this->getDerivedAlpha() / 255.0f;
		}
		if (this->useDisabledAlpha && !this->isDerivedEnabled())
		{
			alpha *= 0.5f;
		}
		return april::Color(this->color, (unsigned char)alpha);
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

	void Object::move(gvec2 position, float speed)
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

	void Object::scale(gvec2 scale, float speed)
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

	void Object::resize(gvec2 size, float speed)
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

	void Object::movePivot(gvec2 pivot, float speed)
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
	
	void Object::fadeColor(april::Color color, float speed)
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

	void Object::moveQueue(gvec2 position, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, this->rect.x, position.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, this->rect.y, position.y, speed, delay);
	}

	void Object::scaleQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, this->scaleFactor.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, this->scaleFactor.y, y, speed, delay);
	}

	void Object::scaleQueue(gvec2 scale, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, this->scaleFactor.x, scale.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, this->scaleFactor.y, scale.y, speed, delay);
	}

	void Object::resizeQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, this->rect.w, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, this->rect.h, y, speed, delay);
	}

	void Object::resizeQueue(gvec2 size, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, this->rect.w, size.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, this->rect.h, size.y, speed, delay);
	}

	void Object::movePivotQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(PivotMoverX, this->pivot.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(PivotMoverY, this->pivot.y, y, speed, delay);
	}

	void Object::movePivotQueue(gvec2 pivot, float speed, float delay)
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

	void Object::fadeColorQueue(april::Color color, float speed, float delay)
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

	void Object::moveXStop()
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
	}

	void Object::moveYStop()
	{
		REMOVE_EXISTING_ANIMATORS(MoverY);
	}

	void Object::scaleXStop()
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
	}

	void Object::scaleYStop()
	{
		REMOVE_EXISTING_ANIMATORS(ScalerY);
	}

	void Object::resizeXStop()
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
	}

	void Object::resizeYStop()
	{
		REMOVE_EXISTING_ANIMATORS(ResizerY);
	}

	void Object::rotateStop()
	{
		REMOVE_EXISTING_ANIMATORS(Rotator);
	}

	void Object::movePivotXStop()
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverX);
	}

	void Object::movePivotYStop()
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverY);
	}

	void Object::fadeRedStop()
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
	}

	void Object::fadeGreenStop()
	{
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
	}

	void Object::fadeBlueStop()
	{
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
	}

	void Object::fadeAlphaStop()
	{
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
	}

	void Object::changeZOrderStop()
	{
		REMOVE_EXISTING_ANIMATORS(ZOrderChanger);
	}

	void Object::moveStop()
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		REMOVE_EXISTING_ANIMATORS(MoverY);
	}

	void Object::scaleStop()
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		REMOVE_EXISTING_ANIMATORS(ScalerY);
	}

	void Object::resizeStop()
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		REMOVE_EXISTING_ANIMATORS(ResizerY);
	}

	void Object::movePivotStop()
	{
		REMOVE_EXISTING_ANIMATORS(PivotMoverX);
		REMOVE_EXISTING_ANIMATORS(PivotMoverY);
	}

	void Object::fadeColorStop()
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
	}

	void Object::stopAllAnimations()
	{
		foreach (Animator*, it, this->dynamicAnimators)
		{
			delete (*it);
		}
		this->dynamicAnimators.clear();
	}
	
}
