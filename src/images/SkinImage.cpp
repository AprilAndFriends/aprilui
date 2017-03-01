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

#define TILED_FIT_LIMIT 0.2f

#define CREATE_TRIANGLE(vertices, condition, index0, index1, index2, index3) \
	if (condition) \
	{ \
		vertices += v[index0]; \
		vertices += v[index1]; \
		vertices += v[index2]; \
		vertices += v[index1]; \
		vertices += v[index2]; \
		vertices += v[index3]; \
	}

#define SET_HELPER_VERTICES(helper, original, index0, index1, index2, index3) \
	helper[0] = original[index0]; \
	helper[1] = original[index1]; \
	helper[2] = original[index2]; \
	helper[3] = original[index1]; \
	helper[4] = original[index2]; \
	helper[5] = original[index3];

namespace aprilui
{
	harray<PropertyDescription> SkinImage::_propertyDescriptions;
	int SkinImage::maxRectCache = 30;

	SkinImage::SkinImage(Texture* texture, chstr name, grect source) : Image(texture, name, source)
	{
		this->_skinCoordinatesCalculated = false;
		this->tiledBorders = false;
	}

	SkinImage::SkinImage(const SkinImage& other) : Image(other)
	{
		this->skinRect = other.skinRect;
		this->tiledBorders = other.tiledBorders;
		this->_skinCoordinatesCalculated = other._skinCoordinatesCalculated;
		this->_rectVertices = other._rectVertices;
	}

	SkinImage::~SkinImage()
	{
	}

	harray<PropertyDescription> SkinImage::getPropertyDescriptions() const
	{
		if (SkinImage::_propertyDescriptions.size() == 0)
		{
			SkinImage::_propertyDescriptions += PropertyDescription("skin_rect", PropertyDescription::Type::Grect);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_position", PropertyDescription::Type::Gvec2);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_size", PropertyDescription::Type::Gvec2);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_x", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_y", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_w", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions += PropertyDescription("skin_h", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions += PropertyDescription("tiled_borders", PropertyDescription::Type::Bool);
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

	void SkinImage::setTiledBorders(bool value)
	{
		if (this->tiledBorders != value)
		{
			this->tiledBorders = value;
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
		if (name == "tiled_borders")	return this->isTiledBorders();
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
		else if (name == "tiled_borders")	this->setTiledBorders(value);
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
			this->_skinCoordinatesCalculated = false;
		}
		if (this->texture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->texture->load();
		}
		april::rendersys->setTexture(this->texture->getTexture());
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		this->tryLoadTextureCoordinates();
		if (!this->_skinCoordinatesCalculated)
		{
			this->_skinCoordinatesCalculated = true;
			this->_rectVertices.clear();
		}
		bool found = false;
		foreach (RectVertices, it, this->_rectVertices)
		{
			if ((*it).rect == rect)
			{
				this->_vertices = (*it).vertices;
				found = true;
				break;
			}
		}
		if (!found)
		{
			this->_vertices.clear();
			if (this->_rectVertices.size() >= SkinImage::maxRectCache)
			{
				this->_rectVertices.clear();
			}
			// which pieces will be rendered
			bool left = (this->skinRect.x > 0.0f);
			bool hcenter = (rect.w > this->srcRect.w - this->skinRect.w);
			bool right = (this->srcRect.w > this->skinRect.right());
			bool top = (this->skinRect.y > 0.0f);
			bool vcenter = (rect.h > this->srcRect.h - this->skinRect.h);
			bool bottom = (this->srcRect.h > this->skinRect.bottom());
			grect skinRect = this->skinRect;
			gvec2 srcSize = this->srcRect.getSize();
			// modifying skinRect if only borders are visible
			if (!hcenter)
			{
				float difference = this->srcRect.w - this->skinRect.w - rect.w;
				skinRect.x -= difference * 0.5f;
				skinRect.w += difference;
			}
			if (!vcenter)
			{
				float difference = this->srcRect.h - this->skinRect.h - rect.h;
				skinRect.y -= difference * 0.5f;
				skinRect.h += difference;
			}
			// coordinate translation
			gvec2 pos[4];
			pos[0] = rect.getPosition();
			pos[3] = rect.getBottomRight();
			pos[1] = pos[0] + skinRect.getPosition();
			pos[2] = pos[0] + rect.getSize() - (srcRect.getSize() - skinRect.getBottomRight());
			gvec2 uv[4];
			uv[0].set(Image::vertices[0].u, Image::vertices[0].v);
			uv[3].set(Image::vertices[APRILUI_IMAGE_MAX_VERTICES - 1].u, Image::vertices[APRILUI_IMAGE_MAX_VERTICES - 1].v);
			// take rotation into account
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
			// setting up the vertices grid
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
			if (!this->tiledBorders)
			{
				// when tiled borders are disabled, the vertices only have to be arranged into triangles
				if (top)
				{
					CREATE_TRIANGLE(this->_vertices, left, 0, 1, 4, 5);
					CREATE_TRIANGLE(this->_vertices, hcenter, 1, 2, 5, 6);
					CREATE_TRIANGLE(this->_vertices, right, 2, 3, 6, 7);
				}
				if (vcenter)
				{
					CREATE_TRIANGLE(this->_vertices, left, 4, 5, 8, 9);
					CREATE_TRIANGLE(this->_vertices, hcenter, 5, 6, 9, 10);
					CREATE_TRIANGLE(this->_vertices, right, 6, 7, 10, 11);
				}
				if (bottom)
				{
					CREATE_TRIANGLE(this->_vertices, left, 8, 9, 12, 13);
					CREATE_TRIANGLE(this->_vertices, hcenter, 9, 10, 13, 14);
					CREATE_TRIANGLE(this->_vertices, right, 10, 11, 14, 15);
				}
			}
			else
			{
				// when tiled borders are enabled, a more complicated procedure is required to arrange all vertices into triangles
				april::TexturedVertex w[6];
				gvec2 tile = this->skinRect.getSize();
				gvec2 border = rect.getSize() - (this->srcRect.getSize() - this->skinRect.getSize());
				float tileCountX = border.x / tile.x;
				float tileCountY = border.y / tile.y;
				int countX = (int)tileCountX;
				int countY = (int)tileCountY;
				if (tileCountX - countX > TILED_FIT_LIMIT || countX == 0 && tileCountX > 0.0f)
				{
					++countX;
				}
				if (tileCountY - countY > TILED_FIT_LIMIT || countY == 0 && tileCountY > 0.0f)
				{
					++countY;
				}
				if (countX > 0)
				{
					tile.x = border.x / countX;
				}
				if (countY > 0)
				{
					tile.y = border.y / countY;
				}
				int i = 0;
				int j = 0;
				if (top)
				{
					CREATE_TRIANGLE(this->_vertices, left, 0, 1, 4, 5);
					// top center
					if (hcenter)
					{
						SET_HELPER_VERTICES(w, v, 1, 2, 5, 6);
						i = countX;
						while (true)
						{
							w[1].x = w[3].x = w[5].x = w[0].x + tile.x;
							this->_vertices.add(w, 6);
							--i;
							if (i <= 0)
							{
								break;
							}
							w[0].x = w[2].x = w[4].x = w[1].x;
						}
					}
					CREATE_TRIANGLE(this->_vertices, right, 2, 3, 6, 7);
				}
				if (vcenter)
				{
					// center left
					if (left)
					{
						SET_HELPER_VERTICES(w, v, 4, 5, 8, 9);
						j = countY;
						while (true)
						{
							w[2].y = w[4].y = w[5].y = w[0].y + tile.y;
							this->_vertices.add(w, 6);
							--j;
							if (j <= 0)
							{
								break;
							}
							w[0].y = w[1].y = w[3].y = w[2].y;
						}
					}
					// center center
					if (hcenter)
					{
						SET_HELPER_VERTICES(w, v, 5, 6, 9, 10);
						j = countY;
						while (true)
						{
							w[2].y = w[4].y = w[5].y = w[0].y + tile.y;
							i = countX;
							while (true)
							{
								w[1].x = w[3].x = w[5].x = w[0].x + tile.x;
								this->_vertices.add(w, 6);
								--i;
								if (i <= 0)
								{
									break;
								}
								w[0].x = w[2].x = w[4].x = w[1].x;
							}
							w[1].x = w[3].x = w[5].x = w[1].x - countX * tile.x;
							w[0].x = w[2].x = w[4].x = w[1].x;
							this->_vertices.add(w, 6);
							--j;
							if (j <= 0)
							{
								break;
							}
							w[0].y = w[1].y = w[3].y = w[2].y;
						}
					}
					// center right
					if (right)
					{
						SET_HELPER_VERTICES(w, v, 6, 7, 10, 11);
						j = countY;
						while (true)
						{
							w[2].y = w[4].y = w[5].y = w[0].y + tile.y;
							this->_vertices.add(w, 6);
							--j;
							if (j <= 0)
							{
								break;
							}
							w[0].y = w[1].y = w[3].y = w[2].y;
						}
					}
				}
				if (bottom)
				{
					CREATE_TRIANGLE(this->_vertices, left, 8, 9, 12, 13);
					// bottom center
					if (hcenter)
					{
						SET_HELPER_VERTICES(w, v, 9, 10, 13, 14);
						i = countX;
						while (true)
						{
							w[1].x = w[3].x = w[5].x = w[0].x + tile.x;
							this->_vertices.add(w, 6);
							--i;
							if (i <= 0)
							{
								break;
							}
							w[0].x = w[2].x = w[4].x = w[1].x;
						}
					}
					CREATE_TRIANGLE(this->_vertices, right, 10, 11, 14, 15);
				}
			}
			RectVertices rectVertices;
			rectVertices.rect = rect;
			rectVertices.vertices = this->_vertices;
			this->_rectVertices += rectVertices;
		}
		april::rendersys->render(april::RO_TRIANGLE_LIST, (april::TexturedVertex*)this->_vertices, this->_vertices.size(), color);
	}

	void SkinImage::draw(harray<april::TexturedVertex> vertices, april::Color color)
	{
		hlog::warn(logTag, "SkinImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}

}
