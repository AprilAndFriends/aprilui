/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
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
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Texture.h"

namespace april
{
	class Texture;
}

namespace aprilui
{
	class apriluiExport Image
	{
	public:
		Image(Texture* texture, chstr name, grect source, bool vertical = false, bool invertX = false, bool invertY = false);
		Image(Image& img, chstr name);
		virtual ~Image();
		
		virtual void draw(grect rect, april::Color color = APRIL_COLOR_WHITE);
		void draw(grect rect, april::Color color, float angle);
		
		Texture* getTexture() const { return mTexture; }
		hstr getName() const { return mName; }
		hstr getImageName() const { return mImageName; }
		grect getSrcRect() const { return mSrcRect; }
		void setSrcRect(grect value) { mSrcRect = value; }
		bool isVertical() { return mVertical; };
		bool isXInverted() { return mInvertX; };
		bool isYInverted() { return mInvertY; };
        
		april::BlendMode getBlendMode() { return mBlendMode; }
		void setBlendMode(april::BlendMode mode) { mBlendMode = mode; }
		
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
		april::TexturedVertex _tVertexes[4];
		
	};

}
#endif
