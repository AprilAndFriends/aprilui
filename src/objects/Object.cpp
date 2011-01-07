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
#include "aprilui.h"

namespace aprilui
{
	Object::Object(chstr type, chstr name)
	{
		mTypeName = type;
		mName = name;
		mParent = NULL;
		mDataset = NULL;
		mEnabled = true;
	}

	Object::~Object()
	{
		foreach_m (Event*, it, mEvents)
		{
			delete it->second;
		}
	}
	
	bool _objectSortCallback(Object* a, Object* b)
	{
		return (a->getZOrder() < b->getZOrder());
	}

	void Object::_sortChildren()
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
		_sortChildren();
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

	void Object::setZOrder(int value)
	{
		if (mParent != NULL)
		{
			mParent->_sortChildren();
		}
	}

	bool Object::isAnimated()
	{
		foreach (Object*, it, mChildren)
		{
			if (dynamic_cast<Animator*>(*it) != NULL && (*it)->isAnimated())
			{
				return true;
			}
		}
		return false;
	}

	void Object::update(float k)
	{
		foreach (Object*, it, mChildren)
		{
			(*it)->update(k);
		}
	}

	bool Object::OnMouseDown(float x, float y, int button)
	{
		if (mDataset != NULL)
		{
			if (mDataset->getFocusedObject() != NULL)
			{
				april::rendersys->getWindow()->terminateKeyboardHandling();
			}
			mDataset->setFocusedObject(NULL);
		}
		foreach_r (Object*, it, mChildren)
		{
			if ((*it)->OnMouseDown(x - getX(), y - getY(), button))
			{
				return true;
			}
		}
		return false;
	}

	bool Object::OnMouseUp(float x, float y, int button)
	{
		foreach_r (Object*, it, mChildren)
		{
			if ((*it)->OnMouseUp(x - getX(), y - getY(), button))
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
			(*it)->OnMouseMove(x - getX(), y - getY());
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
		if (name == "enabled")	setEnabled(value);
	}

	Object* Object::getChildUnderPoint(float x, float y)
	{
		return getChildUnderPoint(gvec2(x, y));
	}
	
	Object* Object::getChildUnderPoint(gvec2 pos)
	{
		Object* object;
		foreach_r (Object*, it, mChildren)
		{
			object = (*it)->getChildUnderPoint(pos);
			if (object != NULL && dynamic_cast<Animator*>(object) == NULL)
			{
				return object;
			}
		}
		return NULL;
	}
	
}
