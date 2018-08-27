/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_COLORED_IMAGE_H
#define APRILUI_COLORED_IMAGE_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class Texture;

	class apriluiExport ColoredImage : public Image
	{
	public:
		ColoredImage(Texture* texture, chstr name, grectf source, bool vertical = false, april::Color color = april::Color());
		
		void draw(grectf rect, april::Color color);
		void draw(grectf rect, april::Color color, float angle, gvec2f center);

		float getAlpha();
		void setAlpha(float alpha);
		void setColor(april::Color value) { mColor = value; }
		void setColor(float a, float r, float g, float b);
		
	protected:
		april::Color mColor;
		
	};

}
#endif
