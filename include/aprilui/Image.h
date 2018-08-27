/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		Image(Texture* texture, chstr name, grectf source, bool vertical = false, bool invertX = false, bool invertY = false);
		virtual ~Image();
		
		virtual void draw(grectf rect);
		virtual void draw(grectf rect, april::Color color);
		virtual void draw(grectf rect, april::Color color, float angle, gvec2f center);
		virtual void draw(grectf rect, april::Color color, float angle);
		
		bool isVertical() { return mVertical; };
		bool isXInverted() { return mInvertX; };
		bool isYInverted() { return mInvertY; };

		Texture* getTexture() { return mTexture; }
	
		hstr getName() const { return mName; }
		hstr getImageName() const { return mImageName; }
		const grectf& getSource() const { return mSource; }
		void setSourceRect(grectf r);
		
		april::BlendMode getBlendMode() { return mBlendMode; }
		void setBlendMode(april::BlendMode mode) { mBlendMode = mode; }
		
	protected:
		Texture* mTexture;
		hstr mName;
		hstr mImageName;
		grectf mSource;
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
