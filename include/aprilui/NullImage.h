/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_NULL_IMAGE_H
#define APRILUI_NULL_IMAGE_H

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport NullImage : public Image
	{
	public:
		NullImage();
		
		void draw(grectf rect, april::Color color) { }
		void draw(grectf rect, april::Color color, float angle) { }
		
	};

}
#endif
