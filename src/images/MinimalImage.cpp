/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hdir.h>
#include <hltypes/hltypesUtil.h>

#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "MinimalImage.h"
#include "Texture.h"

namespace aprilui
{
	harray<PropertyDescription> MinimalImage::_propertyDescriptions;

	MinimalImage::MinimalImage(Texture* texture, chstr name, cgrect source) : BaseImage(name)
	{
		this->texture = texture;
		this->srcRect = source;
		this->_textureCoordinatesLoaded = false;
	}

	MinimalImage::MinimalImage(const MinimalImage& other) : BaseImage(other)
	{
		this->texture = other.texture;
		this->srcRect = other.srcRect;
		for_iter (i, 0, APRILUI_IMAGE_MAX_VERTICES)
		{
			this->vertices[i] = other.vertices[i];
		}
		this->_textureCoordinatesLoaded = false;
	}

	MinimalImage::~MinimalImage()
	{
	}

	MinimalImage* MinimalImage::createInstance(Texture* texture, chstr name, cgrect source)
	{
		return new MinimalImage(texture, name, source);
	}
	
	harray<PropertyDescription> MinimalImage::getPropertyDescriptions() const
	{
		if (MinimalImage::_propertyDescriptions.size() == 0)
		{
			MinimalImage::_propertyDescriptions += PropertyDescription("rect", PropertyDescription::Type::Grect);
			MinimalImage::_propertyDescriptions += PropertyDescription("position", PropertyDescription::Type::Gvec2);
			MinimalImage::_propertyDescriptions += PropertyDescription("x", PropertyDescription::Type::Float);
			MinimalImage::_propertyDescriptions += PropertyDescription("y", PropertyDescription::Type::Float);
			MinimalImage::_propertyDescriptions += PropertyDescription("texture", PropertyDescription::Type::String);
		}
		return (MinimalImage::_propertyDescriptions + BaseImage::getPropertyDescriptions());
	}

	void MinimalImage::setSrcRect(cgrect value)
	{
		if (this->srcRect != value)
		{
			this->srcRect = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcX(float value)
	{
		if (this->srcRect.x != value)
		{
			this->srcRect.x = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcY(float value)
	{
		if (this->srcRect.y != value)
		{
			this->srcRect.y = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcWidth(const float& value)
	{
		if (this->srcRect.w != value)
		{
			this->srcRect.w = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcHeight(const float& value)
	{
		if (this->srcRect.h != value)
		{
			this->srcRect.h = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcPosition(cgvec2 value)
	{
		if (this->srcRect.getPosition() != value)
		{
			this->srcRect.setPosition(value);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcPosition(float x, float y)
	{
		if (this->srcRect.x != x || this->srcRect.y != y)
		{
			this->srcRect.setPosition(x, y);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcSize(cgvec2 value)
	{
		if (this->srcRect.getSize() != value)
		{
			this->srcRect.setSize(value);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcSize(float w, float h)
	{
		if (this->srcRect.w != w || this->srcRect.h != h)
		{
			this->srcRect.setSize(w, h);
			this->_textureCoordinatesLoaded = false;
		}
	}

	hstr MinimalImage::getProperty(chstr name)
	{
		if (name == "rect")					return april::grectToHstr(this->getSrcRect());
		if (name == "position")				return april::gvec2ToHstr(this->getSrcRect().getPosition());
		if (name == "x")					return this->getSrcRect().x;
		if (name == "y")					return this->getSrcRect().y;
		if (name == "texture")
		{
			return (this->texture != NULL ? this->texture->getName() : "");
		}
		return BaseImage::getProperty(name);
	}
	
	bool MinimalImage::setProperty(chstr name, chstr value)
	{
		if		(name == "rect")				this->setSrcRect(april::hstrToGrect(value));
		else if	(name == "position")			this->setSrcPosition(april::hstrToGvec2(value));
		else if	(name == "x")					this->setSrcX(value);
		else if	(name == "y")					this->setSrcY(value);
		else return BaseImage::setProperty(name, value);
		return true;
	}

	void MinimalImage::tryLoadTextureCoordinates()
	{
		if ((!this->_textureCoordinatesLoaded || !this->_clipRectCalculated) && this->texture != NULL && this->texture->getWidth() > 0 && this->texture->getHeight() > 0)
		{
			this->_textureCoordinatesLoaded = true;
			this->_clipRectCalculated = true;
			float iw = 1.0f / this->texture->getWidth();
			float ih = 1.0f / this->texture->getHeight();
			grect rect = this->_makeClippedSrcRect();
			gvec2 topLeft;
			gvec2 topRight;
			gvec2 bottomLeft;
			gvec2 bottomRight;
			topLeft.x = bottomLeft.x = rect.left() * iw;
			topLeft.y = topRight.y = rect.top() * ih;
			topRight.x = bottomRight.x = rect.right() * iw;
			bottomLeft.y = bottomRight.y = rect.bottom() * ih;
			this->vertices[0].u = topLeft.x;
			this->vertices[0].v = topLeft.y;
			this->vertices[1].u = topRight.x;
			this->vertices[1].v = topRight.y;
			this->vertices[2].u = bottomLeft.x;
			this->vertices[2].v = bottomLeft.y;
			this->vertices[3] = this->vertices[1];
			this->vertices[4] = this->vertices[2];
			this->vertices[5].u = bottomRight.x;
			this->vertices[5].v = bottomRight.y;
		}
	}

	grect MinimalImage::_makeClippedSrcRect() const
	{
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			return this->srcRect.clipped(this->clipRect + this->srcRect.getPosition());
		}
		return this->srcRect;
	}

	void MinimalImage::draw(cgrect rect, const april::Color& color)
	{
		if (color.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		grect drawRect = rect;
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			gvec2 sizeRatio = drawRect.getSize() / this->srcRect.getSize();
			drawRect += this->clipRect.getPosition() * sizeRatio;
			drawRect.setSize(this->clipRect.getSize() * sizeRatio);
		}
		this->vertices[0].x = this->vertices[2].x = this->vertices[4].x = drawRect.left();
		this->vertices[0].y = this->vertices[1].y = this->vertices[3].y = drawRect.top();
		this->vertices[1].x = this->vertices[3].x = this->vertices[5].x = drawRect.right();
		this->vertices[2].y = this->vertices[4].y = this->vertices[5].y = drawRect.bottom();
		if (this->texture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->texture->load();
		}
		april::rendersys->setTexture(this->texture->getTexture());
		this->tryLoadTextureCoordinates();
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply, 1.0f);
		april::rendersys->render(april::RenderOperation::TriangleList, this->vertices, APRILUI_IMAGE_MAX_VERTICES, drawColor);
	}

	void MinimalImage::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		if (color.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		this->texture->load();
		april::rendersys->setTexture(this->texture->getTexture());
		this->tryLoadTextureCoordinates();
		// texture coordinate scaling
		float iw = 1.0f / this->texture->getWidth();
		float ih = 1.0f / this->texture->getHeight();
		grect rect = this->_makeClippedSrcRect();
		harray<april::TexturedVertex> textureVertices = vertices;
		foreach (april::TexturedVertex, it, textureVertices)
		{
			it->u = (rect.x + it->u * rect.w) * iw;
			it->v = (rect.y + it->v * rect.h) * ih;
		}
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply, 1.0f);
		april::rendersys->render(april::RenderOperation::TriangleList, (april::TexturedVertex*)textureVertices, textureVertices.size(), drawColor);
	}
	
}
