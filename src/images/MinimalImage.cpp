/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hltypesUtil.h>

#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "MinimalImage.h"
#include "Texture.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> MinimalImage::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> MinimalImage::_getters;
	hmap<hstr, PropertyDescription::Accessor*> MinimalImage::_setters;

	MinimalImage::MinimalImage(Texture* texture, chstr name, cgrectf source) :
		BaseImage(name)
	{
		this->texture = texture;
		this->srcRect = source;
		this->_textureCoordinatesLoaded = false;
	}

	MinimalImage::MinimalImage(const MinimalImage& other) :
		BaseImage(other)
	{
		this->texture = other.texture;
		this->srcRect = other.srcRect;
		for_iter (i, 0, APRILUI_IMAGE_MAX_VERTICES)
		{
			this->vertices[i] = other.vertices[i];
		}
		this->_textureCoordinatesLoaded = false;
	}

	MinimalImage* MinimalImage::createInstance(Texture* texture, chstr name, cgrectf source)
	{
		return new MinimalImage(texture, name, source);
	}
	
	hmap<hstr, PropertyDescription>& MinimalImage::getPropertyDescriptions() const
	{
		if (MinimalImage::_propertyDescriptions.size() == 0)
		{
			MinimalImage::_propertyDescriptions = BaseImage::getPropertyDescriptions();
			MinimalImage::_propertyDescriptions["rect"] = PropertyDescription("rect", PropertyDescription::Type::Grectf);
			MinimalImage::_propertyDescriptions["position"] = PropertyDescription("position", PropertyDescription::Type::Gvec2f);
			MinimalImage::_propertyDescriptions["x"] = PropertyDescription("x", PropertyDescription::Type::Float);
			MinimalImage::_propertyDescriptions["y"] = PropertyDescription("y", PropertyDescription::Type::Float);
			MinimalImage::_propertyDescriptions["texture"] = PropertyDescription("texture", PropertyDescription::Type::String);
		}
		return MinimalImage::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& MinimalImage::_getGetters() const
	{
		if (MinimalImage::_getters.size() == 0)
		{
			MinimalImage::_getters = BaseImage::_getGetters();
			MinimalImage::_getters["rect"] = new PropertyDescription::GetGrectf<MinimalImage>(&MinimalImage::getSrcRect);
			MinimalImage::_getters["position"] = new PropertyDescription::GetGvec2f<MinimalImage>(&MinimalImage::getSrcPosition);
			MinimalImage::_getters["x"] = new PropertyDescription::Get<MinimalImage, float>(&MinimalImage::getSrcX);
			MinimalImage::_getters["y"] = new PropertyDescription::Get<MinimalImage, float>(&MinimalImage::getSrcY);
		}
		return MinimalImage::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& MinimalImage::_getSetters() const
	{
		if (MinimalImage::_setters.size() == 0)
		{
			MinimalImage::_setters = BaseImage::_getSetters();
			MinimalImage::_setters["rect"] = new PropertyDescription::SetGrectf<MinimalImage>(&MinimalImage::setSrcRect);
			MinimalImage::_setters["position"] = new PropertyDescription::SetGvec2f<MinimalImage>(&MinimalImage::setSrcPosition);
			MinimalImage::_setters["x"] = new PropertyDescription::Set<MinimalImage, float>(&MinimalImage::setSrcX);
			MinimalImage::_setters["y"] = new PropertyDescription::Set<MinimalImage, float>(&MinimalImage::setSrcY);
		}
		return MinimalImage::_setters;
	}

	void MinimalImage::setSrcRect(cgrectf value)
	{
		if (this->srcRect != value)
		{
			this->srcRect = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcRect(const float& x, const float& y, const float& width, const float& height)
	{
		if (this->srcRect.x != x || this->srcRect.y != y || this->srcRect.w != width || this->srcRect.h != height)
		{
			this->srcRect.set(x, y, width, height);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcX(const float& value)
	{
		if (this->srcRect.x != value)
		{
			this->srcRect.x = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcY(const float& value)
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

	void MinimalImage::setSrcPosition(cgvec2f value)
	{
		if (this->srcRect.getPosition() != value)
		{
			this->srcRect.setPosition(value);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcPosition(const float& x, const float& y)
	{
		if (this->srcRect.x != x || this->srcRect.y != y)
		{
			this->srcRect.setPosition(x, y);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcSize(cgvec2f value)
	{
		if (this->srcRect.getSize() != value)
		{
			this->srcRect.setSize(value);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void MinimalImage::setSrcSize(const float& w, const float& h)
	{
		if (this->srcRect.w != w || this->srcRect.h != h)
		{
			this->srcRect.setSize(w, h);
			this->_textureCoordinatesLoaded = false;
		}
	}

	hstr MinimalImage::getProperty(chstr name)
	{
		if (name == "texture")
		{
			return (this->texture != NULL ? this->texture->getName() : "");
		}
		return BaseImage::getProperty(name);
	}
	
	void MinimalImage::_setDeviceTexture()
	{
		if (this->texture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->texture->load();
			april::rendersys->setTexture(this->texture->getTexture());
		}
		else
		{
			hlog::errorf(logTag, "Image '%s' has no texture assigned!", this->name.cStr());
			april::rendersys->setTexture(NULL);
		}
	}

	bool MinimalImage::tryLoadTextureCoordinates()
	{
		if ((!this->_textureCoordinatesLoaded || !this->_clipRectCalculated) && this->texture != NULL && this->texture->getWidth() > 0 && this->texture->getHeight() > 0)
		{
			this->_textureCoordinatesLoaded = true;
			this->_clipRectCalculated = true;
			float iw = 1.0f / this->texture->getWidth();
			float ih = 1.0f / this->texture->getHeight();
			grectf rect = this->_makeClippedSrcRect();
			gvec2f topLeft;
			gvec2f topRight;
			gvec2f bottomLeft;
			gvec2f bottomRight;
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
			return true;
		}
		return false;
	}

	grectf MinimalImage::_makeClippedSrcRect() const
	{
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			return this->srcRect.clipped(this->clipRect + this->srcRect.getPosition());
		}
		return this->srcRect;
	}

	void MinimalImage::draw(cgrectf rect, const april::Color& color)
	{
		if (color.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		grectf drawRect = rect;
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			gvec2f sizeRatio = drawRect.getSize() / this->srcRect.getSize();
			drawRect += this->clipRect.getPosition() * sizeRatio;
			drawRect.setSize(this->clipRect.getSize() * sizeRatio);
		}
		this->vertices[0].x = this->vertices[2].x = this->vertices[4].x = drawRect.left();
		this->vertices[0].y = this->vertices[1].y = this->vertices[3].y = drawRect.top();
		this->vertices[1].x = this->vertices[3].x = this->vertices[5].x = drawRect.right();
		this->vertices[2].y = this->vertices[4].y = this->vertices[5].y = drawRect.bottom();
		this->_setDeviceTexture();
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
		this->_setDeviceTexture();
		this->tryLoadTextureCoordinates();
		// texture coordinate scaling
		float iw = 1.0f / this->texture->getWidth();
		float ih = 1.0f / this->texture->getHeight();
		grectf rect = this->_makeClippedSrcRect();
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
