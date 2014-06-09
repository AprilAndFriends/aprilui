/// @file
/// @version 3.0
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

#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport PropertyDescription
	{
	public:
		enum PropertyType
		{
			TYPE_INT,
			TYPE_FLOAT,
			TYPE_CHAR,
			TYPE_UCHAR,
			TYPE_BOOL,
			TYPE_STRING,
			TYPE_ENUM,
			TYPE_GRECT,
			TYPE_GVEC2,
			TYPE_HEXCOLOR
		};

		PropertyDescription(chstr name, PropertyType type);
		~PropertyDescription();

		HL_DEFINE_GET(PropertyType, type, Type);
		HL_DEFINE_GET(hstr, name, Name);

	protected:
		hstr name;
		PropertyType type;

	};

}
#endif
