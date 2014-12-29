/// @file
/// @version 4.0
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

	Image::Image(Texture* texture, chstr name, grect source) : BaseImage(name)
	{
		this->texture = texture;
		this->srcRect = source;
		this->color = april::Color::White;
		this->blendMode = april::BM_DEFAULT;
		this->colorMode = april::CM_DEFAULT;
		this->colorModeFactor = 1.0f;
		this->rotated = false;
		this->invertX = false;
		this->invertY = false;
	}

	Image::Image(Image& other, chstr name) : BaseImage(other, name)
	{
		this->texture = other.texture;
		this->srcRect = other.srcRect;
		this->color = other.color;
		this->blendMode = other.blendMode;
		this->colorMode = other.colorMode;
		this->colorModeFactor = other.colorModeFactor;
		this->rotated = other.rotated;
		this->invertX = other.invertX;
		this->invertY = other.invertY;
	}
	
	Image::~Image()
	{
	}
	
	harray<PropertyDescription> Image::getPropertyDescriptions()
	{
		if (Image::_propertyDescriptions.size() == 0)
		{
			Image::_propertyDescriptions += PropertyDescription("rect", PropertyDescription::GRECT);
			Image::_propertyDescriptions += PropertyDescription("position", PropertyDescription::GVEC2);
			Image::_propertyDescriptions += PropertyDescription("size", PropertyDescription::GVEC2);
			Image::_propertyDescriptions += PropertyDescription("x", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("y", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("w", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("h", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("color", PropertyDescription::HEXCOLOR);
			Image::_propertyDescriptions += PropertyDescription("rotated", PropertyDescription::BOOL);
			Image::_propertyDescriptions += PropertyDescription("blend_mode", PropertyDescription::ENUM);
			Image::_propertyDescriptions += PropertyDescription("color_mode", PropertyDescription::ENUM);
			Image::_propertyDescriptions += PropertyDescription("color_mode_factor", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("texture", PropertyDescription::STRING);
		}
		return (Image::_propertyDescriptions + BaseImage::getPropertyDescriptions());
	}

	void Image::setSrcRect(grect value)
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

	void Image::setSrcWidth(float value)
	{
		if (this->srcRect.w != value)
		{
			this->srcRect.w = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcHeight(float value)
	{
		if (this->srcRect.h != value)
		{
			this->srcRect.h = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void Image::setSrcPosition(gvec2 value)
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

	void Image::setSrcSize(gvec2 value)
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

	hstr Image::getProperty(chstr name)
	{
		if (name == "rect")					return april::grectToHstr(this->getSrcRect());
		if (name == "position")				return april::gvec2ToHstr(this->getSrcRect().getPosition());
		if (name == "size")					return april::gvec2ToHstr(this->getSrcRect().getSize());
		if (name == "x")					return this->getSrcRect().x;
		if (name == "y")					return this->getSrcRect().y;
		if (name == "w")					return this->getSrcRect().w;
		if (name == "h")					return this->getSrcRect().h;
		if (name == "color")				return this->getColor().hex();
		if (name == "rotated")				return this->isRotated();
		if (name == "vertical")
		{
			hlog::warn(aprilui::logTag, "'vertical' is deprecated. Use 'rotated' instead."); // DEPRECATED
			return this->isRotated();
		}
		if (name == "invert_x" || name == "invertx")
		{
			hlog::warn(aprilui::logTag, "'" + name + "' is deprecated. Use 'Object.scale_x' instead."); // DEPRECATED
			return this->isInvertX();
		}
		if (name == "invert_y" || name == "inverty")
		{
			hlog::warn(aprilui::logTag, "'" + name + "' is deprecated. Use 'Object.scale_y' instead."); // DEPRECATED
			return this->isInvertY();
		}
		if (name == "blend_mode")
		{
			if (this->blendMode == april::BM_ADD)		return "add";
			if (this->blendMode == april::BM_SUBTRACT)	return "subtract";
			if (this->blendMode == april::BM_OVERWRITE)	return "overwrite";
			return "default";
		}
		if (name == "color_mode")
		{
			if (this->colorMode == april::CM_LERP)		return "lerp";
			if (this->colorMode == april::CM_MULTIPLY)	return "alpha_map";
			return "multiply";
		}
		if (name == "color_mode_factor")	return this->getColorModeFactor();
		if (name == "texture")
		{
			return (this->texture != NULL ? hdir::basename(this->texture->getOriginalFilename()) : "");
		}
		if (name == "dataset")
		{
			return (this->dataset != NULL ? this->dataset->getName() : "");
		}
		return BaseImage::getProperty(name);
	}
	
	bool Image::setProperty(chstr name, chstr value)
	{
		if		(name == "rect")				this->setSrcRect(april::hstrToGrect(value));
		else if	(name == "position")			this->setSrcPosition(april::hstrToGvec2(value));
		else if	(name == "size")				this->setSrcSize(april::hstrToGvec2(value));
		else if	(name == "x")					this->setSrcX(value);
		else if	(name == "y")					this->setSrcY(value);
		else if	(name == "w")					this->setSrcWidth(value);
		else if	(name == "h")					this->setSrcHeight(value);
		else if (name == "color")				this->setColor(value);
		else if	(name == "rotated")				this->setRotated(value);
		else if	(name == "vertical")
		{
			hlog::warn(aprilui::logTag, "'vertical=' is deprecated. Use 'rotated=' instead."); // DEPRECATED
			this->setRotated(value);
		}
		else if	(name == "invert_x" || name == "invertx")
		{
			hlog::warn(aprilui::logTag, "'" + name + "=' is deprecated. Use 'Object.scale_x=' instead."); // DEPRECATED
			this->setInvertX(value);
		}
		else if (name == "invert_y" || name == "inverty")
		{
			hlog::warn(aprilui::logTag, "'"+ name + "=' is deprecated. Use 'Object.scale_y=' instead."); // DEPRECATED
			this->setInvertY(value);
		}
		else if	(name == "blend_mode")
		{
			if		(value == "default")	this->setBlendMode(april::BM_DEFAULT);
			else if	(value == "alpha")		this->setBlendMode(april::BM_ALPHA);
			else if	(value == "add")		this->setBlendMode(april::BM_ADD);
			else if	(value == "subtract")	this->setBlendMode(april::BM_SUBTRACT);
			else if	(value == "overwrite")	this->setBlendMode(april::BM_OVERWRITE);
			else hlog::warnf(aprilui::logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.c_str(), name.c_str(), this->name.c_str());
		}
		else if	(name == "color_mode")
		{
			if		(value == "default")	this->setColorMode(april::CM_DEFAULT);
			else if	(value == "multiply")	this->setColorMode(april::CM_MULTIPLY);
			else if	(value == "lerp")		this->setColorMode(april::CM_LERP);
			else if	(value == "alpha_map")	this->setColorMode(april::CM_ALPHA_MAP);
			else hlog::warnf(aprilui::logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.c_str(), name.c_str(), this->name.c_str());
		}
		else if	(name == "color_mode_factor")	this->setColorModeFactor(value);
		else return BaseImage::setProperty(name, value);
		return true;
	}

	void Image::tryLoadTextureCoordinates()
	{
		if (!this->_textureCoordinatesLoaded && this->texture != NULL && this->texture->getWidth() > 0 && this->texture->getHeight() > 0)
		{
			this->_textureCoordinatesLoaded = true;
			float iw = 1.0f / this->texture->getWidth();
			float ih = 1.0f / this->texture->getHeight();
			grect rect = this->_makeClippedSrcRect();
			if (!this->rotated)
			{
				this->vertices[0].u = this->vertices[2].u = rect.left() * iw;
				this->vertices[0].v = this->vertices[1].v = rect.top() * ih;
				this->vertices[1].u = this->vertices[3].u = rect.right() * iw;
				this->vertices[2].v = this->vertices[3].v = rect.bottom() * ih;
			}
			else
			{
				this->vertices[0].u = this->vertices[1].u = (rect.x + rect.h) * iw;
				this->vertices[0].v = this->vertices[2].v = rect.y * ih;
				this->vertices[1].v = this->vertices[3].v = (rect.y + rect.w) * ih;
				this->vertices[2].u = this->vertices[3].u = rect.x * iw;
			}
			if (this->invertX)
			{
				hswap(this->vertices[0].u, this->vertices[1].u);
				hswap(this->vertices[2].u, this->vertices[3].u);
			}
			if (this->invertY)
			{
				hswap(this->vertices[0].v, this->vertices[2].v);
				hswap(this->vertices[1].v, this->vertices[3].v);
			}
		}
	}

	grect Image::_makeClippedSrcRect()
	{
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			grect rect = this->clipRect;
			if (this->rotated)
			{
				hswap(rect.x, rect.y);
				rect.x = this->srcRect.h - (rect.x + rect.h);
			}
			return this->srcRect.clipped(rect + this->srcRect.getPosition());
		}
		return this->srcRect;
	}

	void Image::draw(grect rect, april::Color color)
	{
		if (this->color != april::Color::White)
		{
			color *= this->color;
		}
		if (color.a == 0)
		{
			return;
		}
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			gvec2 sizeRatio = rect.getSize() / this->srcRect.getSize();
			rect += this->clipRect.getPosition() * sizeRatio;
			rect.setSize(this->clipRect.getSize() * sizeRatio);
		}
		this->vertices[0].x = this->vertices[2].x = rect.left();
		this->vertices[0].y = this->vertices[1].y = rect.top();
		this->vertices[1].x = this->vertices[3].x = rect.right();
		this->vertices[2].y = this->vertices[3].y = rect.bottom();
		if (this->texture != NULL)
		{
			this->texture->load();
		}
		april::rendersys->setTexture(this->texture->getTexture());
		this->tryLoadTextureCoordinates();
		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::RO_TRIANGLE_STRIP, this->vertices, 4, color);
		}
		else
		{
			april::rendersys->render(april::RO_TRIANGLE_STRIP, this->vertices, 4);
		}
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		april::rendersys->setTextureColorMode(april::CM_DEFAULT);
	}

	void Image::draw(harray<april::TexturedVertex> vertices, april::Color color)
	{
		if (this->color != april::Color::White)
		{
			color *= this->color;
		}
		if (color.a == 0)
		{
			return;
		}
		this->texture->load();
		april::rendersys->setTexture(this->texture->getTexture());
		this->tryLoadTextureCoordinates();
		// texture coordinate scaling
		float iw = 1.0f / this->texture->getWidth();
		float ih = 1.0f / this->texture->getHeight();
		grect rect = this->_makeClippedSrcRect();
		foreach (april::TexturedVertex, it, vertices)
		{
			it->u = (rect.x + it->u * rect.w) * iw;
			it->v = (rect.y + it->v * rect.h) * ih;
		}
		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::RO_TRIANGLE_LIST, &vertices[0], vertices.size(), color);
		}
		else
		{
			april::rendersys->render(april::RO_TRIANGLE_LIST, &vertices[0], vertices.size());
		}
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		april::rendersys->setTextureColorMode(april::CM_DEFAULT);
	}
	
}
