/// @file
/// @version 5.1
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
	hmap<hstr, PropertyDescription> SkinImage::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> SkinImage::_getters;
	hmap<hstr, PropertyDescription::Accessor*> SkinImage::_setters;
	int SkinImage::maxRectCache = 30;

	SkinImage::SkinImage(Texture* texture, chstr name, cgrectf source) :
		Image(texture, name, source)
	{
		this->_skinCoordinatesCalculated = false;
		this->tiledBorders = false;
	}

	SkinImage::SkinImage(const SkinImage& other) :
		Image(other)
	{
		this->skinRect = other.skinRect;
		this->borderIndent = other.borderIndent;
		this->tiledBorders = other.tiledBorders;
		this->_skinCoordinatesCalculated = other._skinCoordinatesCalculated;
		this->_rectVertices = other._rectVertices;
	}

	MinimalImage* SkinImage::createInstance(Texture* texture, chstr name, cgrectf source)
	{
		return new SkinImage(texture, name, source);
	}

	hmap<hstr, PropertyDescription>& SkinImage::getPropertyDescriptions() const
	{
		if (SkinImage::_propertyDescriptions.size() == 0)
		{
			SkinImage::_propertyDescriptions = Image::getPropertyDescriptions();
			SkinImage::_propertyDescriptions["skin_rect"] = PropertyDescription("skin_rect", PropertyDescription::Type::Grectf);
			SkinImage::_propertyDescriptions["skin_position"] = PropertyDescription("skin_position", PropertyDescription::Type::Gvec2f);
			SkinImage::_propertyDescriptions["skin_size"] = PropertyDescription("skin_size", PropertyDescription::Type::Gvec2f);
			SkinImage::_propertyDescriptions["skin_x"] = PropertyDescription("skin_x", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions["skin_y"] = PropertyDescription("skin_y", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions["skin_w"] = PropertyDescription("skin_w", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions["skin_h"] = PropertyDescription("skin_h", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions["border_indent"] = PropertyDescription("border_indent", PropertyDescription::Type::Gvec2f);
			SkinImage::_propertyDescriptions["border_indent_x"] = PropertyDescription("border_indent_x", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions["border_indent_y"] = PropertyDescription("border_indent_y", PropertyDescription::Type::Float);
			SkinImage::_propertyDescriptions["tiled_borders"] = PropertyDescription("tiled_borders", PropertyDescription::Type::Bool);
		}
		return SkinImage::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& SkinImage::_getGetters() const
	{
		if (SkinImage::_getters.size() == 0)
		{
			SkinImage::_getters = Image::_getGetters();
			SkinImage::_getters["skin_rect"] = new PropertyDescription::GetGrectf<SkinImage>(&SkinImage::getSkinRect);
			SkinImage::_getters["skin_position"] = new PropertyDescription::GetGvec2f<SkinImage>(&SkinImage::getSkinPosition);
			SkinImage::_getters["skin_size"] = new PropertyDescription::GetGvec2f<SkinImage>(&SkinImage::getSkinSize);
			SkinImage::_getters["skin_x"] = new PropertyDescription::Get<SkinImage, float>(&SkinImage::getSkinX);
			SkinImage::_getters["skin_y"] = new PropertyDescription::Get<SkinImage, float>(&SkinImage::getSkinY);
			SkinImage::_getters["skin_w"] = new PropertyDescription::Get<SkinImage, float>(&SkinImage::getSkinWidth);
			SkinImage::_getters["skin_h"] = new PropertyDescription::Get<SkinImage, float>(&SkinImage::getSkinHeight);
			SkinImage::_getters["border_indent"] = new PropertyDescription::GetGvec2f<SkinImage>(&SkinImage::getBorderIndent);
			SkinImage::_getters["border_indent_x"] = new PropertyDescription::Get<SkinImage, float>(&SkinImage::getBorderIndentX);
			SkinImage::_getters["border_indent_y"] = new PropertyDescription::Get<SkinImage, float>(&SkinImage::getBorderIndentY);
			SkinImage::_getters["tiled_borders"] = new PropertyDescription::Get<SkinImage, bool>(&SkinImage::isTiledBorders);
		}
		return SkinImage::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& SkinImage::_getSetters() const
	{
		if (SkinImage::_setters.size() == 0)
		{
			SkinImage::_setters = Image::_getSetters();
			SkinImage::_setters["skin_rect"] = new PropertyDescription::SetGrectf<SkinImage>(&SkinImage::setSkinRect);
			SkinImage::_setters["skin_position"] = new PropertyDescription::SetGvec2f<SkinImage>(&SkinImage::setSkinPosition);
			SkinImage::_setters["skin_size"] = new PropertyDescription::SetGvec2f<SkinImage>(&SkinImage::setSkinSize);
			SkinImage::_setters["skin_x"] = new PropertyDescription::Set<SkinImage, float>(&SkinImage::setSkinX);
			SkinImage::_setters["skin_y"] = new PropertyDescription::Set<SkinImage, float>(&SkinImage::setSkinY);
			SkinImage::_setters["skin_w"] = new PropertyDescription::Set<SkinImage, float>(&SkinImage::setSkinWidth);
			SkinImage::_setters["skin_h"] = new PropertyDescription::Set<SkinImage, float>(&SkinImage::setSkinHeight);
			SkinImage::_setters["border_indent"] = new PropertyDescription::SetGvec2f<SkinImage>(&SkinImage::setBorderIndent);
			SkinImage::_setters["border_indent_x"] = new PropertyDescription::Set<SkinImage, float>(&SkinImage::setBorderIndentX);
			SkinImage::_setters["border_indent_y"] = new PropertyDescription::Set<SkinImage, float>(&SkinImage::setBorderIndentY);
			SkinImage::_setters["tiled_borders"] = new PropertyDescription::Set<SkinImage, bool>(&SkinImage::setTiledBorders);
		}
		return SkinImage::_setters;
	}

	void SkinImage::setSkinRect(cgrectf value)
	{
		if (this->skinRect != value)
		{
			this->skinRect = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinRect(const float& x, const float& y, const float& w, const float& h)
	{
		if (this->skinRect.x != x || this->skinRect.y != y || this->skinRect.w != w || this->skinRect.h != h)
		{
			this->skinRect.set(x, y, w, h);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinX(const float& value)
	{
		if (this->skinRect.x != value)
		{
			this->skinRect.x = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinY(const float& value)
	{
		if (this->skinRect.y != value)
		{
			this->skinRect.y = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinWidth(const float& value)
	{
		if (this->skinRect.w != value)
		{
			this->skinRect.w = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinHeight(const float& value)
	{
		if (this->skinRect.h != value)
		{
			this->skinRect.h = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinPosition(cgvec2f value)
	{
		if (this->skinRect.getPosition() != value)
		{
			this->skinRect.setPosition(value);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinPosition(const float& x, const float& y)
	{
		if (this->skinRect.x != x || this->skinRect.y != y)
		{
			this->skinRect.setPosition(x, y);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinSize(cgvec2f value)
	{
		if (this->skinRect.getSize() != value)
		{
			this->skinRect.setSize(value);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setSkinSize(const float& w, const float& h)
	{
		if (this->skinRect.w != w || this->skinRect.h != h)
		{
			this->skinRect.setSize(w, h);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setBorderIndent(cgvec2f value)
	{
		if (this->borderIndent != value)
		{
			this->borderIndent = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setBorderIndent(const float& x, const float& y)
	{
		if (this->borderIndent.x != x || this->borderIndent.y != y)
		{
			this->borderIndent.set(x, y);
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setBorderIndentX(const float& value)
	{
		if (this->borderIndent.x != value)
		{
			this->borderIndent.x = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setBorderIndentY(const float& value)
	{
		if (this->borderIndent.y != value)
		{
			this->borderIndent.y = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::setTiledBorders(const bool& value)
	{
		if (this->tiledBorders != value)
		{
			this->tiledBorders = value;
			this->_skinCoordinatesCalculated = false;
		}
	}

	void SkinImage::draw(cgrectf rect, const april::Color& color)
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
		if (!this->_textureCoordinatesLoaded || !this->_clipRectCalculated)
		{
			this->_skinCoordinatesCalculated = false;
			this->_clipRectCalculated = true; // not used for texture coordinates in this class so it's disabled
		}
		this->_setDeviceTexture();
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		grectf originalClipRect = this->clipRect; // prevents wrong calculations for src coordinates
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
			grectf indentedSkinRect(this->skinRect.getPosition() + this->borderIndent, this->skinRect.getSize() - this->borderIndent * 2);
			grectf clippedIndentedSkinRect = indentedSkinRect.clipped(grectf(0.0f, 0.0f, this->srcRect.getSize()));
			if (indentedSkinRect != clippedIndentedSkinRect)
			{
				hlog::warnf(logTag, "Image '%s' uses a border_indent (%g,%g) that is bigger than the actual border defined by skin_rect (%g,%g,%g,%g) resulting in (%g,%g,%g,%g)!",
					this->name.cStr(), this->borderIndent.x, this->borderIndent.y, this->skinRect.x, this->skinRect.y, this->skinRect.w, this->skinRect.h,
					clippedIndentedSkinRect.x, clippedIndentedSkinRect.y, clippedIndentedSkinRect.w, clippedIndentedSkinRect.h);
				indentedSkinRect = clippedIndentedSkinRect;
			}
			grectf skinRect = this->skinRect;
			gvec2f srcSize = this->srcRect.getSize();
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
			gvec2f pos[4];
			pos[0] = rect.getPosition();
			pos[3] = rect.getBottomRight();
			pos[1] = pos[0] + indentedSkinRect.getPosition();
			pos[2] = pos[0] + rect.getSize() - (srcRect.getSize() - indentedSkinRect.getBottomRight());
			gvec2f uv[4];
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
				gvec2f tile = this->skinRect.getSize();
				gvec2f border = rect.getSize() - (this->srcRect.getSize() - indentedSkinRect.getSize());
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
				gvec2f clipScale = rect.getSize() / this->srcRect.getSize();
				grectf clipRect(rect.getPosition() + this->clipRect.getPosition() * clipScale, this->clipRect.getSize() * clipScale);
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
