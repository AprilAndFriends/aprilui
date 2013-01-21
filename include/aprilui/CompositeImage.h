/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
