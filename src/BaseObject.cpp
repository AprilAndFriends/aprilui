/// @file
/// @version 4.3
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
#include "Style.h"

namespace aprilui
{
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

	harray<PropertyDescription> BaseObject::getPropertyDescriptions() const
	{
		if (BaseObject::_propertyDescriptions.size() == 0)
		{
			BaseObject::_propertyDescriptions += PropertyDescription("name", PropertyDescription::Type::String);
			BaseObject::_propertyDescriptions += PropertyDescription("full_name", PropertyDescription::Type::String);
			BaseObject::_propertyDescriptions += PropertyDescription("tag", PropertyDescription::Type::String);
			BaseObject::_propertyDescriptions += PropertyDescription("enabled", PropertyDescription::Type::Bool);
			BaseObject::_propertyDescriptions += PropertyDescription("awake", PropertyDescription::Type::Bool);
			BaseObject::_propertyDescriptions += PropertyDescription("z_order", PropertyDescription::Type::Int);
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

	hstr BaseObject::getFullName() const
	{
		return (this->dataset != NULL ? this->dataset->getName() + "." + this->name : this->name);
	}

	void BaseObject::setEnabled(const bool& value)
	{
		if (this->enabled != value)
		{
			this->enabled = value;
			this->notifyEvent(Event::EnabledChanged, NULL);
		}
	}

	void BaseObject::setAwake(const bool& value)
	{
		if (this->awake != value)
		{
			this->awake = value;
			this->notifyEvent(Event::AwakeChanged, NULL);
		}
	}

	void BaseObject::setZOrder(const int& zOrder)
	{
		if (this->zOrder != zOrder)
		{
			this->zOrder = zOrder;
			if (this->parent != NULL)
			{
				this->parent->_sortChildren();
			}
		}
	}

	harray<Object*> BaseObject::getAncestors() const
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

	harray<BaseObject*> BaseObject::getDescendants() const
	{
		harray<BaseObject*> result = this->getChildren();
		foreachc (Object*, it, this->childrenObjects)
		{
			result += (*it)->getDescendants();
		}
		return result;
	}

	harray<Object*> BaseObject::getDescendantObjects() const
	{
		harray<Object*> result = this->childrenObjects;
		foreachc (Object*, it, this->childrenObjects)
		{
			result += (*it)->getDescendantObjects();
		}
		return result;
	}

	harray<BaseObject*> BaseObject::getChildren() const
	{
		return (this->childrenObjects.cast<BaseObject*>() + this->childrenAnimators.cast<BaseObject*>());
	}

	void BaseObject::_sortChildren()
	{
		HL_LAMBDA_CLASS(_sortObjects, bool, ((Object* const& a, Object* const& b) { return (a->getZOrder() < b->getZOrder()); }));
		this->childrenObjects.sort(&_sortObjects::lambda);
	}

	bool BaseObject::isDerivedEnabled() const
	{
		return (this->isEnabled() && (this->parent == NULL || this->parent->isDerivedEnabled()));
	}

	bool BaseObject::isDerivedAwake() const
	{
		return (this->isAwake() && (this->parent == NULL || this->parent->isDerivedAwake()));
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

	void BaseObject::applyStyle(Style* style)
	{
		if (style != NULL)
		{
			hmap<hstr, hstr> properties = style->findProperties(this->getClassName());
			foreach_m (hstr, it, properties)
			{
				this->setProperty(it->first, it->second);
			}
		}
	}

	void BaseObject::applyStyleByName(chstr name)
	{
		this->applyStyle(name != "" ? this->dataset->getStyle(name) : NULL);
	}

	hstr BaseObject::getProperty(chstr name)
	{
		if (name == "name")			return this->getName();
		if (name == "tag")			return this->getTag();
		if (name == "full_name")	return this->getFullName();
		if (name == "enabled")		return this->isEnabled();
		if (name == "awake")		return this->isAwake();
		if (name == "z_order")		return this->getZOrder();
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
		else if (name == "z_order")	this->setZOrder(value);
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
