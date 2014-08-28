/// @file
/// @version 3.3
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
		this->zOrder = 0;
	}

	BaseObject::~BaseObject()
	{
	}
	
	harray<PropertyDescription> BaseObject::getPropertyDescriptions()
	{
		if (BaseObject::_propertyDescriptions.size() == 0)
		{
			BaseObject::_propertyDescriptions += PropertyDescription("name", PropertyDescription::STRING);
			BaseObject::_propertyDescriptions += PropertyDescription("full_name", PropertyDescription::STRING);
			BaseObject::_propertyDescriptions += PropertyDescription("enabled", PropertyDescription::BOOL);
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
		foreach (Object*, it, this->objects)
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
		return (this->objects.cast<BaseObject*>() + this->animators.cast<BaseObject*>());
	}

	void BaseObject::_sortChildren()
	{
		this->objects.sort(&_objectSortCallback);
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
		foreach (Object*, it, this->objects)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		foreach (Animator*, it, this->animators)
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
		foreach (Object*, it, this->objects)
		{
			object = (*it)->findDescendantByName(name);
			if (object != NULL)
			{
				return object;
			}
		}
		return NULL;
	}

	void BaseObject::attach(Object* object)
	{
		object->addChild(this);
	}

	void BaseObject::detach()
	{
		if (this->parent == NULL)
		{
			throw ObjectWithoutParentException(this->getName());
		}
		this->parent->removeChild(this);
	}

	void BaseObject::update(float timeDelta)
	{
		foreach (Object*, it, this->objects)
		{
			(*it)->update(timeDelta);
		}
		foreach (Animator*, it, this->animators)
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
		if (value != this->enabled)
		{
			this->enabled = value;
			this->notifyEvent(Event::ENABLED_CHANGED, NULL);
		}
	}

	hstr BaseObject::getProperty(chstr name)
	{
		if (name == "name")			return this->getName();
		if (name == "full_name")	return this->getFullName();
		if (name == "enabled")		return this->isEnabled();
		if (name == "zorder")		return this->getZOrder();
		return "";
	}
	
	bool BaseObject::setProperty(chstr name, chstr value)
	{
		if		(name == "enabled")	this->setEnabled(value);
		else if (name == "zorder")	this->setZOrder(value);
		else return false;
		return true;
	}
	
}
