/// @file
/// @version 3.5
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
			INT,
			FLOAT,
			CHAR,
			UCHAR,
			BOOL,
			STRING,
			ENUM,
			GRECT,
			GVEC2,
			HEXCOLOR
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
