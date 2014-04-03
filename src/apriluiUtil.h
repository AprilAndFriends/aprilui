/// @file
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines utility functions used internally.

#ifndef APRILUI_UTIL_H
#define APRILUI_UTIL_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	static const float eTolerance = 0.0001f;

	apriluiFnExport hstr generateName(chstr prefix);
	
}
#endif
