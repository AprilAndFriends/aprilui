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
/// Defines a base class for all special image classes and a basic image class.

#ifndef APRILUI_IMAGE_H
#define APRILUI_IMAGE_H

#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

#define APRILUI_IMAGE_NAME_NULL "null"

namespace aprilui
{
	class Dataset;
	class Texture;
	
	class apriluiExport Image
	{
	public:
		friend class Dataset;

		Image(Texture* texture, chstr name, grect source, bool vertical = false, bool invertX = false, bool invertY = false);
		Image(Image& img, chstr name);
		virtual ~Image();
		
		virtual void draw(grect rect, april::Color color = april::Color::White);
		void draw(grect rect, april::Color color, float angle);
		
		HL_DEFINE_GET(Dataset*, dataset, Dataset);
		HL_DEFINE_GET(Texture*, texture, Texture);
		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(grect, srcRect, SrcRect);
		HL_DEFINE_IS(vertical, Vertical);
		HL_DEFINE_IS(invertedX, InvertedX);
		HL_DEFINE_IS(invertedY, InvertedY);
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_GETSET(april::ColorMode, colorMode, ColorMode);
		HL_DEFINE_GETSET(float, colorModeFactor, ColorModeFactor);
		hstr getFullName();
		void setSrcRect(grect value);
		
	protected:
		Dataset* dataset;
		Texture* texture;
		hstr name;
		hstr imageName;
		grect srcRect;
		april::BlendMode blendMode;
		april::ColorMode colorMode;
		float colorModeFactor;
		bool vertical;
		bool invertedX;
		bool invertedY;

		bool _textureCoordinatesLoaded;
		
		void _tryLoadTexCoords();
		
	private:
		april::TexturedVertex _tVertices[4];
		
	};

}
#endif
