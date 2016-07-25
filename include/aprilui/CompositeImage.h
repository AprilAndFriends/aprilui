/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_COMPOSITE_IMAGE_H
#define APRILUI_COMPOSITE_IMAGE_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport CompositeImage : public Image
	{
	public:
		struct ImageRef
		{
			Image* image;
			grect rect;
		};
		
	public:
		CompositeImage(chstr name, float w, float h);
		CompositeImage(chstr name, CompositeImage& base);
		
		void addImageRef(Image* image, grect rect);
		
		void draw(grect rect, april::Color color);
		void draw(grect rect, april::Color color, float angle, gvec2 center);
		
		harray<ImageRef>& getImageList() { return mImages; }
		
	protected:
		harray<ImageRef> mImages;
		
	};
	
}
#endif
