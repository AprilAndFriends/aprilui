/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_UTIL_H
#define UTIL_H

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

struct xml_node;

namespace AprilUI
{
	typedef unsigned char byte;

	hstr pathGetFilename(chstr path,bool with_suffix=1);
	hstr pathGetBaseDir(chstr path);

	int hexstr_to_int(chstr s);
	void hexstr_to_argb(chstr hex,byte* a,byte* r,byte* g,byte* b);
	void hexstr_to_argb_float(chstr hex,float* a,float* r,float* g,float* b);

	hstr generateName(chstr prefix);
	int getdir(chstr dir, harray<hstr> &files);
	
	void writelog(chstr msg);
	
	hstr getPWD();
}
#endif
