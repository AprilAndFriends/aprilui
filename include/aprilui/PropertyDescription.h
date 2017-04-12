/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a description for properties in objects.

#ifndef APRILUI_PROPERTY_DESCRIPTION_H
#define APRILUI_PROPERTY_DESCRIPTION_H

#include <hltypes/henum.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport PropertyDescription
	{
	public:
		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, Type,
		(
			HL_ENUM_DECLARE(Type, Int);
			HL_ENUM_DECLARE(Type, Float);
			HL_ENUM_DECLARE(Type, Char);
			HL_ENUM_DECLARE(Type, UChar);
			HL_ENUM_DECLARE(Type, Bool);
			HL_ENUM_DECLARE(Type, String);
			HL_ENUM_DECLARE(Type, Enum);
			HL_ENUM_DECLARE(Type, Grect);
			HL_ENUM_DECLARE(Type, Gvec2);
			HL_ENUM_DECLARE(Type, Gvec3);
			HL_ENUM_DECLARE(Type, Color);
		));

		PropertyDescription(chstr name, Type type, bool arrayData = false);
		~PropertyDescription();

		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(Type, type, Type);
		HL_DEFINE_IS(arrayData, ArrayData);

	protected:
		hstr name;
		Type type;
		bool arrayData;

	};

}
#endif
