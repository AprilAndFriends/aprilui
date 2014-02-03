/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a special image that is composed of other images.

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
		
		CompositeImage(chstr name, float w, float h);
		CompositeImage(chstr name, CompositeImage& base);
		~CompositeImage();
		
		const harray<ImageRef>& getImages() { return this->images; }
		
		void addImageRef(Image* image, grect rect);
        void clearImages();
		
		void draw(grect rect, april::Color color);
		
	protected:
		harray<ImageRef> images;
		
	};
	
}
#endif
