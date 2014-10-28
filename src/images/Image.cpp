/// @file
/// @version 3.4
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
		this->clipRect = other.clipRect;
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
			Image::_propertyDescriptions += PropertyDescription("clip_rect", PropertyDescription::GRECT);
			Image::_propertyDescriptions += PropertyDescription("clip_position", PropertyDescription::GVEC2);
			Image::_propertyDescriptions += PropertyDescription("clip_size", PropertyDescription::GVEC2);
			Image::_propertyDescriptions += PropertyDescription("clip_x", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("clip_y", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("clip_w", PropertyDescription::FLOAT);
			Image::_propertyDescriptions += PropertyDescription("clip_h", PropertyDescription::FLOAT);
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

	void Image::setSrcPosition(gvec2 value)
	{
		this->srcRect.setPosition(value);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSrcPosition(float x, float y)
	{
		this->srcRect.setPosition(x, y);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSrcSize(gvec2 value)
	{
		this->srcRect.setSize(value);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setSrcSize(float x, float y)
	{
		this->srcRect.setSize(x, y);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipRect(grect value)
	{
		this->clipRect = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipX(float value)
	{
		this->clipRect.x = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipY(float value)
	{
		this->clipRect.y = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipWidth(float value)
	{
		this->clipRect.w = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipHeight(float value)
	{
		this->clipRect.h = value;
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipPosition(gvec2 value)
	{
		this->clipRect.setPosition(value);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipPosition(float x, float y)
	{
		this->clipRect.setPosition(x, y);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipSize(gvec2 value)
	{
		this->clipRect.setSize(value);
		this->_textureCoordinatesLoaded = false;
	}

	void Image::setClipSize(float x, float y)
	{
		this->clipRect.setSize(x, y);
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
		if (name == "clip_rect")			return grect_to_hstr(this->getClipRect());
		if (name == "clip_position")		return gvec2_to_hstr(this->getClipRect().getPosition());
		if (name == "clip_size")			return gvec2_to_hstr(this->getClipRect().getSize());
		if (name == "clip_x")				return this->getClipRect().x;
		if (name == "clip_y")				return this->getClipRect().y;
		if (name == "clip_w")				return this->getClipRect().w;
		if (name == "clip_h")				return this->getClipRect().h;
		if (name == "color")				return this->getColor().hex();
		if (name == "rotated")				return this->isRotated();
		if (name == "vertical")
		{
			hlog::warn(aprilui::logTag, "'vertical' is deprecated. Use 'rotated' instead."); // DEPRECATED
			return this->isRotated();
		}
		if (name == "invert_x")				return this->isInvertX();
		if (name == "invertx")
		{
			hlog::warn(aprilui::logTag, "'invertx' is deprecated. Use 'invert_x' instead."); // DEPRECATED
			return this->isInvertX();
		}
		if (name == "invert_y")				return this->isInvertY();
		if (name == "inverty")
		{
			hlog::warn(aprilui::logTag, "'inverty' is deprecated. Use 'invert_y' instead."); // DEPRECATED
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
		else if (name == "clip_rect")			this->setClipRect(hstr_to_grect(value));
		else if (name == "clip_position")		this->clipRect.setPosition(hstr_to_gvec2(value));
		else if (name == "clip_size")			this->clipRect.setSize(hstr_to_gvec2(value));
		else if (name == "clip_x")				this->setClipX(value);
		else if (name == "clip_y")				this->setClipY(value);
		else if (name == "clip_w")				this->setClipWidth(value);
		else if (name == "clip_h")				this->setClipHeight(value);
		else if (name == "color")				this->setColor(value);
		else if	(name == "rotated")				this->setRotated(value);
		else if	(name == "vertical")
		{
			hlog::warn(aprilui::logTag, "'vertical=' is deprecated. Use 'rotated=' instead."); // DEPRECATED
			this->setRotated(value);
		}
		else if	(name == "invert_x")			this->setInvertX(value);
		else if	(name == "invertx")
		{
			hlog::warn(aprilui::logTag, "'invertx=' is deprecated. Use 'invert_x=' instead."); // DEPRECATED
			this->setInvertX(value);
		}
		else if	(name == "invert_y")			this->setInvertY(value);
		else if	(name == "inverty")
		{
			hlog::warn(aprilui::logTag, "'inverty=' is deprecated. Use 'invert_y=' instead."); // DEPRECATED
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
			grect rect = this->_makeClippedSrcRect();
			if (!this->rotated)
			{
				this->_tVertices[0].u = this->_tVertices[2].u = rect.left() * iw;
				this->_tVertices[0].v = this->_tVertices[1].v = rect.top() * ih;
				this->_tVertices[1].u = this->_tVertices[3].u = rect.right() * iw;
				this->_tVertices[2].v = this->_tVertices[3].v = rect.bottom() * ih;
			}
			else
			{
				this->_tVertices[0].u = this->_tVertices[1].u = (rect.x + rect.h) * iw;
				this->_tVertices[0].v = this->_tVertices[2].v = rect.y * ih;
				this->_tVertices[1].v = this->_tVertices[3].v = (rect.y + rect.w) * ih;
				this->_tVertices[2].u = this->_tVertices[3].u = rect.x * iw;
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
		this->_tVertices[0].x = this->_tVertices[2].x = rect.left();
		this->_tVertices[0].y = this->_tVertices[1].y = rect.top();
		this->_tVertices[1].x = this->_tVertices[3].x = rect.right();
		this->_tVertices[2].y = this->_tVertices[3].y = rect.bottom();
		this->texture->load();
		april::rendersys->setTexture(this->texture->getTexture());
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
		if (color.a == 0)
		{
			return;
		}
		this->texture->load();
		april::rendersys->setTexture(this->texture->getTexture());
		this->_tryLoadTexCoords();
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
