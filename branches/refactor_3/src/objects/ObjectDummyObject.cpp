/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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

	void DummyObject::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
	}
	
}
