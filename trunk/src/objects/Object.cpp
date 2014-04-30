/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.14
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
#include "CallbackEvent.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "Exception.h"
#include "ObjectCallbackObject.h"

#define REMOVE_EXISTING_ANIMATORS(name) \
	int i ## name = 0; \
	Animators::name* removeAnimator ## name; \
	while (i ## name < dynamicAnimators.size()) \
	{ \
		removeAnimator ## name = dynamic_cast<Animators::name*>(dynamicAnimators[i ## name]); \
		if (removeAnimator ## name != NULL) \
		{ \
			delete removeAnimator ## name; \
			this->dynamicAnimators.remove_at(i ## name); \
		} \
		else \
		{ \
			++(i ## name); \
		} \
	}

#define CREATE_DYNAMIC_ANIMATOR(type, offset, target, speed) \
	CREATE_DELAYED_DYNAMIC_ANIMATOR(type, offset, target, speed, 0.0f);

#define CREATE_DELAYED_DYNAMIC_ANIMATOR(type, offset, target, speed, delay) \
	Animator* animator ## type = new Animators::type(generateName("dynamic_animator_")); \
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

#define CREATE_DYNAMIC_ANIMATOR_F(type, offset, amplitude, speed, function, periodStart, periodLength) \
	CREATE_DELAYED_DYNAMIC_ANIMATOR_F(type, offset, amplitude, speed, function, periodStart, periodLength, 0.0f);

#define CREATE_DELAYED_DYNAMIC_ANIMATOR_F(type, offset, amplitude, speed, function, periodStart, periodLength, delay) \
	Animator* animator ## type = new Animators::type(generateName("dynamic_animator_")); \
	this->dynamicAnimators += animator ## type; \
	animator ## type->parent = this; \
	animator ## type->setOffset(offset); \
	animator ## type->setAmplitude(amplitude); \
	animator ## type->setAnimationFunction(function); \
	animator ## type->setSpeed(speed * periodLength); \
	animator ## type->setPeriods(periodStart + periodLength); \
	animator ## type->setTimer(periodStart / (speed * periodLength)); \
	if (delay > 0.0f) \
	{ \
		animator ## type->setInheritValue(true); \
		animator ## type->setDelay(delay); \
	}

#define DEFINE_ANIMATOR_F(functionName, animatorName) \
	Animator* Object::functionName ## F(float offset, float amplitude, float speed, AnimationFunction function, float periodStart, float periodLength) \
	{ \
		REMOVE_EXISTING_ANIMATORS(animatorName); \
		CREATE_DYNAMIC_ANIMATOR_F(animatorName, offset, amplitude, speed, function, periodStart, periodLength); \
		return animator ## animatorName; \
	}

#define DEFINE_ANIMATOR_F_DELAYED(functionName, animatorName) \
	Animator* Object::functionName ## QueueF(float offset, float amplitude, float speed, AnimationFunction function, float periodStart, float periodLength, float delay) \
	{ \
		CREATE_DELAYED_DYNAMIC_ANIMATOR_F(animatorName, offset, amplitude, speed, function, periodStart, periodLength, delay); \
		return animator ## animatorName; \
	}

namespace aprilui
{
	static bool _objectSortCallback(Object* a, Object* b)
	{
		return (a->getZOrder() < b->getZOrder());
	}

	Object::Object(chstr name, grect rect) : EventReceiver()
	{
		this->name = name;
		this->rect = rect;
		if (this->rect.w != -1)
		{
			this->center.x = this->rect.w * 0.5f;
		}
		if (this->rect.h != -1)
		{
			this->center.y = this->rect.h* 0.5f;
		}
		this->scaleFactor.set(1.0f, 1.0f);
		this->parent = NULL;
		this->childUnderCursor = NULL;
		this->checkedChildUnderCursor = false;
		this->dataset = NULL;
		this->zOrder = 0;
		this->enabled = true;
		this->visible = true;
		this->angle = 0.0f;
		this->clickThrough = false;
		this->inheritAlpha = true;
		this->anchorLeft = true;
		this->anchorRight = false;
		this->anchorTop = true;
		this->anchorBottom = false;
		this->retainAnchorAspect = false;
		this->focusIndex = -1;
		this->clip = false;
		this->useDisabledAlpha = true;
		this->customPointInsideCallback = NULL;
	}

	Object::~Object()
	{
		foreach_m (Event*, it, this->events)
		{
			if (dataset) dataset->removeCallbackFromQueue(it->second);
			delete it->second;
		}
		this->events.clear();
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
	
	void Object::_sortChildren()
	{
		this->children.sort(&_objectSortCallback);
	}

	hstr Object::getFullName()
	{
		return (this->dataset != NULL ? this->dataset->getName() + "." + this->name : this->name);
	}

	void Object::addChild(Object* object)
	{
		if (object->getParent())
		{
			throw ObjectHasParentException(object->getName(), this->getName());
		}
		this->children += object;
		this->_sortChildren();
		object->parent = this;
		object->notifyEvent("AttachToObject", NULL);
	}
	
	void Object::removeChild(Object* object)
	{
		if (object->getParent() != this)
		{
			throw ObjectNotChildException(object->getName(), this->getName());
		}
		object->notifyEvent("DetachFromObject", NULL);
		this->children -= object;
		object->parent = NULL;
	}

	void Object::registerChild(Object* object)
	{
		this->addChild(object);
		this->dataset->registerObjects(object);
	}
	
	void Object::unregisterChild(Object* object)
	{
		this->removeChild(object);
		this->dataset->unregisterObjects(object);
	}

	void Object::removeChildren(bool recursive)
	{
		if (recursive)
		{
			foreach (Object*, it, this->children)
			{
				(*it)->removeChildren(recursive);
			}
		}
		foreach (Object*, it, this->children)
		{
			(*it)->parent = NULL;
		}
		this->children.clear();
	}
	
	void Object::destroyChildren()
	{
		while (this->children.size() > 0)
		{
			this->dataset->destroyObject(this->children.first());
		}
	}
	
	void Object::attach(Object* object)
	{
		object->addChild(this);
	}
	
	void Object::detach()
	{
		if (this->parent == NULL)
		{
			throw ObjectWithoutParentException(this->getName());
		}
		this->parent->removeChild(this);
	}
	
	Object* Object::findChildByName(chstr name)
	{
		foreach (Object*, it, this->children)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}
	
	Object* Object::findDescendantByName(chstr name)
	{
		Object* object = this->findChildByName(name);
		if (object != NULL)
		{
			return object;
		}
		foreach (Object*, it, this->children)
		{
			object = (*it)->findDescendantByName(name);
			if (object != NULL)
			{
				return object;
			}
		}
		return NULL;
	}

	void Object::setZOrder(int zorder)
	{
		if (this->zOrder != zorder)
		{
			this->zOrder = zorder;
			if (this->parent != NULL)
			{
				this->parent->_sortChildren();
			}
		}
	}

	void Object::_updateChildrenHorizontal(float difference)
	{
		if (heqf(difference, 0.0f))
		{
			return;
		}
		float width;
		float height;
		float differenceAlt;
		foreach (Object*, it, this->children)
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
					(*it)->setCenterX((*it)->getCenterX() * (width + difference) / width);
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
						(*it)->setCenterY((*it)->getCenterY() * (height + differenceAlt) / height);
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
		float width;
		float height;
		float differenceAlt;
		foreach (Object*, it, this->children)
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
					(*it)->setCenterY((*it)->getCenterY() * (height + difference) / height);
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
						(*it)->setCenterX((*it)->getCenterX() * (width + differenceAlt) / width);
					}
				}
			}
		}
	}

	void Object::setWidth(float value)
	{
		this->_updateChildrenHorizontal(value - this->rect.w);
		this->rect.w = value;
		this->notifyEvent("Resized", NULL);
	}

	void Object::setHeight(float value)
	{
		this->_updateChildrenVertical(value - this->rect.h);
		this->rect.h = value;
		this->notifyEvent("Resized", NULL);
	}

	void Object::setSize(gvec2 value)
	{
		this->_updateChildrenHorizontal(value.x - this->rect.w);
		this->_updateChildrenVertical(value.y - this->rect.h);
		this->rect.setSize(value);
		this->notifyEvent("Resized", NULL);
	}

	void Object::setSize(float w, float h)
	{
		this->_updateChildrenHorizontal(w - this->rect.w);
		this->_updateChildrenVertical(h - this->rect.h);
		this->rect.w = w;
		this->rect.h = h;
		this->notifyEvent("Resized", NULL);
	}

	void Object::setRect(grect value)
	{
		this->_updateChildrenHorizontal(value.w - this->rect.w);
		this->_updateChildrenVertical(value.h - this->rect.h);
		this->rect = value;
		this->notifyEvent("Resized", NULL);
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
		foreach (Animator*, it, this->dynamicAnimators)
		{
			if ((*it)->isAnimated())
			{
				return true;
			}
		}
		foreach (Object*, it, this->children)
		{
			if (dynamic_cast<Animator*>(*it) != NULL && (*it)->isAnimated())
			{
				return true;
			}
		}
		return false;
	}

	bool Object::isWaitingAnimation()
	{
		foreach (Animator*, it, this->dynamicAnimators)
		{
			if ((*it)->isWaitingAnimation())
			{
				return true;
			}
		}
		foreach (Object*, it, this->children)
		{
			if (dynamic_cast<Animator*>(*it) != NULL && (*it)->isWaitingAnimation())
			{
				return true;
			}
		}
		return false;
	}

	bool Object::hasDynamicAnimation()
	{
		return (this->dynamicAnimators.size() > 0);
	}

	void Object::draw()
	{
		if (!this->isVisible() || heqf(this->scaleFactor.x, 0.0f, aprilui::eTolerance) || heqf(this->scaleFactor.y, 0.0f, aprilui::eTolerance))
		{
			return;
		}
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		grect viewport;
		grect orthoProjection;
		bool clipped = (this->clip && this->parent != NULL);
		if (clipped)
		{
			// TODO - this has to be implemented using clipping planes in April
			orthoProjection = april::rendersys->getOrthoProjection();
			viewport = april::rendersys->getViewport();
			grect rect = this->parent->getBoundingRect();
			gvec2 ratio = orthoProjection.getSize() / viewport.getSize();
			april::rendersys->setOrthoProjection(grect(-rect.getPosition(), rect.getSize()));
			april::rendersys->setViewport(grect((rect.getPosition() + orthoProjection.getPosition()) / ratio, rect.getSize() / ratio));
		}
		gvec2 position = this->rect.getPosition() + this->center;
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
		this->OnDraw();
		if (aprilui::isDebugEnabled())
		{
			this->OnDrawDebug();
		}
		if (this->center.x != 0.0f || this->center.y != 0.0f)
		{
			april::rendersys->translate(-this->center.x, -this->center.y);
		}
		foreach (Object*, it, children)
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
	
	void Object::update(float timeDelta)
	{
		if (this->checkedChildUnderCursor)
		{
			this->clearChildUnderCursor();
		}
		foreach (Object*, it, this->children)
		{
			(*it)->update(timeDelta);
		}
		foreach (Animator*, it, this->dynamicAnimators)
		{
			(*it)->update(timeDelta);
		}
		// faster than creating a new array, adding expired animators and the deleting them
		int i = 0;
		Animator* animator = NULL;
		while (i < this->dynamicAnimators.size())
		{
			animator = this->dynamicAnimators[i];
			if (animator->isExpired())
			{
				delete animator;
				this->dynamicAnimators.remove_at(i);
			}
			else
			{
				++i;
			}
		}
	}

	void Object::OnDraw()
	{
	}

	void Object::OnDrawDebug()
	{
		april::rendersys->drawRect(grect(-1.0f, -1.0f, 2.0f, 2.0f), april::Color::White);
		april::rendersys->drawRect(grect(-3.0f, -3.0f, 6.0f, 6.0f), april::Color::Green);
	}

	bool Object::isCursorInside()
	{
		return this->isPointInside(aprilui::getCursorPosition());
	}
	
	bool Object::isPointInside(gvec2 position)
	{
		if (heqf(this->scaleFactor.x, 0.0f, aprilui::eTolerance) || heqf(this->scaleFactor.y, 0.0f, aprilui::eTolerance))
		{
			return false;
		}
		if (this->parent != NULL)
		{
			Object* obj = this;
			Object* parent = this->parent;
			while (parent != NULL)
			{
				if (obj->isClip() && !parent->getBoundingRect().isPointInside(position))
				{
					return false;
				}
				obj = parent;
				parent = obj->getParent();
			}
		}
		if (this->customPointInsideCallback) return this->customPointInsideCallback(this, position);
		return grect(0.0f, 0.0f, this->rect.getSize()).isPointInside(this->transformToLocalSpace(position));
	}

	bool Object::onMouseDown(april::Key keyCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (this->clickThrough || !this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		if (this->dataset != NULL)
		{
			this->dataset->removeFocus();
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if (!(*it)->isClickThrough() && (*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onMouseDown(keyCode))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::onMouseUp(april::Key keyCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (this->clickThrough || !this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		harray<Object*> validObjects;
		Object* object = NULL;
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if (!(*it)->isClickThrough() && (*it)->isVisible() && (*it)->isDerivedEnabled())
			{
				if (object == NULL && (*it)->onMouseUp(keyCode))
				{
					object = (*it);
				}
				else
				{
					validObjects += (*it);
				}
			}
		}
		if (object != NULL)
		{
			foreach (Object*, it, validObjects)
			{
				(*it)->onMouseCancel(keyCode);
			}
            // does not call the mouse cancel event for its children
            this->mouseCancel();
			return true;
		}
		return false;
	}

	bool Object::onMouseCancel(april::Key keyCode)
	{
		this->mouseCancel();
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			(*it)->onMouseCancel(keyCode);
		}
		return true;
	}

	bool Object::onMouseMove()
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onMouseMove())
			{
				return true;
			}
		}
		return false;
	}

	bool Object::onMouseScroll(float x, float y)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onMouseScroll(x, y))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::onKeyDown(april::Key keyCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onKeyDown(keyCode))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::onKeyUp(april::Key keyCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onKeyUp(keyCode))
			{
				return true;
			}
		}
		return false;
	}
	
	bool Object::onChar(unsigned int charCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onChar(charCode))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::onTouch(const harray<gvec2>& touches)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onTouch(touches))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::onButtonDown(april::Button buttonCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onButtonDown(buttonCode))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::onButtonUp(april::Button buttonCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case children gets changed
		harray<Object*> children = this->children;
		foreach_r (Object*, it, children)
		{
			// this check is generally important and should not be removed (the previous one should be removed for the system to work properly)
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && (*it)->onButtonUp(buttonCode))
			{
				return true;
			}
		}
		return false;
	}

	void Object::mouseCancel()
	{
	}
	
	void Object::registerEvent(chstr name, void (*callback)(EventArgs*))
	{
		this->registerEvent(name, new CallbackEvent(callback));
	}

	void Object::registerEvent(chstr name, Event* e)
	{
		this->unregisterEvent(name);
		if (e != NULL)
		{
			this->events[name] = e;
		}
	}

	void Object::unregisterEvent(chstr name)
	{
		if (this->events.has_key(name))
		{
			Event* event = this->events[name];
			if (this->dataset != NULL)
			{
				this->dataset->removeCallbackFromQueue(event);
			}
			delete event;
			this->events.remove_key(name);
		}
	}

	// TODO - this needs to be seriously refactored
	bool Object::triggerEvent(chstr name, april::Key keyCode, chstr extra)
	{
		if (this->events.has_key(name))
		{
			gvec2 cursorPosition = aprilui::getCursorPosition();
			EventArgs* args = new EventArgs(this, cursorPosition.x, cursorPosition.y, keyCode, extra);
			this->dataset->queueCallback(this->events[name], args);
			return true;
		}
		return false;
	}

	// TODO - this needs to be seriously refactored
	bool Object::triggerEvent(chstr name, april::Button buttonCode, chstr extra)
	{
		if (this->events.has_key(name))
		{
			gvec2 cursorPosition = aprilui::getCursorPosition();
			EventArgs* args = new EventArgs(this, cursorPosition.x, cursorPosition.y, buttonCode, extra);
			this->dataset->queueCallback(this->events[name], args);
			return true;
		}
		return false;
	}

	// TODO - this needs to be seriously refactored
	bool Object::triggerEvent(chstr name, float x, float y, april::Key keyCode, chstr extra)
	{
		if (this->events.has_key(name))
		{
			EventArgs* args = new EventArgs(this, x, y, keyCode, extra);
			this->dataset->queueCallback(this->events[name], args);
			return true;
		}
		return false;
	}

	void Object::resetCenter()
	{
		this->center = this->rect.getSize() / 2;
	}

	bool Object::isDerivedEnabled()
	{
		return (this->isEnabled() && (this->parent == NULL || this->parent->isDerivedEnabled()));
	}
	
	bool Object::isDerivedVisible()
	{
		return (this->isVisible() && (this->parent == NULL || this->parent->isDerivedVisible()));
	}
	
	bool Object::_isDerivedClickThrough()
	{
		return (this->isClickThrough() && (this->parent == NULL || this->parent->_isDerivedClickThrough()));
	}
	
	void Object::setEnabled(bool value)
	{
		if (value != this->enabled)
		{
			this->enabled = value;
			this->notifyEvent("OnEnableChanged", NULL);
		}
	}

	hstr Object::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "rect")					return grect_to_hstr(this->getRect());
		if (name == "position")				return gvec2_to_hstr(this->getPosition());
		if (name == "size")					return gvec2_to_hstr(this->getSize());
		if (name == "x")					return this->getX();
		if (name == "y")					return this->getY();
		if (name == "w")					return this->getWidth();
		if (name == "h")					return this->getHeight();
		if (name == "visible")				return this->isVisible();
		if (name == "zorder")				return this->getZOrder();
		if (name == "enabled")				return this->isEnabled();
		if (name == "click_through")		return this->isClickThrough();
		if (name == "inherit_alpha")		return this->isInheritAlpha();
		if (name == "red")					return this->getRed();
		if (name == "green")				return this->getGreen();
		if (name == "blue")					return this->getBlue();
		if (name == "alpha")				return this->getAlpha();
		if (name == "color")				return this->getColor().hex();
		if (name == "angle")				return this->getAngle();
		if (name == "scale")				return gvec2_to_hstr(this->getScale());
		if (name == "scale_x")				return this->getScaleX();
		if (name == "scale_y")				return this->getScaleY();
		if (name == "center")				return gvec2_to_hstr(this->getCenter());
		if (name == "center_x")				return this->getCenterX();
		if (name == "center_y")				return this->getCenterY();
		if (name == "anchor_left")			return this->isAnchorLeft();
		if (name == "anchor_right")			return this->isAnchorRight();
		if (name == "anchor_top")			return this->isAnchorTop();
		if (name == "anchor_bottom")		return this->isAnchorBottom();
		if (name == "retain_anchor_aspect")	return this->isRetainAnchorAspect();
		if (name == "clip")					return this->isClip();
		if (name == "use_disabled_alpha")	return this->isUseDisabledAlpha();
		if (name == "focus_index")			return this->getFocusIndex();
		if (name == "name")					return this->getName();
		if (name == "full_name")			return this->getFullName();
		if (propertyExists != NULL)
		{
			*propertyExists = false;
		}
		return "";
	}
	
	bool Object::setProperty(chstr name, chstr value)
	{
		if		(name == "rect")					this->setRect(hstr_to_grect(value));
		else if	(name == "position")				this->setPosition(hstr_to_gvec2(value));
		else if	(name == "size")					this->setSize(hstr_to_gvec2(value));
		else if	(name == "x")						this->setX(value);
		else if	(name == "y")						this->setY(value);
		else if	(name == "w")						this->setWidth(value);
		else if	(name == "h")						this->setHeight(value);
		else if	(name == "visible")					this->setVisible(value);
		else if	(name == "zorder")					this->setZOrder(value);
		else if	(name == "enabled")					this->setEnabled(value);
		else if	(name == "click_through")			this->setClickThrough(value);
		else if	(name == "inherit_alpha")			this->setInheritAlpha(value);
		else if	(name == "red")						this->setRed((int)value);
		else if	(name == "green")					this->setGreen((int)value);
		else if	(name == "blue")					this->setBlue((int)value);
		else if	(name == "alpha")					this->setAlpha((int)value);
		else if	(name == "color")					this->setColor(value);
		else if	(name == "angle")					this->setAngle(value);
		else if	(name == "scale")					this->setScale(hstr_to_gvec2(value));
		else if	(name == "scale_x")					this->setScaleX(value);
		else if	(name == "scale_y")					this->setScaleY(value);
		else if	(name == "center")					this->setCenter(hstr_to_gvec2(value));
		else if	(name == "center_x")				this->setCenterX(value);
		else if	(name == "center_y")				this->setCenterY(value);
		else if	(name == "anchor_left")				this->setAnchorLeft(value);
		else if	(name == "anchor_right")			this->setAnchorRight(value);
		else if	(name == "anchor_top")				this->setAnchorTop(value);
		else if	(name == "anchor_bottom")			this->setAnchorBottom(value);
		else if	(name == "retain_anchor_aspect")	this->setRetainAnchorAspect(value);
		else if	(name == "anchors")
		{
			harray<hstr> anchors = value.replace(" ", "").lower().split(",", -1, true);
			this->setAnchorLeft(anchors.contains("left"));
			this->setAnchorRight(anchors.contains("right"));
			this->setAnchorTop(anchors.contains("top"));
			this->setAnchorBottom(anchors.contains("bottom"));
			anchors.remove_all("left");
			anchors.remove_all("right");
			anchors.remove_all("top");
			anchors.remove_all("bottom");
			if (anchors.size() > 0)
			{
				hlog::warn(aprilui::logTag, "'anchors=' does not support values '" + anchors.join(",") + "'.");
				return false;
			}
		}
		else if	(name == "clip")					this->setClip(value);
		else if	(name == "use_disabled_alpha")		this->setUseDisabledAlpha(value);
		else if	(name == "focus_index")				this->setFocusIndex(value);
		else return false;
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
		if (!this->isVisible() || this->isClickThrough())
		{
			return NULL;
		}
		Object* object = NULL;
		foreach_r (Object*, it, this->children)
		{
			object = (*it)->getChildUnderPoint(pos);
			if (object != NULL && dynamic_cast<Animator*>(object) == NULL)
			{
				break;
			}
		}
		return (object == NULL && this->isPointInside(pos) ? this : object);
	}

	Object* Object::getChildUnderPoint(float x, float y)
	{
		return this->getChildUnderPoint(gvec2(x, y));
	}
	
	Object* Object::getChildUnderCursor()
	{
		if (!this->checkedChildUnderCursor)
		{
			this->childUnderCursor = this->getChildUnderPoint(aprilui::getCursorPosition());
			this->checkedChildUnderCursor = true;
		}
		return this->childUnderCursor;
	}
	
	void Object::clearChildUnderCursor()
	{
		this->childUnderCursor = NULL;
		this->checkedChildUnderCursor = false;
	}
	
	bool Object::isChild(Object* obj)
	{
		return (obj != NULL && obj->isParent(this));
	}
	
	bool Object::isDescendant(Object* obj)
	{
		return (obj != NULL && obj->isAncestor(this));
	}
	
	bool Object::isParent(Object* obj)
	{
		return (obj != NULL && obj == this->parent);
	}
	
	bool Object::isAncestor(Object* obj)
	{
		if (obj == NULL)
		{
			return false;
		}
		for (Object* o = this->getParent(); o != NULL; o = o->getParent())
		{
			if (o == obj)
			{
				return true;
			}
		}
		return false;
	}

	harray<Object*> Object::getAncestors()
	{
		harray<Object*> result;
		Object* parent = this->parent;
		while (parent != NULL)
		{
			result += parent;
			parent = parent->getParent();
		}
		return result;
	}

	harray<Object*> Object::getDescendants()
	{
		harray<Object*> descendants = this->children;
		foreach (Object*, it, this->children)
		{
			descendants += (*it)->getDescendants();
		}
		return descendants;
	}

	harray<gvec2> Object::transformToLocalSpace(harray<gvec2> points, aprilui::Object* overrideRoot)
	{
		harray<Object*> sequence;
		Object* current = this;
		while (current != NULL)
		{
			sequence += current;
			current = (overrideRoot == NULL || overrideRoot != current ? current->getParent() : NULL);
		}
		sequence.reverse();
		gvec2 center;
		gvec2 scale;
		gvec2 position;
		float angle;
		foreach (Object*, it, sequence)
		{
			center = (*it)->getCenter();
			scale = (*it)->getScale();
			position = (*it)->getPosition();
			angle = (*it)->getAngle();
			foreach (gvec2, it2, points)
			{
				(*it2) -= center + position;
				(*it2).rotate(angle);
				(*it2) /= scale;
				(*it2) += center;
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
			current = (overrideRoot == NULL || overrideRoot != current ? current->getParent() : NULL);
		}
		sequence.reverse();
		gvec2 center;
		foreach (Object*, it, sequence)
		{
			center = (*it)->getCenter();
			point -= center + (*it)->getPosition();
			point.rotate((*it)->getAngle());
			point /= (*it)->getScale();
			point += center;
		}
		return point;
	}

	harray<gvec2> Object::getDerivedPoints(harray<gvec2> points, aprilui::Object* overrideRoot)
	{
		Object* current = this;
		gvec2 center;
		gvec2 scale;
		gvec2 position;
		float angle;
		while (current != NULL)
		{
			center = current->getCenter();
			scale = current->getScale();
			position = current->getPosition();
			angle = current->getAngle();
			foreach (gvec2, it, points)
			{
				(*it) -= center;
				(*it) *= scale;
				(*it).rotate(-angle);
				(*it) += center + position;
			}
			current = (overrideRoot == NULL || overrideRoot != current ? current->getParent() : NULL);
		}
		return points;
	}

	gvec2 Object::getDerivedPoint(gvec2 point, aprilui::Object* overrideRoot)
	{
		Object* current = this;
		gvec2 center;
		while (current != NULL)
		{
			center = current->getCenter();
			point -= center;
			point *= current->getScale();
			point.rotate(-current->getAngle());
			point += center + current->getPosition();
			current = (overrideRoot == NULL || overrideRoot != current ? current->getParent() : NULL);
		}
		return point;
	}

	grect Object::getBoundingRect(aprilui::Object* overrideRoot)
	{
		gvec2 max;
		gvec2 min;
		harray<gvec2> corners = this->getDerivedCorners(overrideRoot);
		min = max = corners.remove_first(); // guaranteed to return 4 corner points previously
		gvec2 corner;
		while (corners.size() > 0)
		{
			corner = corners.remove_first();
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

	gvec2 Object::getDerivedCenter(aprilui::Object* overrideRoot)
	{
		return this->getDerivedPoint(this->center, overrideRoot);
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
	
	harray<Image*> Object::getUsedImages()
	{
		harray<Image*> array;
		return array;
	}
	
	grect Object::_getDrawRect()
	{
		return grect(-this->center, this->rect.getSize());
	}

	april::Color Object::_getDrawColor()
	{
		april::Color color = this->color;
		if (this->inheritAlpha)
		{
			color.a = this->getDerivedAlpha();
		}
		return color;
	}

	float Object::_getDisabledAlphaFactor()
	{
		return (this->useDisabledAlpha && !this->isDerivedEnabled() ? 0.5f : 1.0f);
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

	Animator* Object::moveCenterX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverX);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverX, this->center.x, x, speed);
		return animatorCenterMoverX;
	}

	Animator* Object::moveCenterY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverY);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverY, this->center.y, y, speed);
		return animatorCenterMoverY;
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

	void Object::moveCenter(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverX);
		REMOVE_EXISTING_ANIMATORS(CenterMoverY);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverX, this->center.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverY, this->center.y, y, speed);
	}

	void Object::moveCenter(gvec2 center, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverX);
		REMOVE_EXISTING_ANIMATORS(CenterMoverY);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverX, this->center.x, center.x, speed);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverY, this->center.y, center.y, speed);
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

	DEFINE_ANIMATOR_F(moveX, MoverX);
	DEFINE_ANIMATOR_F(moveY, MoverY);
	DEFINE_ANIMATOR_F(scaleX, ScalerX);
	DEFINE_ANIMATOR_F(scaleY, ScalerY);
	DEFINE_ANIMATOR_F(resizeX, ResizerX);
	DEFINE_ANIMATOR_F(resizeY, ResizerY);
	DEFINE_ANIMATOR_F(rotate, Rotator);
	DEFINE_ANIMATOR_F(moveCenterX, CenterMoverX);
	DEFINE_ANIMATOR_F(moveCenterY, CenterMoverY);
	DEFINE_ANIMATOR_F(fadeAlpha, AlphaChanger);
	DEFINE_ANIMATOR_F(fadeRed, RedChanger);
	DEFINE_ANIMATOR_F(fadeGreen, GreenChanger);
	DEFINE_ANIMATOR_F(fadeBlue, BlueChanger);
	
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

	Animator* Object::moveCenterXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverX, this->center.x, x, speed, delay);
		return animatorCenterMoverX;
	}

	Animator* Object::moveCenterYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverY, this->center.y, y, speed, delay);
		return animatorCenterMoverY;
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

	void Object::moveCenterQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverX, this->center.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverY, this->center.y, y, speed, delay);
	}

	void Object::moveCenterQueue(gvec2 center, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverX, this->center.x, center.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverY, this->center.y, center.y, speed, delay);
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
	
	DEFINE_ANIMATOR_F_DELAYED(moveX, MoverX);
	DEFINE_ANIMATOR_F_DELAYED(moveY, MoverY);
	DEFINE_ANIMATOR_F_DELAYED(scaleX, ScalerX);
	DEFINE_ANIMATOR_F_DELAYED(scaleY, ScalerY);
	DEFINE_ANIMATOR_F_DELAYED(resizeX, ResizerX);
	DEFINE_ANIMATOR_F_DELAYED(resizeY, ResizerY);
	DEFINE_ANIMATOR_F_DELAYED(rotate, Rotator);
	DEFINE_ANIMATOR_F_DELAYED(moveCenterX, CenterMoverX);
	DEFINE_ANIMATOR_F_DELAYED(moveCenterY, CenterMoverY);
	DEFINE_ANIMATOR_F_DELAYED(fadeAlpha, AlphaChanger);
	DEFINE_ANIMATOR_F_DELAYED(fadeRed, RedChanger);
	DEFINE_ANIMATOR_F_DELAYED(fadeGreen, GreenChanger);
	DEFINE_ANIMATOR_F_DELAYED(fadeBlue, BlueChanger);
	
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

	void Object::moveCenterXStop()
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverX);
	}

	void Object::moveCenterYStop()
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverY);
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

	void Object::moveCenterStop()
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverX);
		REMOVE_EXISTING_ANIMATORS(CenterMoverY);
	}

	void Object::fadeColorStop()
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
	}
	
}
