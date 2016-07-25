/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "ObjectDummyObject.h"

namespace aprilui
{
	/********************************************************************************************************/
	DummyObject::DummyObject(chstr name, grect rect) :
		Object("DummyObject", name, rect)
	{
	}

	bool DummyObject::setProperty(chstr name, chstr value)
	{
		return Object::setProperty(name, value);
	}
	
}
