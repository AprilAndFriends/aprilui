/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "Animator.h"
#include "BaseObject.h"
#include "CallbackEvent.h"
#include "Dataset.h"
#include "EventArgs.h"
#include "Exception.h"
#include "Object.h"

namespace aprilui
{
	static bool _objectSortCallback(Object* a, Object* b)
	{
		return (a->getZOrder() < b->getZOrder());
	}
	
	harray<PropertyDescription> BaseObject::_propertyDescriptions;

	BaseObject::BaseObject(chstr name) : EventReceiver()
	{
		this->name = name;
		this->parent = NULL;
		this->enabled = true;
		this->awake = true;
		this->zOrder = 0;
	}

	BaseObject::BaseObject(const BaseObject& other) : EventReceiver(other)
	{
		this->name = other.name;
		this->tag = other.tag;
		this->parent = NULL;
		this->enabled = other.enabled;
		this->awake = other.awake;
		this->zOrder = other.zOrder;
	}

	BaseObject::~BaseObject()
	{
		foreach_m (Event*, it, this->events)
		{
			if (this->dataset != NULL)
			{
				this->dataset->removeCallbackFromQueue(it->second);
			}
			delete it->second;
		}
		this->events.clear();
	}

	harray<PropertyDescription> BaseObject::getPropertyDescriptions()
	{
		if (BaseObject::_propertyDescriptions.size() == 0)
		{
			BaseObject::_propertyDescriptions += PropertyDescription("name", PropertyDescription::STRING);
			BaseObject::_propertyDescriptions += PropertyDescription("full_name", PropertyDescription::STRING);
			BaseObject::_propertyDescriptions += PropertyDescription("tag", PropertyDescription::STRING);
			BaseObject::_propertyDescriptions += PropertyDescription("enabled", PropertyDescription::BOOL);
			BaseObject::_propertyDescriptions += PropertyDescription("awake", PropertyDescription::BOOL);
			BaseObject::_propertyDescriptions += PropertyDescription("zorder", PropertyDescription::INT);
		}
		return BaseObject::_propertyDescriptions;
	}

	bool BaseObject::hasProperty(chstr name)
	{
		harray<PropertyDescription> properties = this->getPropertyDescriptions();
		foreach (PropertyDescription, it, properties)
		{
			if ((*it).getName() == name)
			{
				return true;
			}
		}
		return false;
	}

	void BaseObject::setName(chstr value)
	{
		if (this->dataset != NULL)
		{
			hlog::errorf(logTag, "Cannot set name of object '%s', it is already registered in a dataset!", this->name.cStr());
			return;
		}
		this->name = value;
	}

	hstr BaseObject::getFullName()
	{
		return (this->dataset != NULL ? this->dataset->getName() + "." + this->name : this->name);
	}

	harray<Object*> BaseObject::getAncestors()
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

	harray<BaseObject*> BaseObject::getDescendants()
	{
		harray<BaseObject*> descendants = this->getChildren();
		foreach (Object*, it, this->childrenObjects)
		{
			descendants += (*it)->getDescendants();
		}
		return descendants;
	}

	void BaseObject::setZOrder(int zorder)
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

	harray<BaseObject*> BaseObject::getChildren()
	{
		return (this->childrenObjects.cast<BaseObject*>() + this->childrenAnimators.cast<BaseObject*>());
	}

	void BaseObject::_sortChildren()
	{
		this->childrenObjects.sort(&_objectSortCallback);
	}

	bool BaseObject::isChild(BaseObject* obj)
	{
		return (obj != NULL && obj->isParent(this));
	}

	bool BaseObject::isDescendant(BaseObject* obj)
	{
		return (obj != NULL && obj->isAncestor(this));
	}

	bool BaseObject::isParent(BaseObject* obj)
	{
		return (obj != NULL && obj == this->parent);
	}

	bool BaseObject::isAncestor(BaseObject* obj)
	{
		if (obj == NULL)
		{
			return false;
		}
		for (BaseObject* o = this->getParent(); o != NULL; o = o->getParent())
		{
			if (o == obj)
			{
				return true;
			}
		}
		return false;
	}

	BaseObject* BaseObject::findChildByName(chstr name)
	{
		foreach (Object*, it, this->childrenObjects)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		foreach (Animator*, it, this->childrenAnimators)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}

	BaseObject* BaseObject::findDescendantByName(chstr name)
	{
		BaseObject* object = this->findChildByName(name);
		if (object != NULL)
		{
			return object;
		}
		foreach (Object*, it, this->childrenObjects)
		{
			object = (*it)->findDescendantByName(name);
			if (object != NULL)
			{
				return object;
			}
		}
		return NULL;
	}

	void BaseObject::update(float timeDelta)
	{
		if (this->awake)
		{
			this->_update(timeDelta);
		}
	}

	void BaseObject::_update(float timeDelta)
	{
		// because update() could change the Z order and thus the child order
		harray<Object*> objects = this->childrenObjects;
		harray<Animator*> animators = this->childrenAnimators;
		foreach (Object*, it, objects)
		{
			(*it)->update(timeDelta);
		}
		foreach (Animator*, it, animators)
		{
			(*it)->update(timeDelta);
		}
	}

	bool BaseObject::isDerivedEnabled()
	{
		return (this->isEnabled() && (this->parent == NULL || this->parent->isDerivedEnabled()));
	}
	
	void BaseObject::setEnabled(bool value)
	{
		if (this->enabled != value)
		{
			this->enabled = value;
			this->notifyEvent(Event::EnabledChanged, NULL);
		}
	}

	void BaseObject::setAwake(bool value)
	{
		if (this->awake != value)
		{
			this->awake = value;
			this->notifyEvent(Event::AwakeChanged, NULL);
		}
	}

	hstr BaseObject::getProperty(chstr name)
	{
		if (name == "name")			return this->getName();
		if (name == "tag")			return this->getTag();
		if (name == "full_name")	return this->getFullName();
		if (name == "enabled")		return this->isEnabled();
		if (name == "awake")		return this->isAwake();
		if (name == "zorder")		return this->getZOrder();
		if (name != "type" && !this->hasProperty(name))
		{
			hlog::errorf(logTag, "Could not get property '%s' in '%s'!", name.cStr(), this->name.cStr());
		}
		return "";
	}
	
	bool BaseObject::setProperty(chstr name, chstr value)
	{
		if		(name == "name")	this->setName(value);
		else if (name == "tag")		this->setTag(value);
		else if (name == "enabled")	this->setEnabled(value);
		else if (name == "awake")	this->setAwake(value);
		else if (name == "zorder")	this->setZOrder(value);
		else
		{
			if (name != "type" && !this->hasProperty(name))
			{
				hlog::errorf(logTag, "Could not set property '%s' to '%s' in '%s'!", name.cStr(), value.cStr(), this->name.cStr());
			}
			return false;
		}
		return true;
	}
	
}
