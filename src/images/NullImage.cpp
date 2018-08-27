/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>

#include "NullImage.h"

namespace aprilui
{
	NullImage::NullImage() : Image(0, "null", grectf())
	{
	}
	
}
