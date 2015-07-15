/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "aprilui.h"
#include "PropertyDescription.h"

namespace aprilui
{
	PropertyDescription::PropertyDescription(chstr name, PropertyType type, bool arrayData)
	{
		this->name = name;
		this->type = type;
		this->arrayData = arrayData;
	}

	PropertyDescription::~PropertyDescription()
	{
	}

}
