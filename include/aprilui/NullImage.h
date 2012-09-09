/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines an "invisible" image for consistency.

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
		~NullImage();
		
		void draw(grect rect, april::Color color) { }
		
	};

}
#endif
