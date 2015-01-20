/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "BaseImage.h"
#include "Dataset.h"

namespace aprilui
{
	harray<PropertyDescription> BaseImage::_propertyDescriptions;

	BaseImage::BaseImage(chstr name)
	{
		this->name = name;
		this->dataset = NULL;
		this->_textureCoordinatesLoaded = false;
	}

	BaseImage::BaseImage(BaseImage& other, chstr name)
	{
		this->name = name;
		this->dataset = NULL;
		this->clipRect = other.clipRect;
		this->_textureCoordinatesLoaded = false;
	}

	BaseImage::~BaseImage()
	{
	}
	
	hstr BaseImage::getFullName()
	{
		return (this->dataset != NULL ? this->dataset->getName() + "." + this->name : this->name);
	}

	harray<PropertyDescription> BaseImage::getPropertyDescriptions()
	{
		if (BaseImage::_propertyDescriptions.size() == 0)
		{
			BaseImage::_propertyDescriptions += PropertyDescription("name", PropertyDescription::STRING);
			BaseImage::_propertyDescriptions += PropertyDescription("full_name", PropertyDescription::STRING);
			BaseImage::_propertyDescriptions += PropertyDescription("dataset", PropertyDescription::STRING);
			BaseImage::_propertyDescriptions += PropertyDescription("size", PropertyDescription::GVEC2);
			BaseImage::_propertyDescriptions += PropertyDescription("w", PropertyDescription::FLOAT);
			BaseImage::_propertyDescriptions += PropertyDescription("h", PropertyDescription::FLOAT);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_rect", PropertyDescription::GRECT);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_position", PropertyDescription::GVEC2);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_size", PropertyDescription::GVEC2);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_x", PropertyDescription::FLOAT);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_y", PropertyDescription::FLOAT);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_w", PropertyDescription::FLOAT);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_h", PropertyDescription::FLOAT);
		}
		return BaseImage::_propertyDescriptions;
	}

	void BaseImage::setClipRect(grect value)
	{
		if (this->clipRect != value)
		{
			this->clipRect = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BaseImage::setClipX(float value)
	{
		if (this->clipRect.x != value)
		{
			this->clipRect.x = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BaseImage::setClipY(float value)
	{
		if (this->clipRect.y != value)
		{
			this->clipRect.y = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BaseImage::setClipWidth(float value)
	{
		if (this->clipRect.w != value)
		{
			this->clipRect.w = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BaseImage::setClipHeight(float value)
	{
		if (this->clipRect.h != value)
		{
			this->clipRect.h = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BaseImage::setClipPosition(gvec2 value)
	{
		if (this->clipRect.getPosition() != value)
		{
			this->clipRect.setPosition(value);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BaseImage::setClipPosition(float x, float y)
	{
		if (this->clipRect.x != x || this->clipRect.y != y)
		{
			this->clipRect.setPosition(x, y);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BaseImage::setClipSize(gvec2 value)
	{
		if (this->clipRect.getSize() != value)
		{
			this->clipRect.setSize(value);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void BaseImage::setClipSize(float w, float h)
	{
		if (this->clipRect.w != w || this->clipRect.h != h)
		{
			this->clipRect.setSize(w, h);
			this->_textureCoordinatesLoaded = false;
		}
	}

	hstr BaseImage::getProperty(chstr name)
	{
		if (name == "name")				return this->getName();
		if (name == "full_name")		return this->getFullName();
		if (name == "dataset")
		{
			return (this->dataset != NULL ? this->dataset->getName() : "");
		}
		if (name == "size")				return april::gvec2ToHstr(this->getSrcSize());
		if (name == "w")				return this->getSrcWidth();
		if (name == "h")				return this->getSrcHeight();
		if (name == "clip_rect")		return april::grectToHstr(this->getClipRect());
		if (name == "clip_position")	return april::gvec2ToHstr(this->getClipRect().getPosition());
		if (name == "clip_size")		return april::gvec2ToHstr(this->getClipRect().getSize());
		if (name == "clip_x")			return this->getClipRect().x;
		if (name == "clip_y")			return this->getClipRect().y;
		if (name == "clip_w")			return this->getClipRect().w;
		if (name == "clip_h")			return this->getClipRect().h;
		return "";
	}

	bool BaseImage::setProperty(chstr name, chstr value)
	{
		if		(name == "size")			this->setSrcSize(april::hstrToGvec2(value));
		else if	(name == "w")				this->setSrcWidth(value);
		else if	(name == "h")				this->setSrcHeight(value);
		else if (name == "clip_rect")		this->setClipRect(april::hstrToGrect(value));
		else if (name == "clip_position")	this->clipRect.setPosition(april::hstrToGvec2(value));
		else if (name == "clip_size")		this->clipRect.setSize(april::hstrToGvec2(value));
		else if (name == "clip_x")			this->setClipX(value);
		else if (name == "clip_y")			this->setClipY(value);
		else if (name == "clip_w")			this->setClipWidth(value);
		else if (name == "clip_h")			this->setClipHeight(value);
		else return false;
		return true;
	}

}