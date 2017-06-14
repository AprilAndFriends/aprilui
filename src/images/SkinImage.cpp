/// @file
/// @version 4.3
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

	SkinImage::SkinImage(Texture* texture, chstr name, cgrect source) : Image(texture, name, source)
	{
		this->_skinCoordinatesCalculated = false;
		this->tiledBorders = false;
	}

	SkinImage::SkinImage(const SkinImage& other) : Image(other)
	{
		this->skinRect = other.skinRect;
		this->borderIndent = other.borderIndent;
		this->tiledBorders = other.tiledBorders;
		this->_skinCoordinatesCalculated = other._skinCoordinatesCalculated;
		this->_rectVertices = other._rectVertices;
	}

	SkinImage::~SkinImage()
	{
	}

	Image* SkinImage::createInstance(Texture* texture, chstr name, cgrect source)
	{
		return new SkinImage(texture, name, source);
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
			SkinImage::_propertyDescriptions += PropertyDescription("border_indent", PropertyDescription::Type::Gvec2);
			SkinImage::_propertyDescriptions += PropertyDescription("border_indent_x", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions += PropertyDescription("border_indent_y", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions += PropertyDescription("tiled_borders", PropertyDescription::Type::Bool);
		}
		return (SkinImage::_propertyDescriptions + Image::getPropertyDescriptions());
	}

	void SkinImage::setSkinRect(cgrect value)
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

	void SkinImage::setSkinPosition(cgvec2 value)
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

	void SkinImage::setSkinSize(cgvec2 value)
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

	void SkinImage::setBorderIndent(cgvec2 value)
	{
		if (this->borderIndent != value)
		{
			this->borderIndent = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setBorderIndent(float x, float y)
	{
		if (this->borderIndent.x != x || this->borderIndent.y != y)
		{
			this->borderIndent.set(x, y);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setBorderIndentX(float value)
	{
		if (this->borderIndent.x != value)
		{
			this->borderIndent.x = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setBorderIndentY(float value)
	{
		if (this->borderIndent.y != value)
		{
			this->borderIndent.y = value;
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
		if (name == "border_indent")	return april::gvec2ToHstr(this->getBorderIndent());
		if (name == "border_indent_x")	return this->getBorderIndent().x;
		if (name == "border_indent_y")	return this->getBorderIndent().y;
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
		else if (name == "border_indent")	this->setBorderIndent(april::hstrToGvec2(value));
		else if (name == "border_indent_x")	this->setBorderIndentX(value);
		else if (name == "border_indent_y")	this->setBorderIndentY(value);
		else if (name == "tiled_borders")	this->setTiledBorders(value);
		else return Image::setProperty(name, value);
		return true;
	}

	void SkinImage::draw(cgrect rect, const april::Color& color)
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
		if (!this->_textureCoordinatesLoaded || !this->_clipRectCalculated)
		{
			this->_skinCoordinatesCalculated = false;
			this->_clipRectCalculated = true; // not used for texture coordinates in this class so it's disabled
		}
		if (this->texture != NULL) // to prevent a crash in Texture::load so that a possible crash happens below instead
		{
			this->texture->load();
		}
		april::rendersys->setTexture(this->texture->getTexture());
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		grect originalClipRect = this->clipRect; // prevents wrong calculations for src coordinates
		this->clipRect.set(0.0f, 0.0f, 0.0f, 0.0f);
		this->tryLoadTextureCoordinates();
		this->clipRect = originalClipRect;
		if (!this->_skinCoordinatesCalculated)
		{
			this->_skinCoordinatesCalculated = true;
			this->_rectVertices.clear();
		}
		bool found = false;
		foreach (RectVertices, it, this->_rectVertices)
		{
			if ((*it).first == rect)
			{
				this->_vertices = (*it).second;
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
			grect indentedSkinRect(this->skinRect.getPosition() + this->borderIndent, this->skinRect.getSize() - this->borderIndent * 2);
			grect clippedIndentedSkinRect = indentedSkinRect.clipped(grect(0.0f, 0.0f, this->srcRect.getSize()));
			if (indentedSkinRect != clippedIndentedSkinRect)
			{
				hlog::warnf(logTag, "Image '%s' uses a border_indent (%g,%g) that is bigger than the actual border defined by skin_rect (%g,%g,%g,%g) resulting in (%g,%g,%g,%g)!",
					this->name.cStr(), this->borderIndent.x, this->borderIndent.y, this->skinRect.x, this->skinRect.y, this->skinRect.w, this->skinRect.h,
					clippedIndentedSkinRect.x, clippedIndentedSkinRect.y, clippedIndentedSkinRect.w, clippedIndentedSkinRect.h);
				indentedSkinRect = clippedIndentedSkinRect;
			}
			grect skinRect = this->skinRect;
			gvec2 srcSize = this->srcRect.getSize();
			bool left = (indentedSkinRect.x > 0.0f);
			bool hcenter = (rect.w > this->srcRect.w - indentedSkinRect.w);
			bool right = (this->srcRect.w > indentedSkinRect.right());
			bool top = (indentedSkinRect.y > 0.0f);
			bool vcenter = (rect.h > this->srcRect.h - indentedSkinRect.h);
			bool bottom = (this->srcRect.h > indentedSkinRect.bottom());
			// modifying skinRect if only borders are visible
			if (!hcenter)
			{
				float difference = this->srcRect.w - skinRect.w - rect.w;
				skinRect.x -= difference * 0.5f;
				skinRect.w += difference;
				difference = this->srcRect.w - indentedSkinRect.w - rect.w;
				indentedSkinRect.x -= difference * 0.5f;
				indentedSkinRect.w += difference;
			}
			if (!vcenter)
			{
				float difference = this->srcRect.h - skinRect.h - rect.h;
				skinRect.y -= difference * 0.5f;
				skinRect.h += difference;
				difference = this->srcRect.h - indentedSkinRect.h - rect.h;
				indentedSkinRect.y -= difference * 0.5f;
				indentedSkinRect.h += difference;
			}
			// coordinate translation
			gvec2 pos[4];
			pos[0] = rect.getPosition();
			pos[3] = rect.getBottomRight();
			pos[1] = pos[0] + indentedSkinRect.getPosition();
			pos[2] = pos[0] + rect.getSize() - (srcRect.getSize() - indentedSkinRect.getBottomRight());
			gvec2 uv[4];
			uv[0].set(Image::vertices[0].u, Image::vertices[0].v);
			uv[3].set(Image::vertices[APRILUI_IMAGE_MAX_VERTICES - 1].u, Image::vertices[APRILUI_IMAGE_MAX_VERTICES - 1].v);
			uv[1] = uv[0] + skinRect.getPosition() / srcSize * (uv[3] - uv[0]);
			uv[2] = uv[1] + skinRect.getSize() / srcSize * (uv[3] - uv[0]);
			april::TexturedVertex v[16];
			// setting up the vertices grid
			for_iter (j, 0, 4)
			{
				for_iter (i, 0, 4)
				{
					v[i + j * 4].x = pos[i].x;	v[i + j * 4].y = pos[j].y;
					v[i + j * 4].u = uv[i].x;	v[i + j * 4].v = uv[j].y;
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
				gvec2 border = rect.getSize() - (this->srcRect.getSize() - indentedSkinRect.getSize());
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
			// clipped rect
			if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
			{
				float range = 0.0f;
				float offset = 0.0f;
				gvec2 clipScale = rect.getSize() / this->srcRect.getSize();
				grect clipRect(rect.getPosition() + this->clipRect.getPosition() * clipScale, this->clipRect.getSize() * clipScale);
				for_iter (i, 0, this->_vertices.size() / 6)
				{
					// remove out-of bounds
					if (this->_vertices[i * 6 + 1].x <= clipRect.x || this->_vertices[i * 6].x >= clipRect.right() || this->_vertices[i * 6 + 2].y <= clipRect.y || this->_vertices[i * 6].y >= clipRect.bottom())
					{
						this->_vertices.removeAt(i * 6, 6);
						--i;
					}
					else
					{
						// cut coordinates and correct src
						if (this->_vertices[i * 6].x < clipRect.x)
						{
							range = this->_vertices[i * 6 + 1].x - this->_vertices[i * 6].x;
							offset = clipRect.x - this->_vertices[i * 6].x;
							this->_vertices[i * 6].x += offset;		this->_vertices[i * 6 + 2].x += offset;	this->_vertices[i * 6 + 4].x += offset;
							offset *= (this->_vertices[i * 6 + 1].u - this->_vertices[i * 6].u) / range;
							this->_vertices[i * 6].u += offset;		this->_vertices[i * 6 + 2].u += offset;	this->_vertices[i * 6 + 4].u += offset;
						}
						if (this->_vertices[i * 6 + 1].x > clipRect.right())
						{
							range = this->_vertices[i * 6 + 1].x - this->_vertices[i * 6].x;
							offset = this->_vertices[i * 6 + 1].x - clipRect.right();
							this->_vertices[i * 6 + 1].x -= offset;	this->_vertices[i * 6 + 3].x -= offset;	this->_vertices[i * 6 + 5].x -= offset;
							offset *= (this->_vertices[i * 6 + 1].u - this->_vertices[i * 6].u) / range;
							this->_vertices[i * 6 + 1].u -= offset;	this->_vertices[i * 6 + 3].u -= offset;	this->_vertices[i * 6 + 5].u -= offset;
						}
						if (this->_vertices[i * 6].y < clipRect.y)
						{
							range = this->_vertices[i * 6 + 2].y - this->_vertices[i * 6].y;
							offset = clipRect.y - this->_vertices[i * 6].y;
							this->_vertices[i * 6].y += offset;		this->_vertices[i * 6 + 1].y += offset;	this->_vertices[i * 6 + 3].y += offset;
							offset *= (this->_vertices[i * 6 + 2].v - this->_vertices[i * 6].v) / range;
							this->_vertices[i * 6].v += offset;		this->_vertices[i * 6 + 1].v += offset;	this->_vertices[i * 6 + 3].v += offset;
						}
						if (this->_vertices[i * 6 + 2].y > clipRect.bottom())
						{
							range = this->_vertices[i * 6 + 2].y - this->_vertices[i * 6].y;
							offset = this->_vertices[i * 6 + 2].y - clipRect.bottom();
							this->_vertices[i * 6 + 2].y -= offset;	this->_vertices[i * 6 + 4].y -= offset;	this->_vertices[i * 6 + 5].y -= offset;
							offset *= (this->_vertices[i * 6 + 2].v - this->_vertices[i * 6].v) / range;
							this->_vertices[i * 6 + 2].v -= offset;	this->_vertices[i * 6 + 4].v -= offset;	this->_vertices[i * 6 + 5].v -= offset;
						}
					}
				}
			}
			// finished coordinates
			this->_rectVertices += RectVertices(rect, this->_vertices);
		}
		if (this->_vertices.size() > 0)
		{
			april::rendersys->render(april::RenderOperation::TriangleList, (april::TexturedVertex*)this->_vertices, this->_vertices.size(), drawColor);
		}
	}

	void SkinImage::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		hlog::warn(logTag, "SkinImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}

}
