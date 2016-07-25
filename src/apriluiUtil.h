/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_UTIL_H
#define APRILUI_UTIL_H

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

namespace aprilui
{
	extern const char* logTag;
	hstr pathGetBaseDir(chstr path);
	hstr generateName(chstr prefix);
	hstr getPWD();
	
}
#endif
