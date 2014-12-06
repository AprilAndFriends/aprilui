/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for all image classes.

#ifndef APRILUI_BASE_IMAGE_H
#define APRILUI_BASE_IMAGE_H

#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>

#include "apriluiExport.h"

#define APRILUI_IMAGE_NAME_NULL "null"

namespace aprilui
{
	// TODO
	class apriluiExport BaseImage
	{
	public:
		BaseImage();
		virtual ~BaseImage();
		
		virtual void draw(grect rect, april::Color color = april::Color::White) = 0;
		virtual void draw(harray<april::TexturedVertex> vertices, april::Color color = april::Color::White) = 0;
		
	};

}
#endif
