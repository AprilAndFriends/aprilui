/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
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
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Animator.h"
#include "AnimatorAlphaChanger.h"
#include "AnimatorBlueChanger.h"
#include "AnimatorGreenChanger.h"
#include "AnimatorMoverX.h"
#include "AnimatorMoverY.h"
#include "AnimatorRedChanger.h"
#include "AnimatorResizerX.h"
#include "AnimatorResizerY.h"
#include "AnimatorRotator.h"
#include "AnimatorScalerX.h"
#include "AnimatorScalerY.h"
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
			mDynamicAnimators.remove_at(i ## name); \
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
	mDynamicAnimators += animator ## type; \
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
	Object::Object(chstr name, grect rect)
	{
		mName = name;
		mRect = rect;
		if (mRect.w != -1)
		{
			mCenter.x = mRect.w / 2;
		}
		if (mRect.h != -1)
		{
			mCenter.y = mRect.h / 2;
		}
		mScale = gvec2(1.0f, 1.0f);
		mParent = NULL;
		mChildUnderCursor = NULL;
		mCheckedChildUnderCursor = false;
		mDataset = NULL;
		mZOrder = 0;
		mEnabled = true;
		mVisible = true;
		mAngle = 0.0f;
		mClickThrough = false;
		mInheritsAlpha = true;
		mAnchorLeft = true;
		mAnchorRight = false;
		mAnchorTop = true;
		mAnchorBottom = false;
		mClip = false;
	}

	Object::~Object()
	{
		foreach_m (Event*, it, mEvents)
		{
			delete it->second;
		}
		mEvents.clear();
		foreach (Animator*, it, mDynamicAnimators)
		{
			delete (*it);
		}
		mDynamicAnimators.clear();
	}
	
	bool _objectSortCallback(Object* a, Object* b)
	{
		return (a->getZOrder() < b->getZOrder());
	}

	hstr Object::getFullName()
	{
		return ((mDataset != NULL ? mDataset->getName() + "." : "") + mName);
	}

	void Object::_sortChildren()
	{
		mChildren.sort(&_objectSortCallback);
	}

	void Object::addChild(Object* object)
	{
		if (object->getParent())
		{
			throw ObjectHasParentException(object->getName(), getName());
		}
		mChildren += object;
		_sortChildren();
		object->_setParent(this);
		object->notifyEvent("AttachToObject", NULL);
	}
	
	void Object::removeChild(Object* object)
	{
		if (object->getParent() != this)
		{
			throw ObjectNotChildException(object->getName(), getName());
		}
		object->notifyEvent("DetachFromObject", NULL);
		mChildren -= object;
		object->_setParent(NULL);
	}

	void Object::registerChild(Object* object)
	{
		addChild(object);
		mDataset->registerManualObject(object);
	}
	
	void Object::unregisterChild(Object* object)
	{
		removeChild(object);
		mDataset->unregisterManualObject(object);
	}

	void Object::removeChildren(bool recursive)
	{
		if (recursive)
		{
			foreach (Object*, it, mChildren)
			{
				(*it)->removeChildren(recursive);
			}
		}
		foreach (Object*, it, mChildren)
		{
			(*it)->_setParent(NULL);
		}
		mChildren.clear();
	}
	
	void Object::destroyChildren(bool recursive)
	{
		while (mChildren.size() > 0)
		{
			mDataset->destroyObject(mChildren[0], recursive);
		}
	}
	
	void Object::attach(Object* object)
	{
		object->addChild(this);
	}
	
	void Object::detach()
	{
		if (mParent == NULL)
		{
			throw ObjectWithoutParentException(getName());
		}
		mParent->removeChild(this);
	}
	
	Object* Object::findChildByName(chstr name)
	{
		foreach (Object*, it, mChildren)
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
		if (mZOrder != zorder)
		{
			mZOrder = zorder;
			if (mParent != NULL)
			{
				mParent->_sortChildren();
			}
		}
	}

	void Object::_updateChildrenHorizontal(float difference)
	{
		float width;
		foreach (Object*, it, mChildren)
		{
			if (!(*it)->isAnchorLeft())
			{
				if ((*it)->isAnchorRight())
				{
					(*it)->setX((*it)->getX() + difference);
				}
				else
				{
					(*it)->setX((*it)->getX() + difference / 2);
				}
			}
			else if ((*it)->isAnchorRight())
			{
				width = (*it)->getWidth();
				(*it)->setWidth(width + difference);
				if (width != 0.0f)
				{
					(*it)->setCenterX((*it)->getCenterX() * (width + difference) / width);
				}
			}
		}
	}

	void Object::_updateChildrenVertical(float difference)
	{
		float height;
		foreach (Object*, it, mChildren)
		{
			if (!(*it)->isAnchorTop())
			{
				if ((*it)->isAnchorBottom())
				{
					(*it)->setY((*it)->getY() + difference);
				}
				else
				{
					(*it)->setY((*it)->getY() + difference / 2);
				}
			}
			else if ((*it)->isAnchorBottom())
			{
				height = (*it)->getHeight();
				(*it)->setHeight((*it)->getHeight() + difference);
				if (height != 0.0f)
				{
					(*it)->setCenterY((*it)->getCenterY() * (height + difference) / height);
				}
			}
		}
	}

	void Object::setWidth(float value)
	{
		_updateChildrenHorizontal(value - mRect.w);
		mRect.w = value;
		notifyEvent("Resized", NULL);
	}

	void Object::setHeight(float value)
	{
		_updateChildrenVertical(value - mRect.h);
		mRect.h = value;
		notifyEvent("Resized", NULL);
	}

	void Object::setSize(gvec2 value)
	{
		_updateChildrenHorizontal(value.x - mRect.w);
		_updateChildrenVertical(value.y - mRect.h);
		mRect.setSize(value);
		notifyEvent("Resized", NULL);
	}

	void Object::setSize(float w, float h)
	{
		_updateChildrenHorizontal(w - mRect.w);
		_updateChildrenVertical(h - mRect.h);
		mRect.w = w;
		mRect.h = h;
		notifyEvent("Resized", NULL);
	}

	void Object::setRect(grect value)
	{
		_updateChildrenHorizontal(value.w - mRect.w);
		_updateChildrenVertical(value.h - mRect.h);
		mRect = value;
		notifyEvent("Resized", NULL);
	}

	unsigned char Object::getDerivedAlpha(aprilui::Object* overrideRoot)
	{
		// recursive function that combines all the alpha from the parents (if any)
		float factor = 1.0f;
		if (mInheritsAlpha && mParent != overrideRoot)
		{
			factor *= mParent->getDerivedAlpha(overrideRoot) / 255.0f;
		}
		return (unsigned char)(this->getAlpha() * factor);
	}

	float Object::_getDerivedAngle(aprilui::Object* overrideRoot)
	{
		float angle = mAngle;
		if (mParent != overrideRoot)
		{
			angle += mParent->_getDerivedAngle(overrideRoot);
		}
		return angle;
	}

	bool Object::isAnimated()
	{
		foreach (Animator*, it, mDynamicAnimators)
		{
			if ((*it)->isAnimated())
			{
				return true;
			}
		}
		foreach (Object*, it, mChildren)
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
		foreach (Animator*, it, mDynamicAnimators)
		{
			if ((*it)->isWaitingAnimation())
			{
				return true;
			}
		}
		foreach (Object*, it, mChildren)
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
		return (mDynamicAnimators.size() > 0);
	}

	void Object::draw(gvec2 offset)
	{
		if (!isVisible() || heqf(mScale.x, 0.0f, APRILUI_E_TOLERANCE) || heqf(mScale.y, 0.0f, APRILUI_E_TOLERANCE))
		{
			return;
		}
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		grect viewport;
		grect orthoProjection;
		bool clipped = (mClip && mParent != NULL);
		if (clipped)
		{
			orthoProjection = april::rendersys->getOrthoProjection();
			viewport = april::rendersys->getViewport();
			grect rect = mParent->getDerivedRect();
			gvec2 ratio = orthoProjection.getSize() / viewport.getSize();
			april::rendersys->setOrthoProjection(grect(-rect.getPosition(), rect.getSize()));
			april::rendersys->setViewport(grect((rect.getPosition() + orthoProjection.getPosition()) / ratio, rect.getSize() / ratio));
		}
		gvec2 position = mRect.getPosition() + offset + mCenter;
		if (position.x != 0.0f || position.y != 0.0f)
		{
			april::rendersys->translate(position.x, position.y);
		}
		if (mAngle != 0.0f)
		{
			april::rendersys->rotate(mAngle);
		}
		if (mScale.x != 1.0f || mScale.y != 1.0f)
		{
			april::rendersys->scale(mScale.x, mScale.y, 1.0f);
		}
		OnDraw();
		foreach (Object*, it, mChildren)
		{
			(*it)->draw(-mCenter);
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
		if (mCheckedChildUnderCursor)
		{
			clearChildUnderCursor();
		}
		foreach (Object*, it, mChildren)
		{
			(*it)->update(k);
		}
		foreach (Animator*, it, mDynamicAnimators)
		{
			(*it)->update(k);
		}
		int i = 0;
		Animator* animator;
		while (i < mDynamicAnimators.size())
		{
			animator = mDynamicAnimators[i];
			if (animator->isExpired())
			{
				delete animator;
				mDynamicAnimators.remove_at(i);
			}
			else
			{
				i++;
			}
		}
	}

	bool Object::isCursorInside()
	{
		return isPointInside(aprilui::getCursorPosition());
	}
	
	bool Object::isPointInside(gvec2 position)
	{
		if (mScale.x == 0.0f || mScale.y == 0.0f)
		{
			return false;
		}
		if (mParent != NULL)
		{
			Object* obj = this;
			Object* parent = mParent;
			while (parent != NULL)
			{
				if (obj->isClip() && !parent->getDerivedRect().isPointInside(position))
				{
					return false;
				}
				obj = parent;
				parent = obj->getParent();
			}
		}
		grect rect = getDerivedRect();
		if (mAngle == 0.0f)
		{
			return rect.isPointInside(position);
		}
		gvec2 pos = rect.getPosition();
		gvec2 points[3] = {pos, gvec2(rect.x + rect.w, rect.y), gvec2(rect.x, rect.y + rect.h)};
		gvec2 center = mCenter * getDerivedScale();
		float length;
		float angle;
		float sinv;
		float cosv;
		gvec2 current;
		for_iter (i, 0, 3)
		{
			current = points[i] - pos - center;
			if (current.x != 0.0f || current.y != 0.0f)
			{
				angle = hmodf((float)RAD_TO_DEG(atan2(-current.y, current.x)) + mAngle, 360.0f);
				length = current.length();
				sinv = (float)dsin(angle);
				cosv = (float)dcos(angle);
				points[i] += gvec2(cosv * length, -sinv * length) - current;
			}
		}
		gvec2 v1 = points[1] - points[0];
		gvec2 v2 = points[2] - points[0];
		gvec2 vX = position - points[0];
		float d1 = vX.dot(v1) / v1.squaredLength();
		float d2 = vX.dot(v2) / v2.squaredLength();
		return (is_between(d1, 0.0f, 1.0f) && is_between(d2, 0.0f, 1.0f));
	}

	bool Object::onMouseDown(float x, float y, int button)
	{
		if (mClickThrough || !isVisible() || !_isDerivedEnabled())
		{
			return false;
		}
		if (mDataset != NULL)
		{
			if (mDataset->getFocusedObject() != NULL)
			{
				april::window->terminateKeyboardHandling();
			}
			mDataset->setFocusedObject(NULL);
		}
		foreach_r (Object*, it, mChildren)
		{
			if ((*it)->isVisible() && (*it)->_isDerivedEnabled() && !(*it)->isClickThrough() &&
				(*it)->onMouseDown(x - mRect.x, y - mRect.y, button))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::onMouseUp(float x, float y, int button)
	{
		if (mClickThrough || !isVisible() || !_isDerivedEnabled())
		{
			return false;
		}
		foreach_r (Object*, it, mChildren)
		{
			if ((*it)->isVisible() && (*it)->_isDerivedEnabled() && !(*it)->isClickThrough() &&
				(*it)->onMouseUp(x - mRect.x, y - mRect.y, button))
			{
				return true;
			}
		}
		
		return false;
	}

	void Object::onMouseMove(float x, float y)
	{
		foreach_r (Object*, it, mChildren)
		{
			if ((*it)->isVisible() && (*it)->_isDerivedEnabled())
			{
				(*it)->onMouseMove(x - mRect.x, y - mRect.y);
			}
		}
	}

	void Object::onKeyDown(unsigned int keycode)
	{
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL)
			{
				object->onKeyDown(keycode);
			}
		}
	}

	void Object::onKeyUp(unsigned int keycode)
	{
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL)
			{
				object->onKeyUp(keycode);
			}
		}
	}
	
	void Object::onChar(unsigned int charcode)
	{
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL)
			{
				object->onChar(charcode);
			}
		}
	}

	void Object::cancelMouseDown()
	{
		foreach (Object*, it, mChildren)
		{
			(*it)->cancelMouseDown();
		}
	}

	void Object::registerEvent(chstr name, void (*callback)(EventArgs*))
	{
		registerEvent(name,new CallbackEvent(callback));
	}

	void Object::registerEvent(chstr name, Event* e)
	{
		if (mEvents.has_key(name))
		{
			Event* oldEvent = mEvents[name];
			if (e == NULL)
			{
				mEvents.remove_key(name);
			}
			delete oldEvent;
		}
		if (e != NULL)
		{
			mEvents[name] = e;
		}
	}

	void Object::unregisterEvent(chstr name)
	{
		mEvents.remove_key(name);
	}

	void Object::_triggerEvent(chstr name, float x, float y, unsigned int keycode, chstr extra)
	{
		if (mEvents.has_key(name))
		{
			EventArgs args(this, x, y, keycode, extra);
			mEvents[name]->execute(&args);
		}
	}

	void Object::notifyEvent(chstr name, void* params)
	{

	}

	void Object::resetCenter()
	{
		mCenter = mRect.getSize() / 2;
	}

	bool Object::_isDerivedEnabled()
	{
		return (isEnabled() && (mParent == NULL || mParent->_isDerivedEnabled()));
	}
	
	bool Object::_isDerivedClickThrough()
	{
		return (mClickThrough && (mParent == NULL || mParent->_isDerivedClickThrough()));
	}
	
	void Object::setAlpha(unsigned char value)
	{
		mColor.a = value;
	}

	hstr Object::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "x")				return getX();
		if (name == "y")				return getY();
		if (name == "w")				return getWidth();
		if (name == "h")				return getHeight();
		if (name == "visible")			return isVisible();
		if (name == "zorder")			return getZOrder();
		if (name == "enabled")			return isEnabled();
		if (name == "click_through")	return isClickThrough();
		if (name == "inherits_alpha")	return isInheritsAlpha();
		if (name == "red")				return getRed();
		if (name == "green")			return getGreen();
		if (name == "blue")				return getBlue();
		if (name == "alpha")			return getAlpha();
		if (name == "color")			return getColor().hex();
		if (name == "angle")			return getAngle();
		if (name == "scale_x")			return getScaleX();
		if (name == "scale_y")			return getScaleY();
		if (name == "center_x")			return getCenterX();
		if (name == "center_y")			return getCenterY();
		if (name == "anchor_left")		return isAnchorLeft();
		if (name == "anchor_right")		return isAnchorRight();
		if (name == "anchor_top")		return isAnchorTop();
		if (name == "anchor_bottom")	return isAnchorBottom();
		if (name == "clip")				return isClip();
		if (property_exists != NULL)
		{
			*property_exists = false;
		}
		return "";
	}
	
	bool Object::setProperty(chstr name, chstr value)
	{
		if (name == "x")					setX(value);
		else if (name == "y")				setY(value);
		else if (name == "w")				setWidth(value);
		else if (name == "h")				setHeight(value);
		else if (name == "visible")			setVisible(value);
		else if (name == "zorder")			setZOrder(value);
		else if (name == "enabled")			setEnabled(value);
		else if (name == "click_through")	setClickThrough(value);
		else if (name == "clickthrough")
		{
			aprilui::log("WARNING: \"clickthrough=\" is deprecated. Use \"click_through=\" instead."); // DEPRECATED
			setClickThrough(value);
		}
		else if (name == "inherits_alpha")	setInheritsAlpha(value);
		else if (name == "red")				setRed((int)value);
		else if (name == "green")			setGreen((int)value);
		else if (name == "blue")			setBlue((int)value);
		else if (name == "alpha")			setAlpha((int)value);
		else if (name == "color")			setColor(value);
		else if (name == "angle")			setAngle(value);
		else if (name == "scale_x")			setScaleX(value);
		else if (name == "scale_y")			setScaleY(value);
		else if (name == "center_x")		setCenterX(value);
		else if (name == "center_y")		setCenterY(value);
		else if (name == "anchor_left")		setAnchorLeft(value);
		else if (name == "anchor_right")	setAnchorRight(value);
		else if (name == "anchor_top")		setAnchorTop(value);
		else if (name == "anchor_bottom")	setAnchorBottom(value);
		else if (name == "anchors")
		{
			harray<hstr> anchors = value.replace(" ", "").lower().split(",", -1, true);
			setAnchorLeft(anchors.contains("left"));
			setAnchorRight(anchors.contains("right"));
			setAnchorTop(anchors.contains("top"));
			setAnchorBottom(anchors.contains("bottom"));
		}
		else if (name == "clip")			setClip(value);
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
		foreach (Object*, it, mChildren)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		if (recursive)
		{
			Object* object;
			foreach (Object*, it, mChildren)
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
		if (!isVisible() || isClickThrough())
		{
			return NULL;
		}
		Object* object = NULL;
		foreach_r (Object*, it, mChildren)
		{
			object = (*it)->getChildUnderPoint(pos);
			if (object != NULL && dynamic_cast<Animator*>(object) == NULL)
			{
				break;
			}
		}
		return (object == NULL && isPointInside(pos) ? this : object);
	}

	Object* Object::getChildUnderPoint(float x, float y)
	{
		return getChildUnderPoint(gvec2(x, y));
	}
	
	Object* Object::getChildUnderCursor()
	{
		if (!mCheckedChildUnderCursor)
		{
			mChildUnderCursor = getChildUnderPoint(aprilui::getCursorPosition());
			mCheckedChildUnderCursor = true;
		}
		return mChildUnderCursor;
	}
	
	void Object::clearChildUnderCursor()
	{
		mChildUnderCursor = NULL;
		mCheckedChildUnderCursor = false;
	}
	
	bool Object::isChild(Object* obj)
	{
		if (obj == NULL) return false;
		return (obj->getParent() == this);
	}
	
	bool Object::isDescendant(Object* obj)
	{
		if (obj == NULL) return false;
		return obj->isAncestor(this);
	}
	
	bool Object::isParent(Object* obj)
	{
		if (obj == NULL) return false;
		return mParent == obj;
	}
	
	bool Object::isAncestor(Object* obj)
	{
		if (obj == NULL) return false;
		for (Object* o = this->getParent(); o != NULL; o = o->getParent())
		{
			if (o == obj) return true;
		}
		return false;
	}

	grect Object::getDerivedRect(aprilui::Object* overrideRoot)
	{
		return grect(getDerivedPosition(overrideRoot), getDerivedSize(overrideRoot));
	}
	
	gvec2 Object::getDerivedPosition(aprilui::Object* overrideRoot)
	{
		gvec2 position = getPosition();
		position += (gvec2(1.0f, 1.0f) - mScale) * mRect.getSize() * (mCenter / mRect.getSize());
		if (mParent != overrideRoot)
		{
			position *= mParent->getDerivedScale(overrideRoot);
			position += mParent->getDerivedPosition(overrideRoot);
		}
		return position;
	}
	
	gvec2 Object::getDerivedSize(aprilui::Object* overrideRoot)
	{
		return (mRect.getSize() * getDerivedScale(overrideRoot));
	}

	gvec2 Object::getDerivedCenter(aprilui::Object* overrideRoot)
	{
		gvec2 center = getCenter();
		if (mParent != overrideRoot)
		{
			center *= mParent->getDerivedScale(overrideRoot);
		}
		return center;
	}
	
	gvec2 Object::getDerivedScale(aprilui::Object* overrideRoot)
	{
		gvec2 scale = mScale;
		if (mParent != overrideRoot)
		{
			scale *= mParent->getDerivedScale(overrideRoot);
		}
		return scale;
	}
	
	grect Object::_getDrawRect()
	{
		return grect(-mCenter, mRect.getSize());
	}

	april::Color Object::_getDrawColor()
	{
		april::Color color = mColor;
		if (mInheritsAlpha)
		{
			color.a = getDerivedAlpha();
		}
		return color;
	}

	Animator* Object::moveX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		CREATE_DYNAMIC_ANIMATOR(MoverX, mRect.x, x, speed);
		return animatorMoverX;
	}

	Animator* Object::moveY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverY, mRect.y, y, speed);
		return animatorMoverY;
	}

	Animator* Object::scaleX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, mScale.x, x, speed);
		return animatorScalerX;
	}

	Animator* Object::scaleY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, mScale.y, y, speed);
		return animatorScalerY;
	}

	Animator* Object::resizeX(float x, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, mRect.w, x, speed);
		return animatorResizerX;
	}

	Animator* Object::resizeY(float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, mRect.h, y, speed);
		return animatorResizerY;
	}

	Animator* Object::rotate(float angle, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(Rotator);
		CREATE_DYNAMIC_ANIMATOR(Rotator, mAngle, angle, speed);
		return animatorRotator;
	}

	Animator* Object::fadeRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)mColor.r, (float)r, speed);
		return animatorRedChanger;
	}

	Animator* Object::fadeGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)mColor.g, (float)g, speed);
		return animatorGreenChanger;
	}

	Animator* Object::fadeBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)mColor.b, (float)b, speed);
		return animatorBlueChanger;
	}

	Animator* Object::fadeAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)mColor.a, (float)a, speed);
		return animatorAlphaChanger;
	}

	void Object::move(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverX, mRect.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(MoverY, mRect.y, y, speed);
	}

	void Object::move(gvec2 position, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(MoverX);
		REMOVE_EXISTING_ANIMATORS(MoverY);
		CREATE_DYNAMIC_ANIMATOR(MoverX, mRect.x, position.x, speed);
		CREATE_DYNAMIC_ANIMATOR(MoverY, mRect.y, position.y, speed);
	}

	void Object::scale(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, mScale.x, x, speed);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, mScale.y, y, speed);
	}

	void Object::scale(gvec2 scale, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ScalerX);
		REMOVE_EXISTING_ANIMATORS(ScalerY);
		CREATE_DYNAMIC_ANIMATOR(ScalerX, mScale.x, scale.x, speed);
		CREATE_DYNAMIC_ANIMATOR(ScalerY, mScale.y, scale.y, speed);
	}

	void Object::resize(float x, float y, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, mRect.w, x, speed);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, mRect.h, y, speed);
	}

	void Object::resize(gvec2 size, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ResizerX);
		REMOVE_EXISTING_ANIMATORS(ResizerY);
		CREATE_DYNAMIC_ANIMATOR(ResizerX, mRect.w, size.x, speed);
		CREATE_DYNAMIC_ANIMATOR(ResizerY, mRect.h, size.y, speed);
	}

	void Object::fadeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)mColor.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)mColor.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)mColor.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)mColor.a, (float)a, speed);
	}
	
	void Object::fadeColor(april::Color color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(RedChanger, (float)mColor.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenChanger, (float)mColor.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueChanger, (float)mColor.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaChanger, (float)mColor.a, (float)color.a, speed);
	}
	
	Animator* Object::moveXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, mRect.x, x, speed, delay);
		return animatorMoverX;
	}

	Animator* Object::moveYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, mRect.y, y, speed, delay);
		return animatorMoverY;
	}

	Animator* Object::scaleXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, mScale.x, x, speed, delay);
		return animatorScalerX;
	}

	Animator* Object::scaleYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, mScale.y, y, speed, delay);
		return animatorScalerY;
	}

	Animator* Object::resizeXQueue(float x, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, mRect.w, x, speed, delay);
		return animatorResizerX;
	}

	Animator* Object::resizeYQueue(float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, mRect.h, y, speed, delay);
		return animatorResizerY;
	}

	Animator* Object::rotateQueue(float angle, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(Rotator, mAngle, angle, speed, delay);
		return animatorRotator;
	}

	Animator* Object::fadeRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)mColor.r, (float)r, speed, delay);
		return animatorRedChanger;
	}

	Animator* Object::fadeGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)mColor.g, (float)g, speed, delay);
		return animatorGreenChanger;
	}

	Animator* Object::fadeBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)mColor.b, (float)b, speed, delay);
		return animatorBlueChanger;
	}

	Animator* Object::fadeAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)mColor.a, (float)a, speed, delay);
		return animatorAlphaChanger;
	}

	void Object::moveQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, mRect.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, mRect.y, y, speed, delay);
	}

	void Object::moveQueue(gvec2 position, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverX, mRect.x, position.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(MoverY, mRect.y, position.y, speed, delay);
	}

	void Object::scaleQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, mScale.x, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, mScale.y, y, speed, delay);
	}

	void Object::scaleQueue(gvec2 scale, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerX, mScale.x, scale.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ScalerY, mScale.y, scale.y, speed, delay);
	}

	void Object::resizeQueue(float x, float y, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, mRect.w, x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, mRect.h, y, speed, delay);
	}

	void Object::resizeQueue(gvec2 size, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerX, mRect.w, size.x, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ResizerY, mRect.h, size.y, speed, delay);
	}

	void Object::fadeColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)mColor.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)mColor.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)mColor.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)mColor.a, (float)a, speed, delay);
	}

	void Object::fadeColorQueue(april::Color color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedChanger, (float)mColor.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenChanger, (float)mColor.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueChanger, (float)mColor.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaChanger, (float)mColor.a, (float)color.a, speed, delay);
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

	void Object::fadeColorStop()
	{
		REMOVE_EXISTING_ANIMATORS(RedChanger);
		REMOVE_EXISTING_ANIMATORS(GreenChanger);
		REMOVE_EXISTING_ANIMATORS(BlueChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaChanger);
	}
	
}
