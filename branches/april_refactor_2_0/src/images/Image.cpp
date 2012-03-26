/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hltypesUtil.h>

#include "apriluiUtil.h"
#include "Exception.h"
#include "Image.h"

namespace aprilui
{
	april::TexturedVertex tVertices[4];
	
	Image::Image(april::Texture* texture, chstr name, grect source, bool vertical, bool invertX, bool invertY)
	{
		mTexture = texture;
		mName = name;
		int index = name.find("/") + 1;
		mImageName = name(index, name.size() - index); // the name without the dataset's name prefix
		mSrcRect = source;

		mBlendMode = april::ALPHA_BLEND;
		mVertical = vertical;
		mUnloadedFlag = false;
		mInvertX = invertX;
		mInvertY = invertY;
	}

	Image::Image(Image& img, chstr name)
	{
		mTexture = img.mTexture;
		mName = name;
		mImageName = img.mImageName;
		mSrcRect = img.mSrcRect;
		mBlendMode = img.mBlendMode;
		mVertical = img.mVertical;
		mUnloadedFlag = img.mUnloadedFlag;
		mInvertX = img.mInvertX;
		mInvertY = img.mInvertY;
	}
	
	Image::~Image()
	{
	}
	
	void Image::_updateTexCoords()
	{
		if (mTexture != NULL) // mTexture is allowed to be null, since we have derived classes
		{
 			if (!mTexture->isLoaded())
			{
				mUnloadedFlag = true;
				return;
			}
			int w = mTexture->getWidth();
			int h = mTexture->getHeight();
			if (!mVertical)
			{
				tVertices[0].u = mSrcRect.x / w;               tVertices[0].v = mSrcRect.y / h;
				tVertices[1].u = (mSrcRect.x + mSrcRect.w) / w; tVertices[1].v = mSrcRect.y / h;
				tVertices[2].u = mSrcRect.x / w;               tVertices[2].v = (mSrcRect.y + mSrcRect.h) / h;
				tVertices[3].u = (mSrcRect.x + mSrcRect.w) / w; tVertices[3].v = (mSrcRect.y + mSrcRect.h) / h;
				if (mInvertX)
				{
					hswap(tVertices[0].u, tVertices[1].u);
					hswap(tVertices[2].u, tVertices[3].u);
				}
				if (mInvertY)
				{
					hswap(tVertices[0].v, tVertices[2].v);
					hswap(tVertices[1].v, tVertices[3].v);
				}
			}
			else
			{
				tVertices[0].u = (mSrcRect.x + mSrcRect.h) / w; tVertices[0].v = mSrcRect.y / h;
				tVertices[1].u = (mSrcRect.x + mSrcRect.h) / w; tVertices[1].v = (mSrcRect.y + mSrcRect.w) / h;
				tVertices[2].u = (mSrcRect.x) / w;             tVertices[2].v = mSrcRect.y / h;
				tVertices[3].u = (mSrcRect.x) / w;             tVertices[3].v = (mSrcRect.y + mSrcRect.w) / h;
				if (mInvertY)
				{
					hswap(tVertices[0].u, tVertices[2].u);
					hswap(tVertices[1].u, tVertices[3].u);
				}
				if (mInvertX)
				{
					hswap(tVertices[0].v, tVertices[1].v);
					hswap(tVertices[2].v, tVertices[3].v);
				}
			}

		}
	}

	void Image::draw(grect rect, april::Color color)
	{
		tVertices[0].x = rect.x;          tVertices[0].y = rect.y;
		tVertices[1].x = rect.x + rect.w; tVertices[1].y = rect.y;
		tVertices[2].x = rect.x;          tVertices[2].y = rect.y + rect.h;
		tVertices[3].x = rect.x + rect.w; tVertices[3].y = rect.y + rect.h;
		
		april::rendersys->setTexture(mTexture);
		_updateTexCoords();
			
		april::rendersys->setTextureBlendMode(mBlendMode);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::TriangleStrip, tVertices, 4, color);
		}
		else
		{
			april::rendersys->render(april::TriangleStrip, tVertices, 4);
		}
		april::rendersys->setTextureBlendMode(april::DEFAULT);
	}
	
	void Image::draw(grect rect, april::Color color, float angle)
	{
		if (angle == 0.0f)
		{
			draw(rect, color);
			return;
		}
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		april::rendersys->translate(rect.w / 2, rect.h / 2);
		april::rendersys->rotate(angle);
		draw(rect - rect.getSize() / 2, color);
		april::rendersys->setModelviewMatrix(originalMatrix);
	}

	april::Texture* Image::getTexture()
	{
		return mTexture;
	}
	
}
