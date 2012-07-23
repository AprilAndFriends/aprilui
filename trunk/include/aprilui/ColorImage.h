/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
		
		april::Color getColor() { return this->mColor; }
		void setColor(april::Color value) { this->mColor = value; }
		void setColor(chstr value) { this->mColor.set(value); }

		void draw(grect rect, april::Color color);
		
	protected:
		april::Color mColor;

	private:
		april::PlainVertex _pVertices[4];
		
	};

}
#endif
