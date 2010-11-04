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
#include <hltypes/util.h>

#include "Exception.h"
#include "Image.h"
#include "Util.h"

namespace AprilUI
{
	April::TexturedVertex tVertices[4];
	April::PlainVertex pVertices[4];
	
	Image::Image(April::Texture* texture, chstr name, grect source, bool vertical, bool invertX, bool invertY)
	{
		mTexture = texture;
		mName = name;
		int index = name.find("/") + 1;
		mImageName = name(index, name.size() - index); // the name without the dataset's name prefix
		mSource = source;

		mBlendMode = April::ALPHA_BLEND;
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
			float t;
			if (!mVertical)
			{
				tVertices[0].u = mSource.x / w;               tVertices[0].v = mSource.y / h;
				tVertices[1].u = (mSource.x + mSource.w) / w; tVertices[1].v = mSource.y / h;
				tVertices[2].u = mSource.x / w;               tVertices[2].v = (mSource.y + mSource.h) / h;
				tVertices[3].u = (mSource.x + mSource.w) / w; tVertices[3].v = (mSource.y + mSource.h) / h;
				if (mInvertX)
				{
					t = tVertices[0].u; tVertices[0].u = tVertices[1].u; tVertices[1].u = t;
					t = tVertices[2].u; tVertices[2].u = tVertices[3].u; tVertices[3].u = t;
				}
				if (mInvertY)
				{
					t = tVertices[0].v; tVertices[0].v = tVertices[2].v; tVertices[2].v = t;
					t = tVertices[1].v; tVertices[1].v = tVertices[3].v; tVertices[3].v = t;
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
					t = tVertices[0].u; tVertices[0].u = tVertices[2].u; tVertices[2].u = t;
					t = tVertices[1].u; tVertices[1].u = tVertices[3].u; tVertices[3].u = t;
				}
				if (mInvertX)
				{
					t = tVertices[0].v; tVertices[0].v = tVertices[1].v; tVertices[1].v = t;
					t = tVertices[2].v; tVertices[2].v = tVertices[3].v; tVertices[3].v = t;
				}
			}

		}
	}

	void Image::draw(float x, float y, float w, float h)
	{
		draw(x, y, w, h, 1, 1, 1, 1);
	}
	
	void Image::draw(float x, float y, float w, float h, float r, float g, float b, float a)
	{
		if (w == -1)
		{
			w = mSource.w;
		}
		if (h == -1)
		{
			h = mSource.h;
		}
		tVertices[0].x = x;     tVertices[0].y = y;
		tVertices[1].x = x + w; tVertices[1].y = y;
		tVertices[2].x = x;     tVertices[2].y = y + h;
		tVertices[3].x = x + w; tVertices[3].y = y + h;
		
		April::rendersys->setTexture(mTexture);
		_updateTexCoords();
			
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(mBlendMode);
		}
		if (r != 1 || g != 1 || b != 1 || a != 1)
		{
			April::rendersys->render(April::TriangleStrip, tVertices, 4, r, g, b, a);
		}
		else
		{
			April::rendersys->render(April::TriangleStrip, tVertices, 4);
		}
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(April::DEFAULT);
		}
	}

	void Image::draw(float x, float y, float w, float h, float r, float g, float b, float a, float angle)
	{
		if (w == -1)
		{
			w = mSource.w;
		}
		if (h == -1)
		{
			h = mSource.h;
		}
		
		tVertices[0].x = -w / 2; tVertices[0].y = -h / 2;
		tVertices[1].x =  w / 2; tVertices[1].y = -h / 2;
		tVertices[2].x = -w / 2; tVertices[2].y =  h / 2;
		tVertices[3].x =  w / 2; tVertices[3].y =  h / 2;
		
		gtypes::Matrix4 temp_matrix = April::rendersys->getModelviewMatrix();
		April::rendersys->setIdentityTransform();
		April::rendersys->translate(x + w / 2, y + h / 2);
		April::rendersys->rotate(angle);
		April::rendersys->setTexture(mTexture);
		_updateTexCoords();
		
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(mBlendMode);
		}
		if (r != 1 || g != 1 || b != 1 || a != 1)
		{
			April::rendersys->render(April::TriangleStrip, tVertices, 4, r, g, b, a);
		}
		else
		{
			April::rendersys->render(April::TriangleStrip, tVertices, 4);
		}
		if (mBlendMode != April::ALPHA_BLEND)
		{
			April::rendersys->setBlendMode(April::DEFAULT);
		}
		April::rendersys->setModelviewMatrix(temp_matrix);
	}

	void Image::draw(float x, float y, float w, float h, float angle)
	{
		draw(x, y, w, h, 1, 1, 1, 1, angle);
	}

	April::Texture* Image::getTexture()
	{
		return mTexture;
	}
	
}
