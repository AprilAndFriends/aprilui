/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
	while (i ## name < mDynamicAnimators.size()) \
	{ \
		removeAnimator ## name = dynamic_cast<Animators::name*>(mDynamicAnimators[i ## name]); \
		if (removeAnimator ## name != NULL) \
		{ \
			delete removeAnimator ## name; \
			this->mDynamicAnimators.remove_at(i ## name); \
		} \
		else \
		{ \
			(i ## name)++; \
		} \
	}

#define CREATE_DYNAMIC_ANIMATOR(type, offset, target, speed) \
	CREATE_DELAYED_DYNAMIC_ANIMATOR(type, offset, target, speed, 0.0f);

#define CREATE_DELAYED_DYNAMIC_ANIMATOR(type, offset, target, speed, delay) \
	Animator* animator ## type = new Animators::type(generateName("dynamic_animator_")); \
	this->mDynamicAnimators += animator ## type; \
	animator ## type->_setParent(this); \
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
	} \
	animator ## type->setSpeed(speed); \
	animator ## type->setPeriods(1.0f);

namespace aprilui
{
	bool _objectSortCallback(Object* a, Object* b)
	{
		return (a->getZOrder() < b->getZOrder());
	}

	Object::Object(chstr name, grect rect) : EventReceiver()
	{
		this->mName = name;
		this->mRect = rect;
		if (this->mRect.w != -1)
		{
			this->mCenter.x = this->mRect.w * 0.5f;
		}
		if (this->mRect.h != -1)
		{
			this->mCenter.y = this->mRect.h* 0.5f;
		}
		this->mScale.set(1.0f, 1.0f);
		this->mParent = NULL;
		this->mChildUnderCursor = NULL;
		this->mCheckedChildUnderCursor = false;
		this->mDataset = NULL;
		this->mZOrder = 0;
		this->mEnabled = true;
		this->mVisible = true;
		this->mAngle = 0.0f;
		this->mClickThrough = false;
		this->mInheritsAlpha = true;
		this->mAnchorLeft = true;
		this->mAnchorRight = false;
		this->mAnchorTop = true;
		this->mAnchorBottom = false;
		this->mRetainAnchorAspect = false;
		this->mFocusIndex = -1;
		this->mClip = false;
		this->mUseDisabledAlpha = true;
	}

	Object::~Object()
	{
		foreach_m (Event*, it, this->mEvents)
		{
			delete it->second;
		}
		this->mEvents.clear();
		foreach (Animator*, it, this->mDynamicAnimators)
		{
			delete (*it);
		}
		this->mDynamicAnimators.clear();
		
		if (this->isFocused())
		{
			this->setFocused(false);
		}
	}
	
	void Object::_sortChildren()
	{
		this->mChildren.sort(&_objectSortCallback);
	}

	hstr Object::getFullName()
	{
		return ((this->mDataset != NULL ? this->mDataset->getName() + "." : "") + this->mName);
	}

	void Object::addChild(Object* object)
	{
		if (object->getParent())
		{
			throw ObjectHasParentException(object->getName(), this->getName());
		}
		this->mChildren += object;
		this->_sortChildren();
		object->_setParent(this);
		object->notifyEvent("AttachToObject", NULL);
	}
	
	void Object::removeChild(Object* object)
	{
		if (object->getParent() != this)
		{
			throw ObjectNotChildException(object->getName(), this->getName());
		}
		object->notifyEvent("DetachFromObject", NULL);
		this->mChildren -= object;
		object->_setParent(NULL);
	}

	void Object::registerChild(Object* object)
	{
		this->addChild(object);
		this->mDataset->registerManualObject(object);
	}
	
	void Object::unregisterChild(Object* object)
	{
		this->removeChild(object);
		this->mDataset->unregisterManualObject(object);
	}

	void Object::removeChildren(bool recursive)
	{
		if (recursive)
		{
			foreach (Object*, it, this->mChildren)
			{
				(*it)->removeChildren(recursive);
			}
		}
		foreach (Object*, it, this->mChildren)
		{
			(*it)->_setParent(NULL);
		}
		this->mChildren.clear();
	}
	
	void Object::destroyChildren()
	{
		while (this->mChildren.size() > 0)
		{
			this->mDataset->destroyObject(this->mChildren.first());
		}
	}
	
	void Object::attach(Object* object)
	{
		object->addChild(this);
	}
	
	void Object::detach()
	{
		if (this->mParent == NULL)
		{
			throw ObjectWithoutParentException(this->getName());
		}
		this->mParent->removeChild(this);
	}
	
	Object* Object::findChildByName(chstr name)
	{
		foreach (Object*, it, this->mChildren)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}
	
	void Object::setZOrder(int zorder)
	{
		if (this->mZOrder != zorder)
		{
			this->mZOrder = zorder;
			if (this->mParent != NULL)
			{
				this->mParent->_sortChildren();
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
		foreach (Object*, it, this->mChildren)
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
		foreach (Object*, it, this->mChildren)
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
		this->_updateChildrenHorizontal(value - this->mRect.w);
		this->mRect.w = value;
		this->notifyEvent("Resized", NULL);
	}

	void Object::setHeight(float value)
	{
		this->_updateChildrenVertical(value - this->mRect.h);
		this->mRect.h = value;
		this->notifyEvent("Resized", NULL);
	}

	void Object::setSize(gvec2 value)
	{
		this->_updateChildrenHorizontal(value.x - this->mRect.w);
		this->_updateChildrenVertical(value.y - this->mRect.h);
		this->mRect.setSize(value);
		this->notifyEvent("Resized", NULL);
	}

	void Object::setSize(float w, float h)
	{
		this->_updateChildrenHorizontal(w - this->mRect.w);
		this->_updateChildrenVertical(h - this->mRect.h);
		this->mRect.w = w;
		this->mRect.h = h;
		this->notifyEvent("Resized", NULL);
	}

	void Object::setRect(grect value)
	{
		this->_updateChildrenHorizontal(value.w - this->mRect.w);
		this->_updateChildrenVertical(value.h - this->mRect.h);
		this->mRect = value;
		this->notifyEvent("Resized", NULL);
	}

	unsigned char Object::getDerivedAlpha(aprilui::Object* overrideRoot)
	{
		// recursive function that combines all the alpha from the parents (if any)
		float factor = 1.0f;
		if (this->mInheritsAlpha && this->mParent != NULL && this->mParent != overrideRoot)
		{
			factor *= this->mParent->getDerivedAlpha(overrideRoot) / 255.0f;
		}
		return (unsigned char)(this->getAlpha() * factor);
	}

	float Object::_getDerivedAngle(aprilui::Object* overrideRoot)
	{
		float angle = this->mAngle;
		if (this->mParent != overrideRoot)
		{
			angle += this->mParent->_getDerivedAngle(overrideRoot);
		}
		return angle;
	}

	bool Object::isAnimated()
	{
		foreach (Animator*, it, this->mDynamicAnimators)
		{
			if ((*it)->isAnimated())
			{
				return true;
			}
		}
		foreach (Object*, it, this->mChildren)
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
		foreach (Animator*, it, this->mDynamicAnimators)
		{
			if ((*it)->isWaitingAnimation())
			{
				return true;
			}
		}
		foreach (Object*, it, this->mChildren)
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
		return (this->mDynamicAnimators.size() > 0);
	}

	void Object::draw()
	{
		if (!this->isVisible() || heqf(this->mScale.x, 0.0f, aprilui::eTolerance) || heqf(this->mScale.y, 0.0f, aprilui::eTolerance))
		{
			return;
		}
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		grect viewport;
		grect orthoProjection;
		bool clipped = (this->mClip && this->mParent != NULL);
		if (clipped)
		{
			// TODO - this has to be implemented using clipping planes in April
			orthoProjection = april::rendersys->getOrthoProjection();
			viewport = april::rendersys->getViewport();
			grect rect = this->mParent->getBoundingRect();
			gvec2 ratio = orthoProjection.getSize() / viewport.getSize();
			april::rendersys->setOrthoProjection(grect(-rect.getPosition(), rect.getSize()));
			april::rendersys->setViewport(grect((rect.getPosition() + orthoProjection.getPosition()) / ratio, rect.getSize() / ratio));
		}
		gvec2 position = this->mRect.getPosition() + this->mCenter;
		if (position.x != 0.0f || position.y != 0.0f)
		{
			april::rendersys->translate(position.x, position.y);
		}
		if (this->mAngle != 0.0f)
		{
			april::rendersys->rotate(this->mAngle);
		}
		if (this->mScale.x != 1.0f || this->mScale.y != 1.0f)
		{
			april::rendersys->scale(this->mScale.x, this->mScale.y, 1.0f);
		}
		this->OnDraw();
		if (aprilui::isDebugEnabled())
		{
			this->OnDrawDebug();
		}
		if (this->mCenter.x != 0.0f || this->mCenter.y != 0.0f)
		{
			april::rendersys->translate(-this->mCenter.x, -this->mCenter.y);
		}
		foreach (Object*, it, mChildren)
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
	
	void Object::update(float k)
	{
		if (this->mCheckedChildUnderCursor)
		{
			this->clearChildUnderCursor();
		}
		foreach (Object*, it, this->mChildren)
		{
			(*it)->update(k);
		}
		foreach (Animator*, it, this->mDynamicAnimators)
		{
			(*it)->update(k);
		}
		// faster than creating a new array, adding expired animators and the deleting them
		int i = 0;
		Animator* animator = NULL;
		while (i < this->mDynamicAnimators.size())
		{
			animator = this->mDynamicAnimators[i];
			if (animator->isExpired())
			{
				delete animator;
				this->mDynamicAnimators.remove_at(i);
			}
			else
			{
				i++;
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
		if (heqf(this->mScale.x, 0.0f, aprilui::eTolerance) || heqf(this->mScale.y, 0.0f, aprilui::eTolerance))
		{
			return false;
		}
		if (this->mParent != NULL)
		{
			Object* obj = this;
			Object* parent = this->mParent;
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
		return grect(0.0f, 0.0f, this->mRect.getSize()).isPointInside(this->transformToLocalSpace(position));
	}

	bool Object::onMouseDown(april::Key keyCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (this->mClickThrough || !this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		if (this->mDataset != NULL)
		{
			this->mDataset->removeFocus();
		}
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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
		if (this->mClickThrough || !this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		harray<Object*> validObjects;
		Object* object = NULL;
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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
				(*it)->cancelMouseDown();
			}
            // TODO
            // this object also has to process their cancelMouseDown, but without sending it to its children
            // this needs to be removed and fixed upon the next major system refactoring
            validObjects = this->mChildren;
            this->mChildren.clear();
            this->cancelMouseDown();
            this->mChildren = validObjects;
			return true;
		}
        
		return false;
	}

	bool Object::onMouseMove()
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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

	bool Object::onButtonDown(april::Button buttonCode)
	{
		// this check is important when the object is directly accessed for processing (might be refactored in the future)
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
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
	
	void Object::cancelMouseDown()
	{
		// needs to be copied in case mChildren gets changed
		harray<Object*> children = this->mChildren;
		foreach_r (Object*, it, children)
		{
			(*it)->cancelMouseDown();
		}
	}

	void Object::registerEvent(chstr name, void (*callback)(EventArgs*))
	{
		this->registerEvent(name, new CallbackEvent(callback));
	}

	void Object::registerEvent(chstr name, Event* e)
	{
		if (this->mEvents.has_key(name))
		{
			Event* oldEvent = this->mEvents[name];
			if (e == NULL)
			{
				this->mEvents.remove_key(name);
			}
			delete oldEvent;
		}
		if (e != NULL)
		{
			this->mEvents[name] = e;
		}
	}

	void Object::unregisterEvent(chstr name)
	{
		this->mEvents.remove_key(name);
	}

	// TODO - this needs to be seriously refactored
	bool Object::triggerEvent(chstr name, april::Key keyCode, chstr extra)
	{
		if (this->mEvents.has_key(name))
		{
			gvec2 cursorPosition = aprilui::getCursorPosition();
			EventArgs args(this, cursorPosition.x, cursorPosition.y, keyCode, extra);
			this->mEvents[name]->execute(&args);
			return true;
		}
		return false;
	}

	// TODO - this needs to be seriously refactored
	bool Object::triggerEvent(chstr name, april::Button buttonCode, chstr extra)
	{
		if (this->mEvents.has_key(name))
		{
			gvec2 cursorPosition = aprilui::getCursorPosition();
			EventArgs args(this, cursorPosition.x, cursorPosition.y, buttonCode, extra);
			this->mEvents[name]->execute(&args);
			return true;
		}
		return false;
	}

	// TODO - this needs to be seriously refactored
	bool Object::triggerEvent(chstr name, float x, float y, april::Key keyCode, chstr extra)
	{
		if (this->mEvents.has_key(name))
		{
			EventArgs args(this, x, y, keyCode, extra);
			this->mEvents[name]->execute(&args);
			return true;
		}
		return false;
	}

	void Object::resetCenter()
	{
		this->mCenter = this->mRect.getSize() / 2;
	}

	bool Object::isDerivedEnabled()
	{
		return (this->isEnabled() && (this->mParent == NULL || this->mParent->isDerivedEnabled()));
	}
	
	bool Object::isDerivedVisible()
	{
		return (this->isVisible() && (this->mParent == NULL || this->mParent->isDerivedVisible()));
	}
	
	bool Object::_isDerivedClickThrough()
	{
		return (this->isClickThrough() && (this->mParent == NULL || this->mParent->_isDerivedClickThrough()));
	}
	
	void Object::setAlpha(unsigned char value)
	{
		this->mColor.a = value;
	}

	hstr Object::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "x")					return this->getX();
		if (name == "y")					return this->getY();
		if (name == "w")					return this->getWidth();
		if (name == "h")					return this->getHeight();
		if (name == "visible")				return this->isVisible();
		if (name == "zorder")				return this->getZOrder();
		if (name == "enabled")				return this->isEnabled();
		if (name == "click_through")		return this->isClickThrough();
		if (name == "inherits_alpha")		return this->isInheritsAlpha();
		if (name == "red")					return this->getRed();
		if (name == "green")				return this->getGreen();
		if (name == "blue")					return this->getBlue();
		if (name == "alpha")				return this->getAlpha();
		if (name == "color")				return this->getColor().hex();
		if (name == "angle")				return this->getAngle();
		if (name == "scale_x")				return this->getScaleX();
		if (name == "scale_y")				return this->getScaleY();
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
		if		(name == "x")						this->setX(value);
		else if	(name == "y")						this->setY(value);
		else if	(name == "w")						this->setWidth(value);
		else if	(name == "h")						this->setHeight(value);
		else if	(name == "visible")					this->setVisible(value);
		else if	(name == "zorder")					this->setZOrder(value);
		else if	(name == "enabled")					this->setEnabled(value);
		else if	(name == "click_through")			this->setClickThrough(value);
		else if	(name == "inherits_alpha")			this->setInheritsAlpha(value);
		else if	(name == "red")						this->setRed((int)value);
		else if	(name == "green")					this->setGreen((int)value);
		else if	(name == "blue")					this->setBlue((int)value);
		else if	(name == "alpha")					this->setAlpha((int)value);
		else if	(name == "color")					this->setColor(value);
		else if	(name == "angle")					this->setAngle(value);
		else if	(name == "scale_x")					this->setScaleX(value);
		else if	(name == "scale_y")					this->setScaleY(value);
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
		float s2 = (float)dsin(mAngle);
		float c2 = (float)dcos(mAngle);
		return (heqf(s1, s2, (float)HL_E_TOLERANCE) && heqf(c1, c2, (float)HL_E_TOLERANCE));
	}

	Object* Object::getChildByName(chstr name, bool recursive)
	{
		foreach (Object*, it, this->mChildren)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		if (recursive)
		{
			Object* object;
			foreach (Object*, it, this->mChildren)
			{
				object = (*it)->getChildByName(name, recursive);
				if (object != NULL)
				{
					return object;
				}
			}
		}
		return NULL;
	}

	
	Object* Object::getChildUnderPoint(gvec2 pos)
	{
		if (!this->isVisible() || this->isClickThrough())
		{
			return NULL;
		}
		Object* object = NULL;
		foreach_r (Object*, it, this->mChildren)
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
		if (!this->mCheckedChildUnderCursor)
		{
			this->mChildUnderCursor = this->getChildUnderPoint(aprilui::getCursorPosition());
			this->mCheckedChildUnderCursor = true;
		}
		return this->mChildUnderCursor;
	}
	
	void Object::clearChildUnderCursor()
	{
		this->mChildUnderCursor = NULL;
		this->mCheckedChildUnderCursor = false;
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
		return (obj != NULL && obj == this->mParent);
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
		Object* parent = this->mParent;
		while (parent != NULL)
		{
			result += parent;
			parent = parent->getParent();
		}
		return result;
	}

	harray<Object*> Object::getDescendants()
	{
		harray<Object*> descendants = this->mChildren;
		foreach (Object*, it, this->mChildren)
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
		return (this->mDataset != NULL && this->mDataset->getFocusedObject() == this);
	}
	
	void Object::setFocused(bool focused)
	{
		if (this->mDataset != NULL)
		{
			if (focused)
			{
				this->mDataset->focus(this);
			}
			else
			{
				this->mDataset->removeFocus();
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
		points += gvec2(0.0f, this->mRect.h);
		points += gvec2(this->mRect.w, 0.0f);
		points += gvec2(this->mRect.w, this->mRect.h);
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
		return this->getDerivedPoint(this->mCenter, overrideRoot);
	}
	
	gvec2 Object::getDerivedScale(aprilui::Object* overrideRoot)
	{
		gvec2 scale = this->mScale;
		if (this->mParent != overrideRoot)
		{
			scale *= this->mParent->getDerivedScale(overrideRoot);
		}
		return scale;
	}
	
	grect Object::_getDrawRect()
	{
		return grect(-this->mCenter, this->mRect.getSize());
	}

	april::Color Object::_getDrawColor()
	{
		april::Color color = this->mColor;
		if (this->mInheritsAlpha)
		{
			color.a = this->getDerivedAlpha();
		}
		return color;
	}

	float Object::_getDisabledAlphaFactor()
	{
		return (this->mUseDisabledAlpha && !this->isDerivedEnabled() ? 0.5f : 1.0f);
	}

	Animator* Object::moveX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		CREATE_DYNAMIC_ANIMATOR(MoverX, this->mRect.x, x, speed);
		return animatorMoverX;
	}

	Animator* Object::moveY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverY, this->mRect.y, y, speed);
		return animatorMoverY;
	}

	Animator* Object::scaleX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, this->mScale.x, x, speed);
		return animatorScalerX;
	}

	Animator* Object::scaleY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, this->mScale.y, y, speed);
		return animatorScalerY;
	}

	Animator* Object::resizeX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, this->mRect.w, x, speed);
		return animatorResizerX;
	}

	Animator* Object::resizeY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, this->mRect.h, y, speed);
		return animatorResizerY;
	}

	Animator* Object::rotate(float angle, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(Rotator);
		CREATE_DYNAMIC_ANIMATOR(Rotator, this->mAngle, angle, speed);
		return animatorRotator;
	}

	Animator* Object::moveCenterX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverX);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverX, this->mCenter.x, x, speed);
		return animatorCenterMoverX;
	}

	Animator* Object::moveCenterY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverY);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverY, this->mCenter.y, y, speed);
		return animatorCenterMoverY;
	}

	Animator* Object::fadeRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)this->mColor.r, (float)r, speed);
		return animatorRedChanger;
	}

	Animator* Object::fadeGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)this->mColor.g, (float)g, speed);
		return animatorGreenChanger;
	}

	Animator* Object::fadeBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)this->mColor.b, (float)b, speed);
		return animatorBlueChanger;
	}

	Animator* Object::fadeAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->mColor.a, (float)a, speed);
		return animatorAlphaChanger;
	}

	void Object::move(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverX, this->mRect.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(MoverY, this->mRect.y, y, speed);
	}

	void Object::move(gvec2 position, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverX, this->mRect.x, position.x, speed);
		CREATE_DYNAMIC_ANIMATOR(MoverY, this->mRect.y, position.y, speed);
	}

	void Object::scale(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, this->mScale.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, this->mScale.y, y, speed);
	}

	void Object::scale(gvec2 scale, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, this->mScale.x, scale.x, speed);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, this->mScale.y, scale.y, speed);
	}

	void Object::resize(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, this->mRect.w, x, speed);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, this->mRect.h, y, speed);
	}

	void Object::resize(gvec2 size, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, this->mRect.w, size.x, speed);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, this->mRect.h, size.y, speed);
	}

	void Object::moveCenter(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverX);
		REMOVE_EXISTING_ANIMATORS(CenterMoverY);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverX, this->mCenter.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverY, this->mCenter.y, y, speed);
	}

	void Object::moveCenter(gvec2 center, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(CenterMoverX);
		REMOVE_EXISTING_ANIMATORS(CenterMoverY);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverX, this->mCenter.x, center.x, speed);
		CREATE_DYNAMIC_ANIMATOR(CenterMoverY, this->mCenter.y, center.y, speed);
	}

	void Object::fadeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)this->mColor.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)this->mColor.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)this->mColor.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->mColor.a, (float)a, speed);
	}
	
	void Object::fadeColor(april::Color color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)this->mColor.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)this->mColor.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)this->mColor.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->mColor.a, (float)color.a, speed);
	}
	
	Animator* Object::moveXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, this->mRect.x, x, speed, delay);
		return animatorMoverX;
	}

	Animator* Object::moveYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, this->mRect.y, y, speed, delay);
		return animatorMoverY;
	}

	Animator* Object::scaleXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, this->mScale.x, x, speed, delay);
		return animatorScalerX;
	}

	Animator* Object::scaleYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, this->mScale.y, y, speed, delay);
		return animatorScalerY;
	}

	Animator* Object::resizeXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, this->mRect.w, x, speed, delay);
		return animatorResizerX;
	}

	Animator* Object::resizeYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, this->mRect.h, y, speed, delay);
		return animatorResizerY;
	}

	Animator* Object::rotateQueue(float angle, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(Rotator, this->mAngle, angle, speed, delay);
		return animatorRotator;
	}

	Animator* Object::moveCenterXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverX, this->mCenter.x, x, speed, delay);
		return animatorCenterMoverX;
	}

	Animator* Object::moveCenterYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverY, this->mCenter.y, y, speed, delay);
		return animatorCenterMoverY;
	}

	Animator* Object::fadeRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)this->mColor.r, (float)r, speed, delay);
		return animatorRedChanger;
	}

	Animator* Object::fadeGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)this->mColor.g, (float)g, speed, delay);
		return animatorGreenChanger;
	}

	Animator* Object::fadeBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)this->mColor.b, (float)b, speed, delay);
		return animatorBlueChanger;
	}

	Animator* Object::fadeAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->mColor.a, (float)a, speed, delay);
		return animatorAlphaChanger;
	}

	void Object::moveQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, this->mRect.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, this->mRect.y, y, speed, delay);
	}

	void Object::moveQueue(gvec2 position, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, this->mRect.x, position.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, this->mRect.y, position.y, speed, delay);
	}

	void Object::scaleQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, this->mScale.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, this->mScale.y, y, speed, delay);
	}

	void Object::scaleQueue(gvec2 scale, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, this->mScale.x, scale.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, this->mScale.y, scale.y, speed, delay);
	}

	void Object::resizeQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, this->mRect.w, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, this->mRect.h, y, speed, delay);
	}

	void Object::resizeQueue(gvec2 size, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, this->mRect.w, size.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, this->mRect.h, size.y, speed, delay);
	}

	void Object::moveCenterQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverX, this->mCenter.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverY, this->mCenter.y, y, speed, delay);
	}

	void Object::moveCenterQueue(gvec2 center, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverX, this->mCenter.x, center.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(CenterMoverY, this->mCenter.y, center.y, speed, delay);
	}

	void Object::fadeColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)this->mColor.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)this->mColor.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)this->mColor.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->mColor.a, (float)a, speed, delay);
	}

	void Object::fadeColorQueue(april::Color color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)this->mColor.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)this->mColor.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)this->mColor.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)this->mColor.a, (float)color.a, speed, delay);
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
