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
/// Defines a special image that is composed of other images.

#ifndef APRILUI_COMPOSITE_IMAGE_H
#define APRILUI_COMPOSITE_IMAGE_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>

#include "BaseImage.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport CompositeImage : public BaseImage
	{
	public:
		typedef std::pair<BaseImage*, grect> ImageRef;

		CompositeImage(chstr name, gvec2 size);
		DEPRECATED_ATTRIBUTE CompositeImage(chstr name, CompositeImage& base);
		CompositeImage(CompositeImage& other, chstr name);
		~CompositeImage();
		
		HL_DEFINE_GETSET(gvec2, size, SrcSize);
		HL_DEFINE_GETSET(float, size.x, SrcWidth);
		HL_DEFINE_GETSET(float, size.y, SrcHeight);
		HL_DEFINE_ISSET(restoreClipRects, RestoreClipRects);
		inline const harray<ImageRef>& getImages() { return this->images; }
		
		void addImageRef(BaseImage* image, grect rect);
		void clearImages();
		
		void draw(grect rect, april::Color color);
		void draw(harray<april::TexturedVertex> vertices, april::Color color);
		
	protected:
		gvec2 size;
		bool restoreClipRects;
		harray<ImageRef> images;
		
	};
	
}
#endif
