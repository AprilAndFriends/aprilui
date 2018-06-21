/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Style.h"

namespace aprilui
{
	Style::Group::Group(hmap<hstr, hstr> properties)
	{
		this->properties = properties;
	}

	Style::Group::~Group()
	{
	}

	Style::Style(chstr name) :
		Cloneable()
	{
		this->name = name;
		this->dataset = NULL;
	}

	Style::Style(const Style& other) :
		Cloneable(other)
	{
		this->name = other.name;
		this->dataset = NULL;
		this->objectDefaults = other.objectDefaults;
		this->animatorDefaults = other.animatorDefaults;
		this->objects = other.objects;
		this->animators = other.animators;
	}

	Style::~Style()
	{
	}

	hstr Style::getFullName() const
	{
		return (this->dataset != NULL ? this->dataset->getName() + "." + this->name : this->name);
	}

	hmap<hstr, hstr> Style::findProperties(chstr className)
	{
		hmap<hstr, hstr> result;
		if (aprilui::getObjectFactories().hasKey(className))
		{
			result = this->objectDefaults.properties;
			if (this->objects.hasKey(className))
			{
				result.inject(this->objects[className].properties);
			}
		}
		else if (aprilui::getAnimatorFactories().hasKey(className))
		{
			result = this->animatorDefaults.properties;
			if (this->animators.hasKey(className))
			{
				result.inject(this->animators[className].properties);
			}
		}
		else
		{
			hlog::warn(logTag, "Class '" + className + "' not registered in aprilui!");
		}
		return result;
	}

	void Style::_inject(Style* other)
	{
		this->objectDefaults.properties.inject(other->objectDefaults.properties);
		this->animatorDefaults.properties.inject(other->animatorDefaults.properties);
		harray<hstr> thisObjectKeys = this->objects.keys();
		harray<hstr> otherObjectKeys = other->objects.keys();
		harray<hstr> existingObjectKeys = otherObjectKeys & thisObjectKeys;
		harray<hstr> addedObjectKeys = otherObjectKeys / thisObjectKeys;
		foreach (hstr, it, existingObjectKeys)
		{
			this->objects[*it].properties.inject(other->objects[*it].properties);
		}
		foreach (hstr, it, addedObjectKeys)
		{
			this->objects[*it] = other->objects[*it];
		}
		harray<hstr> thisAnimatorKeys = this->animators.keys();
		harray<hstr> otherAnimatorKeys = other->animators.keys();
		harray<hstr> existingAnimatorKeys = otherAnimatorKeys & thisAnimatorKeys;
		harray<hstr> addedAnimatorKeys = otherAnimatorKeys / thisAnimatorKeys;
		foreach (hstr, it, existingAnimatorKeys)
		{
			this->animators[*it].properties.inject(other->animators[*it].properties);
		}
		foreach (hstr, it, addedAnimatorKeys)
		{
			this->animators[*it] = other->animators[*it];
		}
	}

	Style* Style::_injected(Style* other)
	{
		Style* result = this->clone();
		result->_inject(other);
		return result;
	}

}
