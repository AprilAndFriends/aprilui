/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
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
		
	public:
		CompositeImage(chstr name, float w, float h);
		CompositeImage(chstr name, CompositeImage& base);
		~CompositeImage();
		
		void addImageRef(Image* image, grect rect);
		
		void draw(grect rect, april::Color color);
		
		const harray<ImageRef>& getImageList() { return this->mImages; }
		
	protected:
		harray<ImageRef> mImages;
		
	};
	
}
#endif