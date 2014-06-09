/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an image that consists of only one color.

#ifndef APRILUI_COLOR_IMAGE_H
#define APRILUI_COLOR_IMAGE_H

#include <april/aprilUtil.h>
#include <april/Color.h>
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Image.h"

namespace aprilui
{
	class apriluiExport ColorImage : public Image
	{
	public:
		ColorImage(chstr name);
		ColorImage(chstr name, grect rect);
		ColorImage(chstr name, grect rect, chstr color);
		ColorImage(chstr name, grect rect, april::Color color);
		~ColorImage();
		
		HL_DEFINE_GETSET(april::Color, color, Color);

		void draw(grect rect, april::Color color);
		void draw(harray<april::TexturedVertex> vertices, april::Color color);
		
	protected:
		april::Color color;

	private:
		april::PlainVertex _pVertices[4];
		
	};

}
#endif
