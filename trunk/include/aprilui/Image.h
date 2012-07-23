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
/// Defines a base class for all special image classes and a basic image class.

#ifndef APRILUI_IMAGE_H
#define APRILUI_IMAGE_H

#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Texture;
	
	class apriluiExport Image
	{
	public:
		Image(Texture* texture, chstr name, grect source, bool vertical = false, bool invertX = false, bool invertY = false);
		Image(Image& img, chstr name);
		virtual ~Image();
		
		virtual void draw(grect rect, april::Color color = APRIL_COLOR_WHITE);
		void draw(grect rect, april::Color color, float angle);
		
		Texture* getTexture() const { return this->mTexture; }
		hstr getName() const { return this->mName; }
		hstr getImageName() const { return this->mImageName; }
		grect getSrcRect() const { return this->mSrcRect; }
		void setSrcRect(grect value);
		bool isVertical() { return this->mVertical; };
		bool isXInverted() { return this->mInvertX; };
		bool isYInverted() { return this->mInvertY; };
		
		april::BlendMode getBlendMode() { return this->mBlendMode; }
		void setBlendMode(april::BlendMode mode) { this->mBlendMode = mode; }
		
	protected:
		Texture* mTexture;
		hstr mName;
		hstr mImageName;
		grect mSrcRect;
		april::BlendMode mBlendMode;
		bool mVertical;
		bool mInvertX;
		bool mInvertY;
		bool mTextureCoordinatesLoaded;

		void _tryLoadTexCoords();

	private:
		april::TexturedVertex _tVertices[4];
		
	};

}
#endif
