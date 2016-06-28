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
		int index = name.indexOf("/") + 1;
		mImageName = name(index, name.size() - index); // the name without the dataset's name prefix
		mSource = source;

		mBlendMode = april::BM_DEFAULT;
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
		if (mSource != r)
		{
			mSource = r;
			mTextureCoordinatesLoaded = false;
		}
	}

	void Image::setClipRect(grect value)
	{
		if (mClipRect != value)
		{
			mClipRect = value;
			mTextureCoordinatesLoaded = false;
		}
	}

	void Image::_tryLoadTexCoords()
	{
		if (!this->mTextureCoordinatesLoaded && this->mTexture != NULL && this->mTexture->getWidth() > 0 && this->mTexture->getHeight() > 0)
		{
			this->mTextureCoordinatesLoaded = true;
			float iw = 1.0f / this->mTexture->getWidth();
			float ih = 1.0f / this->mTexture->getHeight();
			grect rect = this->_makeClippedSrcRect();
			gvec2 topLeft;
			gvec2 topRight;
			gvec2 bottomLeft;
			gvec2 bottomRight;
			if (!this->mVertical)
			{
				topLeft.x = bottomLeft.x = rect.left() * iw;
				topLeft.y = topRight.y = rect.top() * ih;
				topRight.x = bottomRight.x = rect.right() * iw;
				bottomLeft.y = bottomRight.y = rect.bottom() * ih;
			}
			else
			{
				topLeft.x = topRight.x = (rect.x + rect.h) * iw;
				topLeft.y = bottomLeft.y = rect.y * ih;
				topRight.y = bottomRight.y = (rect.y + rect.w) * ih;
				bottomLeft.x = bottomRight.x = rect.x * iw;
			}
			if (this->mInvertX)
			{
				hswap(topLeft.x, topRight.x);
				hswap(bottomLeft.x, bottomRight.x);
			}
			if (this->mInvertY)
			{
				hswap(topLeft.y, bottomLeft.y);
				hswap(topRight.y, bottomRight.y);
			}
			this->_tVertexes[0].u = topLeft.x;
			this->_tVertexes[0].v = topLeft.y;
			this->_tVertexes[1].u = topRight.x;
			this->_tVertexes[1].v = topRight.y;
			this->_tVertexes[2].u = bottomLeft.x;
			this->_tVertexes[2].v = bottomLeft.y;
			this->_tVertexes[3] = this->_tVertexes[1];
			this->_tVertexes[4] = this->_tVertexes[2];
			this->_tVertexes[5].u = bottomRight.x;
			this->_tVertexes[5].v = bottomRight.y;
		}
	}

	grect Image::_makeClippedSrcRect()
	{
		if (this->mClipRect.w > 0.0f && this->mClipRect.h > 0.0f)
		{
			if (this->mVertical)
			{
				grect rect = this->mClipRect;
				grect srcRect = this->mSource;
				hswap(rect.x, rect.y);
				hswap(rect.w, rect.h);
				hswap(srcRect.w, srcRect.h);
				rect.x = srcRect.w - (rect.x + rect.w);
				srcRect.clip(rect + srcRect.getPosition());
				hswap(srcRect.w, srcRect.h);
				return srcRect;
			}
			return this->mSource.clipped(this->mClipRect + this->mSource.getPosition());
		}
		return this->mSource;
	}

	void Image::draw(grect rect)
	{
		draw(rect, april::Color::White);
	}
	
	void Image::draw(grect rect, april::Color color)
	{
		if (rect.w < 0.0f)
		{
			rect.w = mSource.w;
		}
		if (rect.h < 0.0f)
		{
			rect.h = mSource.h;
		}
		if (this->mClipRect.w > 0.0f && this->mClipRect.h > 0.0f)
		{
			gvec2 sizeRatio = rect.getSize() / this->mSource.getSize();
			rect += this->mClipRect.getPosition() * sizeRatio;
			rect.setSize(this->mClipRect.getSize() * sizeRatio);
		}
		this->_tVertexes[0].x = this->_tVertexes[2].x = this->_tVertexes[4].x = rect.left();
		this->_tVertexes[0].y = this->_tVertexes[1].y = this->_tVertexes[3].y = rect.top();
		this->_tVertexes[1].x = this->_tVertexes[3].x = this->_tVertexes[5].x = rect.right();
		this->_tVertexes[2].y = this->_tVertexes[4].y = this->_tVertexes[5].y = rect.bottom();
		if (this->mTexture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->mTexture->load();
		}
		april::rendersys->setTexture(this->mTexture->getRenderTexture());
		this->_tryLoadTexCoords();
		april::rendersys->setBlendMode(this->mBlendMode);
		april::rendersys->setColorMode(april::CM_DEFAULT);
		april::rendersys->render(april::RO_TRIANGLE_LIST, this->_tVertexes, 6, color);
		april::rendersys->setBlendMode(april::BM_DEFAULT);
		april::rendersys->setColorMode(april::CM_DEFAULT);
	}

	void Image::draw(grect rect, april::Color color, float angle, gvec2 center)
	{
		if (rect.w < 0.0f)
		{
			rect.w = mSource.w;
		}
		if (rect.h < 0.0f)
		{
			rect.h = mSource.h;
		}
		if (this->mClipRect.w > 0.0f && this->mClipRect.h > 0.0f)
		{
			gvec2 sizeRatio = rect.getSize() / this->mSource.getSize();
			rect += this->mClipRect.getPosition() * sizeRatio;
			rect.setSize(this->mClipRect.getSize() * sizeRatio);
		}
		this->_tVertexes[0].x = this->_tVertexes[2].x = this->_tVertexes[4].x = rect.left();
		this->_tVertexes[0].y = this->_tVertexes[1].y = this->_tVertexes[3].y = rect.top();
		this->_tVertexes[1].x = this->_tVertexes[3].x = this->_tVertexes[5].x = rect.right();
		this->_tVertexes[2].y = this->_tVertexes[4].y = this->_tVertexes[5].y = rect.bottom();
		gtypes::Matrix4 tempMatrix = april::rendersys->getModelviewMatrix();
		april::rendersys->setIdentityTransform();
		april::rendersys->translate(rect.x + center.x, rect.y + center.y);
		april::rendersys->rotate(angle);

		if (this->mTexture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->mTexture->load();
		}
		april::rendersys->setTexture(this->mTexture->getRenderTexture());
		this->_tryLoadTexCoords();
		april::rendersys->setBlendMode(this->mBlendMode);
		april::rendersys->setColorMode(april::CM_DEFAULT);
		april::rendersys->render(april::RO_TRIANGLE_LIST, this->_tVertexes, 6, color);
		april::rendersys->setBlendMode(april::BM_DEFAULT);
		april::rendersys->setColorMode(april::CM_DEFAULT);
		april::rendersys->setModelviewMatrix(tempMatrix);
	}

	void Image::draw(grect rect, april::Color color, float angle)
	{
		draw(rect, color, angle, gvec2(rect.w / 2, rect.h / 2));
	}

}
