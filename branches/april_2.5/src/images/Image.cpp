/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
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
#include "Dataset.h"
#include "Exception.h"
#include "Image.h"
#include "Texture.h"

namespace aprilui
{
	Image::Image(Texture* texture, chstr name, grect source, bool vertical, bool invertX, bool invertY)
	{
		this->mTexture = texture;
		this->mName = name;
		this->mSrcRect = source;
		this->mBlendMode = april::ALPHA_BLEND;
		this->mVertical = vertical;
		this->mInvertX = invertX;
		this->mInvertY = invertY;
		this->mTextureCoordinatesLoaded = false;
		this->mDataset = NULL;
	}

	Image::Image(Image& img, chstr name)
	{
		this->mTexture = img.mTexture;
		this->mName = name;
		this->mSrcRect = img.mSrcRect;
		this->mBlendMode = img.mBlendMode;
		this->mVertical = img.mVertical;
		this->mInvertX = img.mInvertX;
		this->mInvertY = img.mInvertY;
		this->mTextureCoordinatesLoaded = false;
	}
	
	Image::~Image()
	{
	}
	
	hstr Image::getFullName() const
	{
		return (mDataset != NULL ? mDataset->getName() + "." + mName : mName);
	}
	
	void Image::setSrcRect(grect value)
	{
		this->mSrcRect = value;
		this->mTextureCoordinatesLoaded = false;
	}
	
	void Image::_tryLoadTexCoords()
	{
		if (!this->mTextureCoordinatesLoaded && this->mTexture != NULL && this->mTexture->getWidth() > 0 && this->mTexture->getHeight() > 0)
		{
			this->mTextureCoordinatesLoaded = true;
			float iw = 1.0f / this->mTexture->getWidth();
			float ih = 1.0f / this->mTexture->getHeight();
			if (!this->mVertical)
			{
				this->_tVertices[0].u = this->_tVertices[2].u = this->mSrcRect.left() * iw;
				this->_tVertices[0].v = this->_tVertices[1].v = this->mSrcRect.top() * ih;
				this->_tVertices[1].u = this->_tVertices[3].u = this->mSrcRect.right() * iw;
				this->_tVertices[2].v = this->_tVertices[3].v = this->mSrcRect.bottom() * ih;
				if (this->mInvertX)
				{
					hswap(this->_tVertices[0].u, this->_tVertices[1].u);
					hswap(this->_tVertices[2].u, this->_tVertices[3].u);
				}
				if (this->mInvertY)
				{
					hswap(this->_tVertices[0].v, this->_tVertices[2].v);
					hswap(this->_tVertices[1].v, this->_tVertices[3].v);
				}
			}
			else
			{
				this->_tVertices[0].u = this->_tVertices[1].u = (this->mSrcRect.x + this->mSrcRect.h) * iw;
				this->_tVertices[0].v = this->_tVertices[2].v = this->mSrcRect.y * ih;
				this->_tVertices[1].v = this->_tVertices[3].v = (this->mSrcRect.y + this->mSrcRect.w) * ih;
				this->_tVertices[2].u = this->_tVertices[3].u = this->mSrcRect.x * iw;
				if (this->mInvertY)
				{
					hswap(this->_tVertices[0].u, this->_tVertices[2].u);
					hswap(this->_tVertices[1].u, this->_tVertices[3].u);
				}
				if (this->mInvertX)
				{
					hswap(this->_tVertices[0].v, this->_tVertices[1].v);
					hswap(this->_tVertices[2].v, this->_tVertices[3].v);
				}
			}
		}
	}

	void Image::draw(grect rect, april::Color color)
	{
		this->_tVertices[0].x = this->_tVertices[2].x = rect.left();
		this->_tVertices[0].y = this->_tVertices[1].y = rect.top();
		this->_tVertices[1].x = this->_tVertices[3].x = rect.right();
		this->_tVertices[2].y = this->_tVertices[3].y = rect.bottom();
		
		this->mTexture->load();
		april::rendersys->setTexture(this->mTexture->getRenderTexture());
		this->_tryLoadTexCoords();
			
		april::rendersys->setTextureBlendMode(this->mBlendMode);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::TriangleStrip, this->_tVertices, 4, color);
		}
		else
		{
			april::rendersys->render(april::TriangleStrip, this->_tVertices, 4);
		}
		april::rendersys->setTextureBlendMode(april::DEFAULT);
	}
	
	void Image::draw(grect rect, april::Color color, float angle)
	{
		if (angle == 0.0f)
		{
			this->draw(rect, color);
			return;
		}
		gmat4 originalMatrix = april::rendersys->getModelviewMatrix();
		april::rendersys->translate(rect.w * 0.5f, rect.h * 0.5f);
		april::rendersys->rotate(angle);
		this->draw(rect - rect.getSize() * 0.5f, color);
		april::rendersys->setModelviewMatrix(originalMatrix);
	}

}
