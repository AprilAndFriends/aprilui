/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_COLORED_IMAGE_BOX_H
#define APRILUI_COLORED_IMAGE_BOX_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport ColoredImageBox : public ImageBox
	{
	public:
		ColoredImageBox(chstr name, grect rect);
		
		april::Color getColor() { return mColor; }
		void setColor(april::Color value) { mColor = value; }
		void setColor(chstr value) { mColor.set(value); }

		bool setProperty(chstr name, chstr value);
		
	protected:
		april::Color mColor;
		
		void OnDraw(gvec2 offset = gvec2());
		
	};
}

#endif
