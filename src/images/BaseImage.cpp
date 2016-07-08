/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "Dataset.h"
#include "Images.h"

namespace aprilui
{
	harray<PropertyDescription> BaseImage::_propertyDescriptions;

	BaseImage::BaseImage(chstr name) : Cloneable()
	{
		this->name = name;
		this->dataset = NULL;
		this->_textureCoordinatesLoaded = false;
	}

	BaseImage::BaseImage(const BaseImage& other) : Cloneable(other)
	{
		this->name = other.name;
		this->tag = other.tag;
		this->dataset = NULL;
		this->clipRect = other.clipRect;
		this->_textureCoordinatesLoaded = other._textureCoordinatesLoaded;
	}

	BaseImage::~BaseImage()
	{
	}
	
	harray<PropertyDescription> BaseImage::getPropertyDescriptions() const
	{
		if (BaseImage::_propertyDescriptions.size() == 0)
		{
			BaseImage::_propertyDescriptions += PropertyDescription("name", PropertyDescription::STRING);
			BaseImage::_propertyDescriptions += PropertyDescription("full_name", PropertyDescription::STRING);
			BaseImage::_propertyDescriptions += PropertyDescription("tag", PropertyDescription::STRING);
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

	void BaseImage::setName(chstr value)
	{
		if (this->dataset != NULL)
		{
			hlog::errorf(logTag, "Cannot set name of image '%s', it is already registered in a dataset!", this->name.cStr());
			return;
		}
		this->name = value;
	}

	hstr BaseImage::getFullName() const
	{
		return (this->dataset != NULL ? this->dataset->getName() + "." + this->name : this->name);
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
		if (name == "tag")				return this->getTag();
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
		if		(name == "name")			this->setName(value);
		else if (name == "tag")				this->setTag(value);
		else if (name == "size")			this->setSrcSize(april::hstrToGvec2(value));
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

	harray<Texture*> BaseImage::findTextures(harray<BaseImage*> baseImages)
	{
		baseImages.removeAll(NULL);
		baseImages.removedDuplicates();
		// get all images from composite images
		harray<Image*> images = baseImages.dynamicCast<Image*>();
		harray<CompositeImage*> compositeImages = baseImages.dynamicCast<CompositeImage*>();
		harray<CompositeImage::ImageRef> imageRefs;
		CompositeImage* compositeImage = NULL;
		harray<BaseImage*> newImages;
		while (compositeImages.size() > 0)
		{
			compositeImage = compositeImages.removeFirst();
			imageRefs = compositeImage->getImages();
			foreach (CompositeImage::ImageRef, it2, imageRefs)
			{
				compositeImage = dynamic_cast<CompositeImage*>((*it2).first);
				if (compositeImage != NULL)
				{
					compositeImages |= compositeImage;
				}
				else
				{
					newImages += (*it2).first;
				}
			}
		}
		newImages.removeDuplicates();
		// will filter out all actual images
		images += newImages.dynamicCast<Image*>();
		images.removeDuplicates();
		// get all textures
		HL_LAMBDA_CLASS(_imageTextures, Texture*, ((Image* const& image) { return image->getTexture(); }));
		harray<Texture*> textures = images.mapped(&_imageTextures::lambda);
		textures.removeAll(NULL);
		textures.removeDuplicates();
		return textures;
	}

}
