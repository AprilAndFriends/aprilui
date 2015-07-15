/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <april/Texture.h>
#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "SkinImage.h"
#include "Texture.h"

#define ADD_VERTICES(condition, index0, index1, index2, index3) \
	if (condition) \
	{ \
		this->_vertices += v[index0]; \
		this->_vertices += v[index1]; \
		this->_vertices += v[index2]; \
		this->_vertices += v[index1]; \
		this->_vertices += v[index2]; \
		this->_vertices += v[index3]; \
	}

namespace aprilui
{
	harray<PropertyDescription> SkinImage::_propertyDescriptions;

	SkinImage::SkinImage(Texture* texture, chstr name, grect source) : Image(texture, name, source)
	{
		this->_skinCoordinatesCalculated = false;
	}

	SkinImage::SkinImage(const SkinImage& other) : Image(other)
	{
		this->skinRect = other.skinRect;
		this->_lastDrawRect = other._lastDrawRect;
		this->_skinCoordinatesCalculated = other._skinCoordinatesCalculated;
		this->_vertices = other._vertices;
	}

	SkinImage::~SkinImage()
	{
	}

	harray<PropertyDescription> SkinImage::getPropertyDescriptions()
	{
		if (SkinImage::_propertyDescriptions.size() == 0)
		{
			SkinImage::_propertyDescriptions += PropertyDescription("skin_rect", PropertyDescription::GRECT);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_position", PropertyDescription::GVEC2);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_size", PropertyDescription::GVEC2);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_x", PropertyDescription::FLOAT);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_y", PropertyDescription::FLOAT);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_w", PropertyDescription::FLOAT);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_h", PropertyDescription::FLOAT);
		}
		return (SkinImage::_propertyDescriptions + Image::getPropertyDescriptions());
	}

	void SkinImage::setSkinRect(grect value)
	{
		if (this->skinRect != value)
		{
			this->skinRect = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinX(float value)
	{
		if (this->skinRect.x != value)
		{
			this->skinRect.x = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinY(float value)
	{
		if (this->skinRect.y != value)
		{
			this->skinRect.y = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinWidth(float value)
	{
		if (this->skinRect.w != value)
		{
			this->skinRect.w = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinHeight(float value)
	{
		if (this->skinRect.h != value)
		{
			this->skinRect.h = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinPosition(gvec2 value)
	{
		if (this->skinRect.getPosition() != value)
		{
			this->skinRect.setPosition(value);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinPosition(float x, float y)
	{
		if (this->skinRect.x != x || this->skinRect.y != y)
		{
			this->skinRect.setPosition(x, y);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinSize(gvec2 value)
	{
		if (this->skinRect.getSize() != value)
		{
			this->skinRect.setSize(value);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinSize(float w, float h)
	{
		if (this->skinRect.w != w || this->skinRect.h != h)
		{
			this->skinRect.setSize(w, h);
			this->_skinCoordinatesCalculated = false;
		}
	}

	hstr SkinImage::getProperty(chstr name)
	{
		if (name == "skin_rect")		return april::grectToHstr(this->getSkinRect());
		if (name == "skin_position")	return april::gvec2ToHstr(this->getSkinRect().getPosition());
		if (name == "skin_size")		return april::gvec2ToHstr(this->getSkinRect().getSize());
		if (name == "skin_x")			return this->getSkinRect().x;
		if (name == "skin_y")			return this->getSkinRect().y;
		if (name == "skin_w")			return this->getSkinRect().w;
		if (name == "skin_h")			return this->getSkinRect().h;
		return Image::getProperty(name);
	}

	bool SkinImage::setProperty(chstr name, chstr value)
	{
		if (name == "skin_rect")			this->setSkinRect(april::hstrToGrect(value));
		else if (name == "skin_position")	this->setSkinPosition(april::hstrToGvec2(value));
		else if (name == "skin_size")		this->setSkinSize(april::hstrToGvec2(value));
		else if (name == "skin_x")			this->setSkinX(value);
		else if (name == "skin_y")			this->setSkinY(value);
		else if (name == "skin_w")			this->setSkinWidth(value);
		else if (name == "skin_h")			this->setSkinHeight(value);
		else return Image::setProperty(name, value);
		return true;
	}

	void SkinImage::draw(grect rect, april::Color color)
	{
		if (this->color != april::Color::White)
		{
			color *= this->color;
		}
		if (color.a == 0)
		{
			return;
		}
		if (!this->_textureCoordinatesLoaded)
		{
			this->_skinCoordinatesCalculated = true;
		}
		if (this->texture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->texture->load();
		}
		april::rendersys->setTexture(this->texture->getTexture());
		april::rendersys->setTextureBlendMode(this->blendMode);
		april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
		this->tryLoadTextureCoordinates();
		if (this->_lastDrawRect != rect || !this->_skinCoordinatesCalculated)
		{
			this->_lastDrawRect = rect;
			this->_skinCoordinatesCalculated = true;
			this->_vertices.clear();
			gvec2 pos[4];
			pos[0] = rect.getPosition();
			pos[3] = rect.getBottomRight();
			pos[1] = pos[0] + this->skinRect.getPosition();
			pos[2] = pos[0] + rect.getSize() - (this->srcRect.getSize() - this->skinRect.getBottomRight());
			gvec2 uv[4];
			uv[0].set(Image::vertices[0].u, Image::vertices[0].v);
			uv[3].set(Image::vertices[3].u, Image::vertices[3].v);
			grect skinRect = this->skinRect;
			gvec2 srcSize = this->srcRect.getSize();
			if (this->rotated)
			{
				hswap(uv[0].x, uv[3].x); // undoes modified Image::vertices
				float y = skinRect.y;
				skinRect.y = skinRect.x;
				skinRect.x = srcSize.y - skinRect.h - y;
				hswap(skinRect.w, skinRect.h);
				hswap(srcSize.x, srcSize.y);
			}
			uv[1] = uv[0] + skinRect.getPosition() / srcSize * (uv[3] - uv[0]);
			uv[2] = uv[1] + skinRect.getSize() / srcSize * (uv[3] - uv[0]);
			april::TexturedVertex v[16];
			for_iter (j, 0, 4)
			{
				for_iter (i, 0, 4)
				{
					v[i + j * 4].x = pos[i].x;	v[i + j * 4].y = pos[j].y;
					if (!this->rotated)
					{
						v[i + j * 4].u = uv[i].x;		v[i + j * 4].v = uv[j].y;
					}
					else
					{
						v[i + j * 4].u = uv[3 - j].x;	v[i + j * 4].v = uv[i].y;
					}
				}
			}
			bool left = (this->skinRect.x > 0.0f);
			bool hcenter = (rect.w > this->srcRect.w - this->skinRect.w);
			bool right = (this->srcRect.w > this->skinRect.right());
			if (this->skinRect.y > 0.0f)
			{
				ADD_VERTICES(left, 0, 1, 4, 5);
				ADD_VERTICES(hcenter, 1, 2, 5, 6);
				ADD_VERTICES(right, 2, 3, 6, 7);
			}
			if (rect.h > this->srcRect.h - this->skinRect.h)
			{
				ADD_VERTICES(left, 4, 5, 8, 9);
				ADD_VERTICES(hcenter, 5, 6, 9, 10);
				ADD_VERTICES(right, 6, 7, 10, 11);
			}
			if (this->srcRect.h > this->skinRect.bottom())
			{
				ADD_VERTICES(left, 8, 9, 12, 13);
				ADD_VERTICES(hcenter, 9, 10, 13, 14);
				ADD_VERTICES(right, 10, 11, 14, 15);
			}
		}
		if (color.r < 255 || color.g < 255 || color.b < 255 || color.a < 255)
		{
			april::rendersys->render(april::RO_TRIANGLE_LIST, (april::TexturedVertex*)this->_vertices, this->_vertices.size(), color);
		}
		else
		{
			april::rendersys->render(april::RO_TRIANGLE_LIST, (april::TexturedVertex*)this->_vertices, this->_vertices.size());
		}
		april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		april::rendersys->setTextureColorMode(april::CM_DEFAULT);
	}

	void SkinImage::draw(harray<april::TexturedVertex> vertices, april::Color color)
	{
		hlog::warn(logTag, "SkinImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}

}
