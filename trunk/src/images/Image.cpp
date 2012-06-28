/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.8
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
#include "Texture.h"

namespace aprilui
{
	Image::Image(Texture* texture, chstr name, grect source, bool vertical, bool invertX, bool invertY)
	{
		mTexture = texture;
		mName = name;
		int index = name.find("/") + 1;
		mImageName = name(index, name.size() - index); // the name without the dataset's name prefix
		mSrcRect = source;
		mBlendMode = april::ALPHA_BLEND;
		mVertical = vertical;
		mInvertX = invertX;
		mInvertY = invertY;
		mTextureCoordinatesLoaded = false;
	}

	Image::Image(Image& img, chstr name)
	{
		mTexture = img.mTexture;
		mName = name;
		mImageName = img.mImageName;
		mSrcRect = img.mSrcRect;
		mBlendMode = img.mBlendMode;
		mVertical = img.mVertical;
		mInvertX = img.mInvertX;
		mInvertY = img.mInvertY;
		mTextureCoordinatesLoaded = false;
	}
	
	Image::~Image()
	{
	}
	
	void Image::setSrcRect(grect value)
	{
		mSrcRect = value;
		mTextureCoordinatesLoaded = false;
	}
	
	void Image::_tryLoadTexCoords()
	{
		if (!mTextureCoordinatesLoaded && mTexture != NULL && mTexture->getWidth() > 0 && mTexture->getHeight() > 0)
		{
			mTextureCoordinatesLoaded = true;
			float w = (float)mTexture->getWidth();
			float h = (float)mTexture->getHeight();
			if (!mVertical)
			{
				_tVertices[0].u = mSrcRect.x / w;					_tVertices[0].v = mSrcRect.y / h;
				_tVertices[1].u = (mSrcRect.x + mSrcRect.w) / w;	_tVertices[1].v = mSrcRect.y / h;
				_tVertices[2].u = mSrcRect.x / w;					_tVertices[2].v = (mSrcRect.y + mSrcRect.h) / h;
				_tVertices[3].u = (mSrcRect.x + mSrcRect.w) / w;	_tVertices[3].v = (mSrcRect.y + mSrcRect.h) / h;
				if (mInvertX)
				{
					hswap(_tVertices[0].u, _tVertices[1].u);
					hswap(_tVertices[2].u, _tVertices[3].u);
				}
				if (mInvertY)
				{
					hswap(_tVertices[0].v, _tVertices[2].v);
					hswap(_tVertices[1].v, _tVertices[3].v);
				}
			}
			else
			{
				_tVertices[0].u = (mSrcRect.x + mSrcRect.h) / w;	_tVertices[0].v = mSrcRect.y / h;
				_tVertices[1].u = (mSrcRect.x + mSrcRect.h) / w;	_tVertices[1].v = (mSrcRect.y + mSrcRect.w) / h;
				_tVertices[2].u = mSrcRect.x / w;					_tVertices[2].v = mSrcRect.y / h;
				_tVertices[3].u = mSrcRect.x / w;					_tVertices[3].v = (mSrcRect.y + mSrcRect.w) / h;
				if (mInvertY)
				{
					hswap(_tVertices[0].u, _tVertices[2].u);
					hswap(_tVertices[1].u, _tVertices[3].u);
				}
				if (mInvertX)
				{
					hswap(_tVertices[0].v, _tVertices[1].v);
					hswap(_tVertices[2].v, _tVertices[3].v);
				}
			}
		}
	}

	void Image::draw(grect rect, april::Color color)
	{
		_tVertices[0].x = _tVertices[2].x = rect.left();
		_tVertices[0].y = _tVertices[1].y = rect.top();
		_tVertices[1].x = _tVertices[3].x = rect.right();
		_tVertices[2].y = _tVertices[3].y = rect.bottom();
		
		mTexture->load();
		april::rendersys->setTexture(mTexture->getRenderTexture());
		_tryLoadTexCoords();
			
		april::rendersys->setTextureBlendMode(mBlendMode);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::TriangleStrip, _tVertices, 4, color);
		}
		else
		{
			april::rendersys->render(april::TriangleStrip, _tVertices, 4);
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

}
