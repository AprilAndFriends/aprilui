/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.8
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines an image with color modulation.

#ifndef APRILUI_COLORED_IMAGE_H
#define APRILUI_COLORED_IMAGE_H

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
		ColoredImage(Texture* texture, chstr name, grect source, bool vertical = false, april::Color color = april::Color::White);
		// TODO - has to be refactored
		/// @brief Takes an Image instance and creates a Colored Image 
		ColoredImage(Image& img, chstr name, april::Color color);
		~ColoredImage();
		
		void draw(grect rect, april::Color color);

		unsigned char getAlpha();
		void setAlpha(unsigned char value);
		void setColor(april::Color value) { this->mColor = value; }
		
	protected:
		april::Color mColor;
		
	};

}
#endif
