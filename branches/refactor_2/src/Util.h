/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_UTIL_H
#define APRILUI_UTIL_H

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

struct xml_node;

namespace aprilui
{
	hstr pathGetBaseDir(chstr path);
	hstr generateName(chstr prefix);
	hstr getPWD();
	
}
#endif
