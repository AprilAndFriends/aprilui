/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
	hmap<hstr, PropertyDescription> Image::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> Image::_getters;
	hmap<hstr, PropertyDescription::Accessor*> Image::_setters;

	Image::Image(Texture* texture, chstr name, cgrectf source) :
		MinimalImage(texture, name, source)
	{
		this->colorTopLeft = april::Color::White;
		this->colorTopRight = april::Color::White;
		this->colorBottomLeft = april::Color::White;
		this->colorBottomRight = april::Color::White;
		this->useAdditionalColors = false;
		this->blendMode = april::BlendMode::Alpha;
		this->colorMode = april::ColorMode::Multiply;
		this->colorModeFactor = 1.0f;
	}

	Image::Image(const Image& other) :
		MinimalImage(other)
	{
		this->colorTopLeft = other.colorTopLeft;
		this->colorTopRight = other.colorTopRight;
		this->colorBottomLeft = other.colorBottomLeft;
		this->colorBottomRight = other.colorBottomRight;
		this->useAdditionalColors = other.useAdditionalColors;
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
			Image::_propertyDescriptions["color_top_right"] = PropertyDescription("color_top_right", PropertyDescription::Type::Color);
			Image::_propertyDescriptions["red_top_right"] = PropertyDescription("red_top_right", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["green_top_right"] = PropertyDescription("green_top_right", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["blue_top_right"] = PropertyDescription("blue_top_right", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["alpha_top_right"] = PropertyDescription("alpha_top_right", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["color_bottom_left"] = PropertyDescription("color_bottom_left", PropertyDescription::Type::Color);
			Image::_propertyDescriptions["red_bottom_left"] = PropertyDescription("red_bottom_left", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["green_bottom_left"] = PropertyDescription("green_bottom_left", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["blue_bottom_left"] = PropertyDescription("blue_bottom_left", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["alpha_bottom_left"] = PropertyDescription("alpha_bottom_left", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["color_bottom_right"] = PropertyDescription("color_bottom_right", PropertyDescription::Type::Color);
			Image::_propertyDescriptions["red_bottom_right"] = PropertyDescription("red_bottom_right", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["green_bottom_right"] = PropertyDescription("green_bottom_right", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["blue_bottom_right"] = PropertyDescription("blue_bottom_right", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["alpha_bottom_right"] = PropertyDescription("alpha_bottom_right", PropertyDescription::Type::UChar);
			Image::_propertyDescriptions["use_additional_colors"] = PropertyDescription("use_additional_colors", PropertyDescription::Type::Bool);
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
			Image::_getters["color_top_right"] = new PropertyDescription::GetColor<Image>(&Image::getColorTopRight);
			Image::_getters["red_top_right"] = new PropertyDescription::GetUChar<Image>(&Image::getRedTopRight);
			Image::_getters["green_top_right"] = new PropertyDescription::GetUChar<Image>(&Image::getGreenTopRight);
			Image::_getters["blue_top_right"] = new PropertyDescription::GetUChar<Image>(&Image::getBlueTopRight);
			Image::_getters["alpha_top_right"] = new PropertyDescription::GetUChar<Image>(&Image::getAlphaTopRight);
			Image::_getters["color_bottom_left"] = new PropertyDescription::GetColor<Image>(&Image::getColorBottomLeft);
			Image::_getters["red_bottom_left"] = new PropertyDescription::GetUChar<Image>(&Image::getRedBottomLeft);
			Image::_getters["green_bottom_left"] = new PropertyDescription::GetUChar<Image>(&Image::getGreenBottomLeft);
			Image::_getters["blue_bottom_left"] = new PropertyDescription::GetUChar<Image>(&Image::getBlueBottomLeft);
			Image::_getters["alpha_bottom_left"] = new PropertyDescription::GetUChar<Image>(&Image::getAlphaBottomLeft);
			Image::_getters["color_bottom_right"] = new PropertyDescription::GetColor<Image>(&Image::getColorBottomRight);
			Image::_getters["red_bottom_right"] = new PropertyDescription::GetUChar<Image>(&Image::getRedBottomRight);
			Image::_getters["green_bottom_right"] = new PropertyDescription::GetUChar<Image>(&Image::getGreenBottomRight);
			Image::_getters["blue_bottom_right"] = new PropertyDescription::GetUChar<Image>(&Image::getBlueBottomRight);
			Image::_getters["alpha_bottom_right"] = new PropertyDescription::GetUChar<Image>(&Image::getAlphaBottomRight);
			Image::_getters["use_additional_colors"] = new PropertyDescription::Get<Image, bool>(&Image::isUseAdditionalColors);
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
			Image::_setters["color_top_right"] = new PropertyDescription::Set<Image, hstr>(&Image::setSymbolicColorTopRight);
			Image::_setters["red_top_right"] = new PropertyDescription::SetUChar<Image>(&Image::setRedTopRight);
			Image::_setters["green_top_right"] = new PropertyDescription::SetUChar<Image>(&Image::setGreenTopRight);
			Image::_setters["blue_top_right"] = new PropertyDescription::SetUChar<Image>(&Image::setBlueTopRight);
			Image::_setters["alpha_top_right"] = new PropertyDescription::SetUChar<Image>(&Image::setAlphaTopRight);
			Image::_setters["color_bottom_left"] = new PropertyDescription::Set<Image, hstr>(&Image::setSymbolicColorBottomLeft);
			Image::_setters["red_bottom_left"] = new PropertyDescription::SetUChar<Image>(&Image::setRedBottomLeft);
			Image::_setters["green_bottom_left"] = new PropertyDescription::SetUChar<Image>(&Image::setGreenBottomLeft);
			Image::_setters["blue_bottom_left"] = new PropertyDescription::SetUChar<Image>(&Image::setBlueBottomLeft);
			Image::_setters["alpha_bottom_left"] = new PropertyDescription::SetUChar<Image>(&Image::setAlphaBottomLeft);
			Image::_setters["color_bottom_right"] = new PropertyDescription::Set<Image, hstr>(&Image::setSymbolicColorBottomRight);
			Image::_setters["red_bottom_right"] = new PropertyDescription::SetUChar<Image>(&Image::setRedBottomRight);
			Image::_setters["green_bottom_right"] = new PropertyDescription::SetUChar<Image>(&Image::setGreenBottomRight);
			Image::_setters["blue_bottom_right"] = new PropertyDescription::SetUChar<Image>(&Image::setBlueBottomRight);
			Image::_setters["alpha_bottom_right"] = new PropertyDescription::SetUChar<Image>(&Image::setAlphaBottomRight);
			Image::_setters["use_additional_colors"] = new PropertyDescription::Set<Image, bool>(&Image::setUseAdditionalColors);
			Image::_setters["color_mode_factor"] = new PropertyDescription::Set<Image, float>(&Image::setColorModeFactor);
		}
		return Image::_setters;
	}

	void Image::setSymbolicColor(chstr value)
	{
		this->setColor(aprilui::_makeColor(value));
	}

	void Image::setColorTopRight(const april::Color& value)
	{
		this->colorTopRight = value;
		this->useAdditionalColors = true;
	}

	void Image::setSymbolicColorTopRight(chstr value)
	{
		this->setColorTopRight(aprilui::_makeColor(value));
	}

	void Image::setColorBottomLeft(const april::Color& value)
	{
		this->colorBottomLeft = value;
		this->useAdditionalColors = true;
	}

	void Image::setSymbolicColorBottomLeft(chstr value)
	{
		this->setColorBottomLeft(aprilui::_makeColor(value));
	}

	void Image::setColorBottomRight(const april::Color& value)
	{
		this->colorBottomRight = value;
		this->useAdditionalColors = true;
	}

	void Image::setSymbolicColorBottomRight(chstr value)
	{
		this->setColorBottomRight(aprilui::_makeColor(value));
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
		if (color.a == 0)
		{
			return;
		}
		if (this->useAdditionalColors)
		{
			this->_drawWithCorners(rect, color);
			return;
		}
		if (this->colorTopLeft.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		if (this->colorTopLeft != april::Color::White)
		{
			drawColor *= this->colorTopLeft;
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
		if (this->tryLoadTextureCoordinates())
		{
			for_iter (i, 0, APRILUI_IMAGE_MAX_VERTICES)
			{
				this->coloredVertices[i].u = this->vertices[i].u;
				this->coloredVertices[i].v = this->vertices[i].v;
			}
		}
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		april::rendersys->render(april::RenderOperation::TriangleList, this->vertices, APRILUI_IMAGE_MAX_VERTICES, drawColor);
	}

	void Image::_drawWithCorners(cgrectf rect, const april::Color& color)
	{
		april::Color topLeft = this->colorTopLeft * color;
		april::Color topRight = this->colorTopRight * color;
		april::Color bottomLeft = this->colorBottomLeft * color;
		april::Color bottomRight = this->colorBottomRight * color;
		if (topLeft.a == 0 && topRight.a == 0 && bottomLeft.a == 0 && bottomRight.a == 0)
		{
			return;
		}
		grectf drawRect = rect;
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			gvec2f sizeRatio = drawRect.getSize() / this->srcRect.getSize();
			drawRect += this->clipRect.getPosition() * sizeRatio;
			drawRect.setSize(this->clipRect.getSize() * sizeRatio);
		}
		this->coloredVertices[0].x = this->coloredVertices[2].x = this->coloredVertices[4].x = drawRect.left();
		this->coloredVertices[0].y = this->coloredVertices[1].y = this->coloredVertices[3].y = drawRect.top();
		this->coloredVertices[1].x = this->coloredVertices[3].x = this->coloredVertices[5].x = drawRect.right();
		this->coloredVertices[2].y = this->coloredVertices[4].y = this->coloredVertices[5].y = drawRect.bottom();
		this->coloredVertices[0].color = april::rendersys->getNativeColorUInt(topLeft);
		this->coloredVertices[1].color = this->coloredVertices[3].color = april::rendersys->getNativeColorUInt(topRight);
		this->coloredVertices[2].color = this->coloredVertices[4].color = april::rendersys->getNativeColorUInt(bottomLeft);
		this->coloredVertices[5].color = april::rendersys->getNativeColorUInt(bottomRight);
		this->_setDeviceTexture();
		if (this->tryLoadTextureCoordinates())
		{
			for_iter (i, 0, APRILUI_IMAGE_MAX_VERTICES)
			{
				this->coloredVertices[i].u = this->vertices[i].u;
				this->coloredVertices[i].v = this->vertices[i].v;
			}
		}
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		april::rendersys->render(april::RenderOperation::TriangleList, this->coloredVertices, APRILUI_IMAGE_MAX_VERTICES);
	}

	void Image::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		if (color.a == 0 || this->colorTopLeft.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		if (this->colorTopLeft != april::Color::White)
		{
			drawColor *= this->colorTopLeft;
			if (drawColor.a == 0)
			{
				return;
			}
		}
		this->_setDeviceTexture();
		if (this->tryLoadTextureCoordinates())
		{
			for_iter (i, 0, APRILUI_IMAGE_MAX_VERTICES)
			{
				this->coloredVertices[i].u = this->vertices[i].u;
				this->coloredVertices[i].v = this->vertices[i].v;
			}
		}
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
