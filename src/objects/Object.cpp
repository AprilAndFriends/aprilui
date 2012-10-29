/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/RenderSystem.h>
#include <april/Window.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Animator.h"
#include "CallbackEvent.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "Exception.h"
#include "ObjectCallbackObject.h"
#include "aprilui.h"

namespace aprilui
{
	Object::Object(chstr type, chstr name, grect rect) : EventReceiver()
	{
		mTypeName = type;
		mName = name;
		mParent = NULL;
		mZOrder = 0;
		mRect=rect;
		mVisible = true;
		mEnabled = true;
		mClickthrough = false;
		mInheritsAlpha = true;
		mAlpha = 1.0f;
		mDataset = NULL;
		mScale.set(1.0f, 1.0f);
		mUseScale = false;
	}
	
	Object::Object(chstr name, grect rect) : EventReceiver()
	{
		mTypeName = "undefined";
		mName = name;
		mParent = NULL;
		mZOrder = 0;
		mRect=rect;
		mVisible = true;
		mEnabled = true;
		mClickthrough = false;
		mInheritsAlpha = true;
		mAlpha = 1.0f;
		mDataset = NULL;
		mScale.set(1.0f, 1.0f);
		mUseScale = false;
	}

	Object::~Object()
	{
		foreach_m(Event*, it, mEvents)
		{
			delete it->second;
		}
	}

	bool _objectSortCallback(Object* a, Object* b)
	{
		return (a->getZOrder() < b->getZOrder());
	}

	void Object::sortChildren()
	{
		mChildren.sort(_objectSortCallback);
	}

	void Object::addChild(Object* object)
	{
		if (object->getParent())
		{
			throw ObjectHasParentException(object->getName(), getName());
		}
		mChildren += object;
		sortChildren();
		object->setParent(this);
	}
	
	void Object::removeChild(Object* object)
	{
		if (object->getParent() != this)
		{
			throw ObjectNotChildException(object->getName(), getName());
		}
		mChildren -= object;
		object->setParent(NULL);
	}

	void Object::removeAllChildren()
	{
		foreach (Object*, it, mChildren)
		{
			(*it)->setParent(NULL);
		}
		mChildren.clear();
	}

	void Object::setZOrder(int zorder)
	{
		if (mZOrder != zorder)
		{
			mZOrder = zorder;
			if (mParent != NULL)
			{
				mParent->sortChildren();
			}
		}
	}

	float Object::getDerivedAlpha()
	{
		// recursive function that combines all the alpha from the parents (if any)
		float alpha = this->getAlpha();
		if (mInheritsAlpha && mParent != NULL)
		{
			alpha *= mParent->getDerivedAlpha();
		}
		return alpha;
	}

	void Object::draw(gvec2 offset)
	{
		if (isVisible())
		{
			OnDraw(offset);
			gvec2 rect(offset.x + mRect.x, offset.y + mRect.y);
			foreach (Object*, it, mChildren)
			{
				(*it)->draw(rect);
			}
		}
	}

	void Object::update(float k)
	{
		foreach (Object*, it, mChildren)
		{
			(*it)->update(k);
		}
	}

	bool Object::isCursorInside()
	{
		gvec2 pos = getCursorPosition();
		for (Object* p = mParent; p != NULL; p = p->mParent)
		{
			pos -= p->getPosition();
		}
		return isPointInside(pos);
	}

	bool Object::isPointInside(gvec2 position)
	{
		return mRect.isPointInside(position);
	}

	bool Object::isPointInside(float x, float y)
	{
		return mRect.isPointInside(x, y);
	}

	bool Object::onMouseDown(int button)
	{
		return OnMouseDown(0, 0, button);
	}
	
	bool Object::onMouseUp(int button)
	{
		return OnMouseUp(0, 0, button);
	}
	
	bool Object::onMouseMove()
	{
		return OnMouseMove(0, 0);
	}
	
	bool Object::onKeyDown(unsigned int keyCode)
	{
		return OnKeyDown(keyCode);
	}
	
	bool Object::onKeyUp(unsigned int keyCode)
	{
		return OnKeyUp(keyCode);
	}
	
	bool Object::onChar(unsigned int charCode)
	{
		return OnChar(charCode);
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
	
	bool Object::OnMouseDown(float x, float y, int button)
	{
		if (mClickthrough || !isVisible() || !isDerivedEnabled())
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
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && !(*it)->isClickthrough() &&
				(*it)->OnMouseDown(x - mRect.x, y - mRect.y, button))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::OnMouseUp(float x, float y, int button)
	{
		if (mClickthrough || !isVisible() || !isDerivedEnabled())
		{
			return false;
		}
		foreach_r (Object*, it, mChildren)
		{
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() && !(*it)->isClickthrough() &&
				(*it)->OnMouseUp(x - mRect.x, y - mRect.y, button))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::OnMouseMove(float x, float y)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		foreach_r (Object*, it, mChildren)
		{
			if ((*it)->isVisible() && (*it)->isDerivedEnabled() &&
				(*it)->OnMouseMove(x - mRect.x, y - mRect.y))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::OnKeyDown(unsigned int keyCode)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL && object->OnKeyDown(keyCode))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::OnKeyUp(unsigned int keyCode)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL && object->OnKeyUp(keyCode))
			{
				return true;
			}
		}
		return false;
	}
	
	bool Object::OnChar(unsigned int charCode)
	{
		if (!this->isVisible() || !this->isDerivedEnabled())
		{
			return false;
		}
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL && object->OnChar(charCode))
			{
				return true;
			}
		}
		return false;
	}
	
	hstr Object::getProperty(chstr prop, bool* prop_exists) // new-april compatibility
	{
		if (prop_exists) *prop_exists = true;
		return "";
	}
	
	grect Object::getBoundingRect(aprilui::Object* overrideRoot)
	{
		return mRect;
	}
	
	gvec2 Object::getDerivedPosition(aprilui::Object* overrideRoot)
	{
		gvec2 position = getPosition();
		position += mRect.getSize() * gvec2(0.5f, 0.5f);
		if (mParent != overrideRoot)
		{
			position += mParent->getDerivedPosition(overrideRoot);
		}
		return position;
	}
	
	gvec2 Object::getDerivedSize(aprilui::Object* overrideRoot)
	{
		return mRect.getSize();
	}
	
	gvec2 Object::getDerivedScale(aprilui::Object* overrideRoot)
	{
		return gvec2(1.0f, 1.0f);
	}

	grect Object::_getDrawRect()
	{
		return (!mUseScale ? mRect : (mRect - mRect.getSize() * 0.5f));
	}

	void Object::registerEvent(chstr name, void (*callback)(EventArgs*))
	{
		Event* event = NULL;
		if (mEvents.has_key(name))
		{
			event = mEvents[name];
		}
        if (callback == NULL)
		{
			mEvents.remove_key(name);
		}
        else
		{
			mEvents[name] = new CallbackEvent(callback);
		}
		if (event != NULL)
		{
			delete event;
		}
	}

	void Object::triggerEvent(chstr name, float x, float y, chstr extra)
	{
		if (mEvents.has_key(name))
		{
			EventArgs args(this, x, y, extra);
			mEvents[name]->execute(&args);
		}
	}

	void Object::notifyEvent(chstr name, void* params)
	{
		if (name != "onLocalizationChanged") // because this is handled somewhere else in a smarter way
		{
			foreach (Object*, it, mChildren)
			{
				(*it)->notifyEvent(name, params);
			}
		}
	}

	bool Object::isDerivedEnabled()
	{
		return (isEnabled() && (mParent == NULL || mParent->isDerivedEnabled()));
	}
	
	bool Object::isDerivedClickThrough()
	{
		return (mClickthrough && (mParent == NULL || mParent->isDerivedClickThrough()));
	}
	
	void Object::setAlpha(float alpha)
	{
		mAlpha = hclamp(alpha, 0.0f, 1.0f);
	}

	void Object::moveToFront()
	{
		if (mParent != NULL)
		{
			mParent->_moveChildToFront(this);
		}
	}

	void Object::moveToBack()
	{
		if (mParent != NULL)
		{
			mParent->_moveChildToBack(this);
		}
	}

	void Object::_moveChildToFront(Object* object)
	{
		mChildren -= object;
		mChildren += object;
	}

	void Object::_moveChildToBack(Object* object)
	{
		mChildren -= object;
		mChildren.push_front(object);
	}

	bool Object::setProperty(chstr name, chstr value)
	{
		if      (name == "visible")        setVisible(value);
		else if (name == "zorder")         setZOrder(value);
		else if (name == "enabled")        setEnabled(value);
		else if (name == "clickthrough")   setClickthrough(value);
		else if (name == "inherits_alpha") setInheritsAlpha(value);
		else if (name == "alpha")          setAlpha(value);
		return 1;
	}

	Object* Object::getChildUnderPoint(float x, float y)
	{
		return getChildUnderPoint(gvec2(x, y));
	}
	
	Object* Object::getChildUnderPoint(gvec2 pos)
	{
		if (!isVisible() || !isPointInside(pos))
		{
			return NULL;
		}
		if (mChildren.size() == 0)
		{
			return this;
		}
		Object* object = NULL;
		foreach_r (Object*, it, mChildren)
		{
			object = (*it)->getChildUnderPoint(pos.x - mRect.x, pos.y - mRect.y);
			if (object != NULL && dynamic_cast<aprilui::Animator*>(object) == NULL)
			{
				break;
			}
		}
		return (object != NULL ? object : this);
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
			if ((*it)->isUseScale())
			{
				center = (*it)->getSize() * 0.5f;
				point -= center + (*it)->getPosition();
				point /= (*it)->getScale();
				point += center;
			}
			else
			{
				point -= (*it)->getPosition();
			}
		}
		return point;
	}

}
