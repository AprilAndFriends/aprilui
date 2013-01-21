/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
		mSource = source;

		mBlendMode = april::ALPHA_BLEND;
		mVertical = vertical;
		mInvertX = invertX;
		mInvertY = invertY;
		mTextureCoordinatesLoaded = false;
	}
	
	Image::~Image()
	{
	}
	
	void Image::setSourceRect(grect r)
	{
		mSource = r;
		mTextureCoordinatesLoaded = 0;
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
				_tVertexes[0].u = mSource.x / w;				_tVertexes[0].v = mSource.y / h;
				_tVertexes[1].u = (mSource.x + mSource.w) / w;	_tVertexes[1].v = mSource.y / h;
				_tVertexes[2].u = mSource.x / w;				_tVertexes[2].v = (mSource.y + mSource.h) / h;
				_tVertexes[3].u = (mSource.x + mSource.w) / w;	_tVertexes[3].v = (mSource.y + mSource.h) / h;
				if (mInvertX)
				{
					hswap(_tVertexes[0].u, _tVertexes[1].u);
					hswap(_tVertexes[2].u, _tVertexes[3].u);
				}
				if (mInvertY)
				{
					hswap(_tVertexes[0].v, _tVertexes[2].v);
					hswap(_tVertexes[1].v, _tVertexes[3].v);
				}
			}
			else
			{
				_tVertexes[0].u = (mSource.x + mSource.h) / w;	_tVertexes[0].v = mSource.y / h;
				_tVertexes[1].u = (mSource.x + mSource.h) / w;	_tVertexes[1].v = (mSource.y + mSource.w) / h;
				_tVertexes[2].u = mSource.x / w;				_tVertexes[2].v = mSource.y / h;
				_tVertexes[3].u = mSource.x / w;				_tVertexes[3].v = (mSource.y + mSource.w) / h;
				if (mInvertY)
				{
					hswap(_tVertexes[0].u, _tVertexes[2].u);
					hswap(_tVertexes[1].u, _tVertexes[3].u);
				}
				if (mInvertX)
				{
					hswap(_tVertexes[0].v, _tVertexes[1].v);
					hswap(_tVertexes[2].v, _tVertexes[3].v);
				}
			}
		}
	}

	void Image::draw(grect rect)
	{
		draw(rect, april::Color());
	}
	
	void Image::draw(grect rect, april::Color color)
	{
		if (rect.w == -1)
		{
			rect.w = mSource.w;
		}
		if (rect.h == -1)
		{
			rect.h = mSource.h;
		}
		_tVertexes[0].x = rect.x;          _tVertexes[0].y = rect.y;
		_tVertexes[1].x = rect.x + rect.w; _tVertexes[1].y = rect.y;
		_tVertexes[2].x = rect.x;          _tVertexes[2].y = rect.y + rect.h;
		_tVertexes[3].x = rect.x + rect.w; _tVertexes[3].y = rect.y + rect.h;
		
		mTexture->load();
		april::rendersys->setTexture(mTexture->getRenderTexture());
		_tryLoadTexCoords();
			
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setTextureBlendMode(mBlendMode);
		}
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::TriangleStrip, _tVertexes, 4, color);
		}
		else
		{
			april::rendersys->render(april::TriangleStrip, _tVertexes, 4);
		}
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setTextureBlendMode(april::DEFAULT);
		}
	}

	void Image::draw(grect rect, april::Color color, float angle, gvec2 center)
	{
		if (rect.w == -1)
		{
			rect.w = mSource.w;
		}
		if (rect.h == -1)
		{
			rect.h = mSource.h;
		}
		
		_tVertexes[0].x = -center.x;				_tVertexes[0].y = -center.y;
		_tVertexes[1].x = rect.w - center.x;		_tVertexes[1].y = -center.y;
		_tVertexes[2].x = -center.x;				_tVertexes[2].y = rect.h - center.y;
		_tVertexes[3].x = rect.w - center.x;		_tVertexes[3].y = rect.h - center.y;
		
		gtypes::Matrix4 temp_matrix = april::rendersys->getModelviewMatrix();
		april::rendersys->setIdentityTransform();
		april::rendersys->translate(rect.x + center.x, rect.y + center.y);
		april::rendersys->rotate(angle);

		mTexture->load();
		april::rendersys->setTexture(mTexture->getRenderTexture());
		_tryLoadTexCoords();
		
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setTextureBlendMode(mBlendMode);
		}
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::TriangleStrip, _tVertexes, 4, color);
		}
		else
		{
			april::rendersys->render(april::TriangleStrip, _tVertexes, 4);
		}
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setTextureBlendMode(april::DEFAULT);
		}
		april::rendersys->setModelviewMatrix(temp_matrix);
	}

	void Image::draw(grect rect, april::Color color, float angle)
	{
		draw(rect, color, angle, gvec2(rect.w / 2, rect.h / 2));
	}

}
