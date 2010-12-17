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
#include <hltypes/util.h>

#include "Exception.h"
#include "Image.h"
#include "Util.h"

namespace aprilui
{
	april::TexturedVertex tVertices[4];
	
	Image::Image(april::Texture* texture, chstr name, grect source, bool vertical, bool invertX, bool invertY)
	{
		mTexture = texture;
		mName = name;
		int index = name.find("/") + 1;
		mImageName = name(index, name.size() - index); // the name without the dataset's name prefix
		mSource = source;

		mBlendMode = april::ALPHA_BLEND;
		mVertical = vertical;
		mUnloadedFlag = false;
		mInvertX = invertX;
		mInvertY = invertY;
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
				tVertices[0].u = mSource.x / w;               tVertices[0].v = mSource.y / h;
				tVertices[1].u = (mSource.x + mSource.w) / w; tVertices[1].v = mSource.y / h;
				tVertices[2].u = mSource.x / w;               tVertices[2].v = (mSource.y + mSource.h) / h;
				tVertices[3].u = (mSource.x + mSource.w) / w; tVertices[3].v = (mSource.y + mSource.h) / h;
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
				tVertices[0].u = (mSource.x + mSource.h) / w; tVertices[0].v = mSource.y / h;
				tVertices[1].u = (mSource.x + mSource.h) / w; tVertices[1].v = (mSource.y + mSource.w) / h;
				tVertices[2].u = (mSource.x) / w;             tVertices[2].v = mSource.y / h;
				tVertices[3].u = (mSource.x) / w;             tVertices[3].v = (mSource.y + mSource.w) / h;
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

	void Image::draw(grect rect)
	{
		draw(rect, april::Color::WHITE);
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
		tVertices[0].x = rect.x;          tVertices[0].y = rect.y;
		tVertices[1].x = rect.x + rect.w; tVertices[1].y = rect.y;
		tVertices[2].x = rect.x;          tVertices[2].y = rect.y + rect.h;
		tVertices[3].x = rect.x + rect.w; tVertices[3].y = rect.y + rect.h;
		
		april::rendersys->setTexture(mTexture);
		_updateTexCoords();
			
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setBlendMode(mBlendMode);
		}
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::TriangleStrip, tVertices, 4, color.r_float(), color.g_float(), color.b_float(), color.a_float());
		}
		else
		{
			april::rendersys->render(april::TriangleStrip, tVertices, 4);
		}
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setBlendMode(april::DEFAULT);
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
		
		tVertices[0].x = -center.x;			tVertices[0].y = -center.y;
		tVertices[1].x = rect.w - center.x;	tVertices[1].y = -center.y;
		tVertices[2].x = -center.x;			tVertices[2].y = rect.h - center.y;
		tVertices[3].x = rect.w - center.x;	tVertices[3].y = rect.h - center.y;
		
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		april::rendersys->setIdentityTransform();
		april::rendersys->translate(rect.x + center.x, rect.y + center.y);
		april::rendersys->rotate(angle);
		april::rendersys->setTexture(mTexture);
		_updateTexCoords();
		
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setBlendMode(mBlendMode);
		}
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::TriangleStrip, tVertices, 4, color.r_float(), color.g_float(), color.b_float(), color.a_float());
		}
		else
		{
			april::rendersys->render(april::TriangleStrip, tVertices, 4);
		}
		if (mBlendMode != april::ALPHA_BLEND)
		{
			april::rendersys->setBlendMode(april::DEFAULT);
		}
		april::rendersys->setModelviewMatrix(originalMatrix);
	}

	void Image::draw(grect rect, april::Color color, float angle)
	{
		draw(rect, color, angle, gvec2(rect.w / 2, rect.h / 2));
	}

	april::Texture* Image::getTexture()
	{
		return mTexture;
	}
	
}
