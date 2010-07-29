/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>

#include "DummyObject.h"

namespace AprilUI
{
	/********************************************************************************************************/
	DummyObject::DummyObject(chstr name,float x,float y,float w,float h) :
		Object("DummyObject",name,x,y,w,h)
	{
	}

	void DummyObject::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);
	}
	
}
