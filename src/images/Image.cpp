/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hdir.h>
#include <hltypes/hltypesUtil.h>

#include "apriluiUtil.h"
#include "Dataset.h"
#include "Exception.h"
#include "Image.h"
#include "Texture.h"

namespace aprilui
{
	harray<PropertyDescription> Image::_propertyDescriptions;

	Image::Image(Texture* texture, chstr name, cgrect source) : BaseImage(name)
	{
		this->texture = texture;
		this->srcRect = source;
		this->color = april::Color::White;
		this->blendMode = april::BlendMode::Alpha;
		this->colorMode = april::ColorMode::Multiply;
		this->colorModeFactor = 1.0f;
		this->rotated = false;
	}

	Image::Image(const Image& other) : BaseImage(other)
	{
		this->texture = other.texture;
		this->srcRect = other.srcRect;
		this->color = other.color;
		this->blendMode = other.blendMode;
		this->colorMode = other.colorMode;
		this->colorModeFactor = other.colorModeFactor;
		this->rotated = other.rotated;
		for_iter (i, 0, APRILUI_IMAGE_MAX_VERTICES)
		{
			this->vertices[i] = other.vertices[i];
		}
	}

	Image::~Image()
	{
	}

	Image* Image::createInstance(Texture* texture, chstr name, cgrect source)
	{
		return new Image(texture, name, source);
	}
	
	harray<PropertyDescription> Image::getPropertyDescriptions() const
	{
		if (Image::_propertyDescriptions.size() == 0)
		{
			Image::_propertyDescriptions += PropertyDescription("rect", PropertyDescription::Type::Grect);
			Image::_propertyDescriptions += PropertyDescription("position", PropertyDescription::Type::Gvec2);
			Image::_propertyDescriptions += PropertyDescription("x", PropertyDescription::Type::Float);
			Image::_propertyDescriptions += PropertyDescription("y", PropertyDescription::Type::Float);
			Image::_propertyDescriptions += PropertyDescription("color", PropertyDescription::Type::Color);
			Image::_propertyDescriptions += PropertyDescription("rotated", PropertyDescription::Type::Bool);
			Image::_propertyDescriptions += PropertyDescription("blend_mode", PropertyDescription::Type::Enum);
			Image::_propertyDescriptions += PropertyDescription("color_mode", PropertyDescription::Type::Enum);
			Image::_propertyDescriptions += PropertyDescription("color_mode_factor", PropertyDescription::Type::Float);
			Image::_propertyDescriptions += PropertyDescription("texture", PropertyDescription::Type::String);
		}
		return (Image::_propertyDescriptions + BaseImage::getPropertyDescriptions());
	}

	void Image::setSrcRect(cgrect value)
	{
		if (this->srcRect != value)
		{
			this->srcRect = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcX(float value)
	{
		if (this->srcRect.x != value)
		{
			this->srcRect.x = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcY(float value)
	{
		if (this->srcRect.y != value)
		{
			this->srcRect.y = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcWidth(const float& value)
	{
		if (this->srcRect.w != value)
		{
			this->srcRect.w = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcHeight(const float& value)
	{
		if (this->srcRect.h != value)
		{
			this->srcRect.h = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcPosition(cgvec2 value)
	{
		if (this->srcRect.getPosition() != value)
		{
			this->srcRect.setPosition(value);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcPosition(float x, float y)
	{
		if (this->srcRect.x != x || this->srcRect.y != y)
		{
			this->srcRect.setPosition(x, y);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcSize(cgvec2 value)
	{
		if (this->srcRect.getSize() != value)
		{
			this->srcRect.setSize(value);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcSize(float w, float h)
	{
		if (this->srcRect.w != w || this->srcRect.h != h)
		{
			this->srcRect.setSize(w, h);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSymbolicColor(chstr value)
	{
		this->setColor(aprilui::_makeColor(value));
	}

	hstr Image::getProperty(chstr name)
	{
		if (name == "rect")					return april::grectToHstr(this->getSrcRect());
		if (name == "position")				return april::gvec2ToHstr(this->getSrcRect().getPosition());
		if (name == "x")					return this->getSrcRect().x;
		if (name == "y")					return this->getSrcRect().y;
		if (name == "color")				return this->getColor().hex();
		if (name == "rotated")				return this->isRotated();
		if (name == "vertical")
		{
			hlog::warn(logTag, "'vertical' is deprecated. Use 'rotated' instead."); // DEPRECATED
			return this->isRotated();
		}
		if (name == "blend_mode")
		{
			if (this->blendMode == april::BlendMode::Alpha)		return "alpha";
			if (this->blendMode == april::BlendMode::Add)		return "add";
			if (this->blendMode == april::BlendMode::Subtract)	return "subtract";
			if (this->blendMode == april::BlendMode::Overwrite)	return "overwrite";
			return "";
		}
		if (name == "color_mode")
		{
			if (this->colorMode == april::ColorMode::Multiply)	return "multiply";
			if (this->colorMode == april::ColorMode::AlphaMap)	return "alpha_map";
			if (this->colorMode == april::ColorMode::Lerp)		return "lerp";
			return "";
		}
		if (name == "color_mode_factor")	return this->getColorModeFactor();
		if (name == "texture")
		{
			return (this->texture != NULL ? hdir::baseName(this->texture->getOriginalFilename()) : "");
		}
		return BaseImage::getProperty(name);
	}
	
	bool Image::setProperty(chstr name, chstr value)
	{
		if		(name == "rect")				this->setSrcRect(april::hstrToGrect(value));
		else if	(name == "position")			this->setSrcPosition(april::hstrToGvec2(value));
		else if	(name == "x")					this->setSrcX(value);
		else if	(name == "y")					this->setSrcY(value);
		else if (name == "color")				this->setColor(aprilui::_makeColor(value));
		else if	(name == "rotated")				this->setRotated(value);
		else if	(name == "vertical")
		{
			hlog::warn(logTag, "'vertical=' is deprecated. Use 'rotated=' instead."); // DEPRECATED
			this->setRotated(value);
		}
		else if	(name == "blend_mode")
		{
			if (value == "default")
			{
				hlog::warn(logTag, "'blend_mode=default' is deprecated. Use 'blend_mode=alpha' instead."); // DEPRECATED
				this->setBlendMode(april::BlendMode::Alpha);
			}
			else if	(value == "alpha")		this->setBlendMode(april::BlendMode::Alpha);
			else if	(value == "add")		this->setBlendMode(april::BlendMode::Add);
			else if	(value == "subtract")	this->setBlendMode(april::BlendMode::Subtract);
			else if	(value == "overwrite")	this->setBlendMode(april::BlendMode::Overwrite);
			else hlog::warnf(logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.cStr(), name.cStr(), this->name.cStr());
		}
		else if	(name == "color_mode")
		{
			if (value == "default")
			{
				hlog::warn(logTag, "'color_mode=default' is deprecated. Use 'color_mode=multiply' instead."); // DEPRECATED
				this->setColorMode(april::ColorMode::Multiply);
			}
			else if	(value == "multiply")	this->setColorMode(april::ColorMode::Multiply);
			else if	(value == "alpha_map")	this->setColorMode(april::ColorMode::AlphaMap);
			else if (value == "lerp")		this->setColorMode(april::ColorMode::Lerp);
			else hlog::warnf(logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.cStr(), name.cStr(), this->name.cStr());
		}
		else if	(name == "color_mode_factor")	this->setColorModeFactor(value);
		else return BaseImage::setProperty(name, value);
		return true;
	}

	void Image::tryLoadTextureCoordinates()
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
			if (!this->rotated)
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

	grect Image::_makeClippedSrcRect() const
	{
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			if (this->rotated)
			{
				grect rect = this->clipRect;
				grect srcRect = this->srcRect;
				hswap(rect.x, rect.y);
				hswap(rect.w, rect.h);
				hswap(srcRect.w, srcRect.h);
				rect.x = srcRect.w - (rect.x + rect.w);
				srcRect.clip(rect + srcRect.getPosition());
				hswap(srcRect.w, srcRect.h);
				return srcRect;
			}
			return this->srcRect.clipped(this->clipRect + this->srcRect.getPosition());
		}
		return this->srcRect;
	}

	void Image::draw(cgrect rect, const april::Color& color)
	{
		if (color.a == 0 || this->color.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		if (this->color != april::Color::White)
		{
			drawColor *= this->color;
			if (drawColor.a == 0)
			{
				return;
			}
		}
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
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		april::rendersys->render(april::RenderOperation::TriangleList, this->vertices, APRILUI_IMAGE_MAX_VERTICES, drawColor);
	}

	void Image::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		if (color.a == 0 || this->color.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		if (this->color != april::Color::White)
		{
			drawColor *= this->color;
		}
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
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		april::rendersys->render(april::RenderOperation::TriangleList, (april::TexturedVertex*)textureVertices, textureVertices.size(), drawColor);
	}
	
}
