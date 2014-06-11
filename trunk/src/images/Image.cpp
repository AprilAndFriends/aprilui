/// @file
/// @version 3.2
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

	Image::Image(Texture* texture, chstr name, grect source)
	{
		this->texture = texture;
		this->name = name;
		this->srcRect = source;
		this->color = april::Color::White;
		this->blendMode = april::BM_DEFAULT;
		this->colorMode = april::CM_DEFAULT;
		this->colorModeFactor = 1.0f;
		this->rotated = false;
		this->invertX = false;
		this->invertY = false;
		this->_textureCoordinatesLoaded = false;
		this->dataset = NULL;
	}

	Image::Image(Image& other, chstr name)
	{
		this->texture = other.texture;
		this->name = name;
		this->srcRect = other.srcRect;
		this->color = other.color;
		this->blendMode = other.blendMode;
		this->colorMode = other.colorMode;
		this->colorModeFactor = other.colorModeFactor;
		this->rotated = other.rotated;
		this->invertX = other.invertX;
		this->invertY = other.invertY;
		this->_textureCoordinatesLoaded = false;
		this->dataset = NULL;
	}
	
	Image::~Image()
	{
	}
	
	hstr Image::getFullName()
	{
		return (this->dataset != NULL ? this->dataset->getName() + "." + this->name : this->name);
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
			Image::_propertyDescriptions += PropertyDescription("invert_x", PropertyDescription::BOOL);
			Image::_propertyDescriptions += PropertyDescription("invert_y", PropertyDescription::BOOL);
			Image::_propertyDescriptions += PropertyDescription("blend_mode", PropertyDescription::ENUM);
			Image::_propertyDescriptions += PropertyDescription("color_mode", PropertyDescription::ENUM);
			Image::_propertyDescriptions += PropertyDescription("color_mode_factor", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("texture", PropertyDescription::STRING);
			Image::_propertyDescriptions += PropertyDescription("dataset", PropertyDescription::STRING);
		}
		return Image::_propertyDescriptions;
	}

	void Image::setSrcRect(grect value)
	{
		this->srcRect = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSrcX(float value)
	{
		this->srcRect.x = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSrcY(float value)
	{
		this->srcRect.y = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSrcWidth(float value)
	{
		this->srcRect.w = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSrcHeight(float value)
	{
		this->srcRect.h = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setPosition(gvec2 value)
	{
		this->srcRect.setPosition(value);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setPosition(float x, float y)
	{
		this->srcRect.setPosition(x, y);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSize(gvec2 value)
	{
		this->srcRect.setSize(value);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSize(float x, float y)
	{
		this->srcRect.setSize(x, y);
		this->_textureCoordinatesLoaded = false;
	}

	hstr Image::getProperty(chstr name)
	{
		if (name == "rect")					return grect_to_hstr(this->getSrcRect());
		if (name == "position")				return gvec2_to_hstr(this->getSrcRect().getPosition());
		if (name == "size")					return gvec2_to_hstr(this->getSrcRect().getSize());
		if (name == "x")					return this->getSrcRect().x;
		if (name == "y")					return this->getSrcRect().y;
		if (name == "w")					return this->getSrcRect().w;
		if (name == "h")					return this->getSrcRect().h;
		if (name == "color")				return this->getColor().hex();
		if (name == "rotated")				return this->isRotated();
		if (name == "vertical")
		{
			hlog::warn(aprilui::logTag, "\"vertical\" is deprecated. Use \"rotated\" instead."); // DEPRECATED
			return this->isRotated();
		}
		if (name == "invert_x")				return this->isInvertX();
		if (name == "invertx")
		{
			hlog::warn(aprilui::logTag, "\"invertx\" is deprecated. Use \"invert_x\" instead."); // DEPRECATED
			return this->isInvertX();
		}
		if (name == "invert_y")				return this->isInvertY();
		if (name == "inverty")
		{
			hlog::warn(aprilui::logTag, "\"inverty\" is deprecated. Use \"invert_y\" instead."); // DEPRECATED
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
		return "";
	}
	
	bool Image::setProperty(chstr name, chstr value)
	{
		if		(name == "rect")				this->setSrcRect(hstr_to_grect(value));
		else if	(name == "position")			this->srcRect.setPosition(hstr_to_gvec2(value));
		else if	(name == "size")				this->srcRect.setSize(hstr_to_gvec2(value));
		else if	(name == "x")					this->setSrcX(value);
		else if	(name == "y")					this->setSrcY(value);
		else if	(name == "w")					this->setSrcWidth(value);
		else if	(name == "h")					this->setSrcHeight(value);
		else if	(name == "color")				this->setColor(value);
		else if	(name == "rotated")				this->setRotated(value);
		else if	(name == "vertical")
		{
			hlog::warn(aprilui::logTag, "\"vertical=\" is deprecated. Use \"rotated=\" instead."); // DEPRECATED
			this->setRotated(value);
		}
		else if	(name == "invert_x")			this->setInvertX(value);
		else if	(name == "invertx")
		{
			hlog::warn(aprilui::logTag, "\"invertx=\" is deprecated. Use \"invert_x=\" instead."); // DEPRECATED
			this->setInvertX(value);
		}
		else if	(name == "invert_y")			this->setInvertY(value);
		else if	(name == "inverty")
		{
			hlog::warn(aprilui::logTag, "\"inverty=\" is deprecated. Use \"invert_y=\" instead."); // DEPRECATED
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
		else return false;
		return true;
	}

	void Image::_tryLoadTexCoords()
	{
		if (!this->_textureCoordinatesLoaded && this->texture != NULL && this->texture->getWidth() > 0 && this->texture->getHeight() > 0)
		{
			this->_textureCoordinatesLoaded = true;
			float iw = 1.0f / this->texture->getWidth();
			float ih = 1.0f / this->texture->getHeight();

			if (!this->rotated)
			{
				this->_tVertices[0].u = this->_tVertices[2].u = this->srcRect.left() * iw;
				this->_tVertices[0].v = this->_tVertices[1].v = this->srcRect.top() * ih;
				this->_tVertices[1].u = this->_tVertices[3].u = this->srcRect.right() * iw;
				this->_tVertices[2].v = this->_tVertices[3].v = this->srcRect.bottom() * ih;
			}
			else
			{
				this->_tVertices[0].u = this->_tVertices[1].u = (this->srcRect.x + this->srcRect.h) * iw;
				this->_tVertices[0].v = this->_tVertices[2].v = this->srcRect.y * ih;
				this->_tVertices[1].v = this->_tVertices[3].v = (this->srcRect.y + this->srcRect.w) * ih;
				this->_tVertices[2].u = this->_tVertices[3].u = this->srcRect.x * iw;
			}

			if (this->invertX)
			{
				hswap(this->_tVertices[0].u, this->_tVertices[1].u);
				hswap(this->_tVertices[2].u, this->_tVertices[3].u);
			}
			if (this->invertY)
			{
				hswap(this->_tVertices[0].v, this->_tVertices[2].v);
				hswap(this->_tVertices[1].v, this->_tVertices[3].v);
			}
		}
	}

	void Image::draw(grect rect, april::Color color)
	{
		if (this->color != april::Color::White)
		{
			color *= this->color;
		}
		this->_tVertices[0].x = this->_tVertices[2].x = rect.left();
		this->_tVertices[0].y = this->_tVertices[1].y = rect.top();
		this->_tVertices[1].x = this->_tVertices[3].x = rect.right();
		this->_tVertices[2].y = this->_tVertices[3].y = rect.bottom();
		
		this->texture->load();
		april::rendersys->setTexture(this->texture->getRenderTexture());
		this->_tryLoadTexCoords();
		
		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::RO_TRIANGLE_STRIP, this->_tVertices, 4, color);
		}
		else
		{
			april::rendersys->render(april::RO_TRIANGLE_STRIP, this->_tVertices, 4);
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
		this->texture->load();
		april::rendersys->setTexture(this->texture->getRenderTexture());
		this->_tryLoadTexCoords();

		// texture coordinate scaling
		float iw = 1.0f / this->texture->getWidth();
		float ih = 1.0f / this->texture->getHeight();
		foreach (april::TexturedVertex, it, vertices)
		{
			it->u = (this->srcRect.x + it->u * this->srcRect.w) * iw;
			it->v = (this->srcRect.y + it->v * this->srcRect.h) * ih;
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
