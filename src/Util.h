/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_UTIL_H
#define UTIL_H

#include "hltypes/Array.h"
#include "hltypes/hstring.h"
struct _xmlNode;

namespace AprilUI
{
	typedef unsigned char byte;

	#define XML_EQ(a,b) xmlStrcmp(a->name, (const xmlChar *) b) == 0

	#define foreach(type,lst) for (type* it = lst.iter(); it; it = lst.next())
	#define foreach_reverse(type,lst) for (type* it = lst.riter(); it; it = lst.rnext())

	#define foreach_in_map(type,list) for (std::map<hstr,type>::iterator it=list.begin();it != list.end(); it++)

	float sign(float number);
	hstr xmlGetPropString(_xmlNode* node,const char* prop);
	float xmlGetPropFloat(_xmlNode* node,const char* prop);
	int xmlGetPropInt(_xmlNode* node,const char* prop);
	bool xmlPropertyExists(_xmlNode* node,const char* name);
	unsigned int xmlGetPropHex(_xmlNode* node,const char* prop);

	hstr pathGetFilename(hstr path,bool with_suffix=1);
	hstr pathGetBaseDir(hstr path);

	int hexstr_to_int(chstr s);
	void hexstr_to_argb(chstr hex,byte* a,byte* r,byte* g,byte* b);
	void hexstr_to_argb_float(chstr hex,float* a,float* r,float* g,float* b);

	float Sin(float angle);
	float Cos(float angle);

	float clamp(float value,float min_value,float max_value);

	hstr generateName(hstr prefix);
	int getdir (hstr dir, harray<hstr> &files);
	
	void writelog(hstr msg);
	
	hstr getPWD();
}
#endif
