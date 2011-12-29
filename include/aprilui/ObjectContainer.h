/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a generic object container.

#ifndef APRILUI_DUMMY_OBJECT_H
#define APRILUI_DUMMY_OBJECT_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport Container : public Object
	{
	public:
		Container(chstr name, grect rect);
		~Container();
		static Object* createInstance(chstr name, grect rect);
		
	};
}

#endif
