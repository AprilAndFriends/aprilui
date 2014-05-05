/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a special RAM texture.

#ifndef APRIL_RAM_TEXTURE_H
#define APRIL_RAM_TEXTURE_H

#include <hltypes/hstring.h>
#include <april/Color.h>
#include <april/Texture.h>
#include "apriluiExport.h"

namespace aprilui
{
	class Image;
	
	class apriluiExport RamTexture
	{
		hstr filename;
		
		int width, height;
	public:
		RamTexture(chstr filename);
		RamTexture(int w, int h);
		~RamTexture();
		bool load();
		void unload();

		int getWidth() { return this->width; }
		int getHeight() { return this->height; }

		bool isLoaded();

		april::Color getInterpolatedPixel(float x, float y);
		april::Color getPixel(int x, int y);
		bool setPixel(int x, int y, april::Color color);		
	protected:
		april::Image* source;
	};

}

#endif
