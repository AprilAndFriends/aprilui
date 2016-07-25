/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_DUMMY_OBJECT_H
#define APRILUI_DUMMY_OBJECT_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport DummyObject : public Object
	{
	public:
		DummyObject(chstr name, grect rect);
		
		bool setProperty(chstr name, chstr value);
		
	};
}

#endif
