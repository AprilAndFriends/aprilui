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
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "Animator.h"
#include "CallbackEvent.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "Exception.h"
#include "ObjectCallbackObject.h"
#include "AprilUI.h"

namespace AprilUI
{
	Object::Object(chstr type, chstr name, grect rect)
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
		mAngle = 0.0f;
		mDataset = NULL;
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
		gvec2 position = getCursorPosition();
		for (Object* p = mParent; p != NULL; p = p->mParent)
		{
			position -= p->getPosition();
		}
		return mRect.isPointInside(position);
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
				April::rendersys->getWindow()->terminateKeyboardHandling();
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

	void Object::OnMouseMove(float x, float y)
	{
		foreach_r (Object*, it, mChildren)
		{
			if ((*it)->isVisible() && (*it)->isDerivedEnabled())
			{
				(*it)->OnMouseMove(x - mRect.x, y - mRect.y);
			}
		}
	}

	void Object::OnKeyDown(unsigned int keycode)
	{
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL)
			{
				object->OnKeyDown(keycode);
			}
		}
	}

	void Object::OnKeyUp(unsigned int keycode)
	{
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL)
			{
				object->OnKeyUp(keycode);
			}
		}
	}
	
	void Object::OnChar(unsigned int charcode)
	{
		if (mDataset != NULL)
		{
			Object* object = mDataset->getFocusedObject();
			if (object != NULL)
			{
				object->OnChar(charcode);
			}
		}
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
		foreach (Object*, it, mChildren)
		{
			(*it)->notifyEvent(name, params);
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

	void Object::setProperty(chstr name, chstr value)
	{
		if      (name == "visible")			setVisible(value);
		else if (name == "zorder")			setZOrder(value);
		else if (name == "enabled")			setEnabled(value);
		else if (name == "clickthrough")	setClickthrough(value);
		else if (name == "inherits_alpha")	setInheritsAlpha(value);
		else if (name == "alpha")			setAlpha(value);
		else if (name == "angle")			setAngle(value);
	}

	bool Object::angleEquals(float angle)
	{
		float s1 = (float)dsin(angle);
		float s2 = (float)dsin(mAngle);
		float c1 = (float)dcos(angle);
		float c2 = (float)dcos(mAngle);
		return (fabs(s1 - s2) < 0.01f && fabs(c1 - c2) < 0.01f);
	}
	
	Object* Object::getChildUnderPoint(float x, float y)
	{
		return getChildUnderPoint(gvec2(x, y));
	}
	
	Object* Object::getChildUnderPoint(gvec2 pos)
	{
		if (!isVisible() || !mRect.isPointInside(pos))
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
			if (object != NULL && dynamic_cast<AprilUI::Animator*>(object) == NULL)
			{
				break;
			}
		}
		return (object != NULL ? object : this);
	}
}
