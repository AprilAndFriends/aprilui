/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>

#include "NullImage.h"

namespace aprilui
{
	NullImage::NullImage() : Image(0, "null", grect())
	{
	}
	
	NullImage::~NullImage()
	{
	}

}
