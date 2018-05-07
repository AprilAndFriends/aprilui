/// @file
/// @version 4.3
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
	HL_ENUM_CLASS_DEFINE(PropertyDescription::Type,
	(
		HL_ENUM_DEFINE(PropertyDescription::Type, Int);
		HL_ENUM_DEFINE(PropertyDescription::Type, Float);
		HL_ENUM_DEFINE(PropertyDescription::Type, Char);
		HL_ENUM_DEFINE(PropertyDescription::Type, UChar);
		HL_ENUM_DEFINE(PropertyDescription::Type, Bool);
		HL_ENUM_DEFINE(PropertyDescription::Type, String);
		HL_ENUM_DEFINE(PropertyDescription::Type, Enum);
		HL_ENUM_DEFINE(PropertyDescription::Type, Grect);
		HL_ENUM_DEFINE(PropertyDescription::Type, Gvec2);
		HL_ENUM_DEFINE(PropertyDescription::Type, Gvec3);
		HL_ENUM_DEFINE(PropertyDescription::Type, Color);
	));

	PropertyDescription::PropertyDescription(chstr name, Type type, bool arrayData)
	{
		this->name = name;
		this->type = type;
		this->arrayData = arrayData;
	}

	PropertyDescription::~PropertyDescription()
	{
	}

}
