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

#include "ObjectContainer.h"

namespace aprilui
{
	/********************************************************************************************************/
	Container::Container(chstr name, grect rect) :
		Object("Container", name, rect)
	{
	}

	void Container::setProperty(chstr name, chstr value)
	{
		Object::setProperty(name, value);
	}
	
}
