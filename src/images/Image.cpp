/// @file
/// @version 5.0
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
	hmap<hstr, PropertyDescription> Image::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> Image::_getters;
	hmap<hstr, PropertyDescription::Accessor*> Image::_setters;

	Image::Image(Texture* texture, chstr name, cgrectf source) :
		MinimalImage(texture, name, source)
	{
		this->color = april::Color::White;
		this->blendMode = april::BlendMode::Alpha;
		this->colorMode = april::ColorMode::Multiply;
		this->colorModeFactor = 1.0f;
	}

	Image::Image(const Image& other) :
		MinimalImage(other)
	{
		this->color = other.color;
		this->blendMode = other.blendMode;
		this->colorMode = other.colorMode;
		this->colorModeFactor = other.colorModeFactor;
	}

	MinimalImage* Image::createInstance(Texture* texture, chstr name, cgrectf source)
	{
		return new Image(texture, name, source);
	}
	
	hmap<hstr, PropertyDescription>& Image::getPropertyDescriptions() const
	{
		if (Image::_propertyDescriptions.size() == 0)
		{
			Image::_propertyDescriptions = MinimalImage::getPropertyDescriptions();
			Image::_propertyDescriptions["color"] = PropertyDescription("color", PropertyDescription::Type::Color);
			Image::_propertyDescriptions["red"] = PropertyDescription("red", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["green"] = PropertyDescription("green", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["blue"] = PropertyDescription("blue", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["alpha"] = PropertyDescription("alpha", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["blend_mode"] = PropertyDescription("blend_mode", PropertyDescription::Type::Enum);
			Image::_propertyDescriptions["color_mode"] = PropertyDescription("color_mode", PropertyDescription::Type::Enum);
			Image::_propertyDescriptions["color_mode_factor"] = PropertyDescription("color_mode_factor", PropertyDescription::Type::Float);
		}
		return Image::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Image::_getGetters() const
	{
		if (Image::_getters.size() == 0)
		{
			Image::_getters = MinimalImage::_getGetters();
			Image::_getters["color"] = new PropertyDescription::GetColor<Image>(&Image::getColor);
			Image::_getters["red"] = new PropertyDescription::GetUChar<Image>(&Image::getRed);
			Image::_getters["green"] = new PropertyDescription::GetUChar<Image>(&Image::getGreen);
			Image::_getters["blue"] = new PropertyDescription::GetUChar<Image>(&Image::getBlue);
			Image::_getters["alpha"] = new PropertyDescription::GetUChar<Image>(&Image::getAlpha);
			Image::_getters["color_mode_factor"] = new PropertyDescription::Get<Image, float>(&Image::getColorModeFactor);
		}
		return Image::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Image::_getSetters() const
	{
		if (Image::_setters.size() == 0)
		{
			Image::_setters = MinimalImage::_getSetters();
			Image::_setters["color"] = new PropertyDescription::Set<Image, hstr>(&Image::setSymbolicColor);
			Image::_setters["red"] = new PropertyDescription::SetUChar<Image>(&Image::setRed);
			Image::_setters["green"] = new PropertyDescription::SetUChar<Image>(&Image::setGreen);
			Image::_setters["blue"] = new PropertyDescription::SetUChar<Image>(&Image::setBlue);
			Image::_setters["alpha"] = new PropertyDescription::SetUChar<Image>(&Image::setAlpha);
			Image::_setters["color_mode_factor"] = new PropertyDescription::Set<Image, float>(&Image::setColorModeFactor);
		}
		return Image::_setters;
	}

	void Image::setSymbolicColor(chstr value)
	{
		this->setColor(aprilui::_makeColor(value));
	}

	hstr Image::getProperty(chstr name)
	{
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
			if (this->colorMode == april::ColorMode::Multiply)		return "multiply";
			if (this->colorMode == april::ColorMode::AlphaMap)		return "alpha_map";
			if (this->colorMode == april::ColorMode::Lerp)			return "lerp";
			if (this->colorMode == april::ColorMode::Desaturate)	return "desaturate";
			if (this->colorMode == april::ColorMode::Sepia)			return "sepia";
			return "";
		}
		return MinimalImage::getProperty(name);
	}
	
	bool Image::setProperty(chstr name, chstr value)
	{
		if (name == "blend_mode")
		{
			if (value == "alpha")			this->setBlendMode(april::BlendMode::Alpha);
			else if	(value == "add")		this->setBlendMode(april::BlendMode::Add);
			else if	(value == "subtract")	this->setBlendMode(april::BlendMode::Subtract);
			else if	(value == "overwrite")	this->setBlendMode(april::BlendMode::Overwrite);
			else
			{
				hlog::warnf(logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.cStr(), name.cStr(), this->name.cStr());
				return false;
			}
			return true;
		}
		if (name == "color_mode")
		{
			if (value == "multiply")		this->setColorMode(april::ColorMode::Multiply);
			else if	(value == "alpha_map")	this->setColorMode(april::ColorMode::AlphaMap);
			else if (value == "lerp")		this->setColorMode(april::ColorMode::Lerp);
			else if (value == "desaturate")	this->setColorMode(april::ColorMode::Desaturate);
			else if (value == "sepia")		this->setColorMode(april::ColorMode::Sepia);
			else
			{
				hlog::warnf(logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.cStr(), name.cStr(), this->name.cStr());
				return false;
			}
			return true;
		}
		return MinimalImage::setProperty(name, value);
	}

	void Image::draw(cgrectf rect, const april::Color& color)
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
			if (drawColor.a == 0)
			{
				return;
			}
		}
		this->texture->load();
		april::rendersys->setTexture(this->texture->getTexture());
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
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		april::rendersys->render(april::RenderOperation::TriangleList, (april::TexturedVertex*)textureVertices, textureVertices.size(), drawColor);
	}
	
}
