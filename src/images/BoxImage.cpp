/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hdir.h>
#include <hltypes/hltypesUtil.h>

#include "aprilui.h"
#include "BoxImage.h"
#include "Dataset.h"
#include "Exception.h"
#include "Texture.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> BoxImage::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> BoxImage::_getters;
	hmap<hstr, PropertyDescription::Accessor*> BoxImage::_setters;

	BoxImage::BoxImage(Texture* texture, chstr name, cgrectf source) : Image(texture, name, source)
	{
		this->realSrcSize = source.getSize();
	}

	BoxImage::BoxImage(const BoxImage& other) : Image(other)
	{
		this->drawPosition = other.drawPosition;
		this->realSrcSize = other.realSrcSize;
	}

	BoxImage::~BoxImage()
	{
	}

	MinimalImage* BoxImage::createInstance(Texture* texture, chstr name, cgrectf source)
	{
		return new BoxImage(texture, name, source);
	}
	
	hmap<hstr, PropertyDescription>& BoxImage::getPropertyDescriptions() const
	{
		if (BoxImage::_propertyDescriptions.size() == 0)
		{
			BoxImage::_propertyDescriptions = Image::getPropertyDescriptions();
			BoxImage::_propertyDescriptions["draw_position"] = PropertyDescription("draw_position", PropertyDescription::Type::Gvec2f);
			BoxImage::_propertyDescriptions["draw_x"] = PropertyDescription("draw_x", PropertyDescription::Type::Float);
			BoxImage::_propertyDescriptions["draw_y"] = PropertyDescription("draw_y", PropertyDescription::Type::Float);
			BoxImage::_propertyDescriptions["real_src_size"] = PropertyDescription("real_src_size", PropertyDescription::Type::Gvec2f);
			BoxImage::_propertyDescriptions["real_src_w"] = PropertyDescription("real_src_w", PropertyDescription::Type::Float);
			BoxImage::_propertyDescriptions["real_src_h"] = PropertyDescription("real_src_h", PropertyDescription::Type::Float);
		}
		return BoxImage::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& BoxImage::_getGetters() const
	{
		if (BoxImage::_getters.size() == 0)
		{
			BoxImage::_getters = Image::_getGetters();
			BoxImage::_getters["draw_position"] = new PropertyDescription::GetGvec2f<BoxImage>(&BoxImage::getDrawPosition);
			BoxImage::_getters["draw_x"] = new PropertyDescription::Get<BoxImage, float>(&BoxImage::getDrawX);
			BoxImage::_getters["draw_y"] = new PropertyDescription::Get<BoxImage, float>(&BoxImage::getDrawY);
			BoxImage::_getters["real_src_size"] = new PropertyDescription::GetGvec2f<BoxImage>(&BoxImage::getRealSrcSize);
			BoxImage::_getters["real_src_w"] = new PropertyDescription::Get<BoxImage, float>(&BoxImage::getRealSrcWidth);
			BoxImage::_getters["real_src_h"] = new PropertyDescription::Get<BoxImage, float>(&BoxImage::getRealSrcHeight);
		}
		return BoxImage::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& BoxImage::_getSetters() const
	{
		if (BoxImage::_setters.size() == 0)
		{
			BoxImage::_setters = Image::_getSetters();
			BoxImage::_setters["draw_position"] = new PropertyDescription::SetGvec2f<BoxImage>(&BoxImage::setDrawPosition);
			BoxImage::_setters["draw_x"] = new PropertyDescription::Set<BoxImage, float>(&BoxImage::setDrawX);
			BoxImage::_setters["draw_y"] = new PropertyDescription::Set<BoxImage, float>(&BoxImage::setDrawY);
			BoxImage::_setters["real_src_size"] = new PropertyDescription::SetGvec2f<BoxImage>(&BoxImage::setRealSrcSize);
			BoxImage::_setters["real_src_w"] = new PropertyDescription::Set<BoxImage, float>(&BoxImage::setRealSrcWidth);
			BoxImage::_setters["real_src_h"] = new PropertyDescription::Set<BoxImage, float>(&BoxImage::setRealSrcHeight);
		}
		return BoxImage::_setters;
	}

	void BoxImage::setRealSrcSize(cgvec2f value)
	{
		if (this->realSrcSize != value)
		{
			this->realSrcSize = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BoxImage::setRealSrcSize(float w, float h)
	{
		if (this->realSrcSize.x != h || this->realSrcSize.y != h)
		{
			this->realSrcSize.set(w, h);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BoxImage::setRealSrcWidth(float const& value)
	{
		if (this->realSrcSize.x != value)
		{
			this->realSrcSize.x = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BoxImage::setRealSrcHeight(float const& value)
	{
		if (this->realSrcSize.y != value)
		{
			this->realSrcSize.y = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	grectf BoxImage::_makeRealSrcRect() const
	{
		return grectf(this->srcRect.getPosition(), this->realSrcSize);
	}

	grectf BoxImage::_makeClippedSrcRect() const
	{
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			grectf realSrcRect = this->_makeRealSrcRect();
			return realSrcRect.clipped(this->clipRect - this->drawPosition + realSrcRect.getPosition());
		}
		return this->_makeRealSrcRect();
	}

	void BoxImage::draw(cgrectf rect, const april::Color& color)
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
		gvec2f sizeRatio = rect.getSize() / this->srcRect.getSize();
		grectf drawRect(rect.getPosition() + this->drawPosition * sizeRatio, this->realSrcSize * sizeRatio);
		if (this->clipRect.w > 0.0f && this->clipRect.h > 0.0f)
		{
			grectf realClipRect(rect.getPosition() + this->clipRect.getPosition() * sizeRatio, this->clipRect.getSize() * sizeRatio);
			drawRect.clip(realClipRect);
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

	void BoxImage::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		hlog::warn(logTag, "scedge::BoxImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}
	
}
