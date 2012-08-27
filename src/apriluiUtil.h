/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines utility functions used internally.

#ifndef APRILUI_UTIL_H
#define APRILUI_UTIL_H

#include <hltypes/hstring.h>
#include "apriluiExport.h"

#define APRILUI_E_TOLERANCE 0.0001f

namespace aprilui
{
	apriluiFnExport hstr generateName(chstr prefix);
	
}
#endif
