/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_COLOR_IMAGE_H
#define APRILUI_COLOR_IMAGE_H

#include <april/aprilUtil.h>
#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport ColorImage : public Image
	{
	public:
		ColorImage(chstr name);
		ColorImage(chstr name, grect rect);
		ColorImage(chstr name, grect rect, chstr color);
		ColorImage(chstr name, grect rect, april::Color color);
		
		void draw(grect rect, april::Color color);
		void draw(grect rect, april::Color color, float angle, gvec2 center);
		
	protected:
		april::Color mColor;
		
	private:
		april::PlainVertex _pVertices[4];
		
	};

}
#endif
