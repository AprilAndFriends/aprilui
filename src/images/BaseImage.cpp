/// @file
/// @version 5.0
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
	hmap<hstr, PropertyDescription::Accessor*> BaseImage::_getters;
	hmap<hstr, PropertyDescription::Accessor*> BaseImage::_setters;

	BaseImage::BaseImage(chstr name) : Cloneable()
	{
		this->name = name;
		this->dataset = NULL;
		this->_clipRectCalculated = false;
	}

	BaseImage::BaseImage(const BaseImage& other) : Cloneable(other)
	{
		this->name = other.name;
		this->tag = other.tag;
		this->dataset = NULL;
		this->clipRect = other.clipRect;
		this->_clipRectCalculated = false;
	}

	BaseImage::~BaseImage()
	{
	}
	
	harray<PropertyDescription> BaseImage::getPropertyDescriptions() const
	{
		if (BaseImage::_propertyDescriptions.size() == 0)
		{
			BaseImage::_propertyDescriptions += PropertyDescription("name", PropertyDescription::Type::String);
			BaseImage::_propertyDescriptions += PropertyDescription("full_name", PropertyDescription::Type::String);
			BaseImage::_propertyDescriptions += PropertyDescription("tag", PropertyDescription::Type::String);
			BaseImage::_propertyDescriptions += PropertyDescription("dataset", PropertyDescription::Type::String);
			BaseImage::_propertyDescriptions += PropertyDescription("size", PropertyDescription::Type::Gvec2);
			BaseImage::_propertyDescriptions += PropertyDescription("w", PropertyDescription::Type::Float);
			BaseImage::_propertyDescriptions += PropertyDescription("h", PropertyDescription::Type::Float);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_rect", PropertyDescription::Type::Grect);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_position", PropertyDescription::Type::Gvec2);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_size", PropertyDescription::Type::Gvec2);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_x", PropertyDescription::Type::Float);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_y", PropertyDescription::Type::Float);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_w", PropertyDescription::Type::Float);
			BaseImage::_propertyDescriptions += PropertyDescription("clip_h", PropertyDescription::Type::Float);
		}
		return BaseImage::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& BaseImage::_getGetters() const
	{
		if (BaseImage::_getters.size() == 0)
		{
			BaseImage::_getters["name"] = new PropertyDescription::Get<BaseImage, hstr>(&BaseImage::getName);
			BaseImage::_getters["full_name"] = new PropertyDescription::Get<BaseImage, hstr>(&BaseImage::getFullName);
			BaseImage::_getters["tag"] = new PropertyDescription::Get<BaseImage, hstr>(&BaseImage::getTag);
			BaseImage::_getters["size"] = new PropertyDescription::GetGvec2<BaseImage>(&BaseImage::getSrcSize);
			BaseImage::_getters["w"] = new PropertyDescription::Get<BaseImage, float>(&BaseImage::getSrcWidth);
			BaseImage::_getters["h"] = new PropertyDescription::Get<BaseImage, float>(&BaseImage::getSrcHeight);
			BaseImage::_getters["clip_rect"] = new PropertyDescription::GetGrect<BaseImage>(&BaseImage::getClipRect);
			BaseImage::_getters["clip_position"] = new PropertyDescription::GetGvec2<BaseImage>(&BaseImage::getClipPosition);
			BaseImage::_getters["clip_size"] = new PropertyDescription::GetGvec2<BaseImage>(&BaseImage::getClipSize);
			BaseImage::_getters["clip_x"] = new PropertyDescription::Get<BaseImage, float>(&BaseImage::getClipX);
			BaseImage::_getters["clip_y"] = new PropertyDescription::Get<BaseImage, float>(&BaseImage::getClipY);
			BaseImage::_getters["clip_w"] = new PropertyDescription::Get<BaseImage, float>(&BaseImage::getClipWidth);
			BaseImage::_getters["clip_h"] = new PropertyDescription::Get<BaseImage, float>(&BaseImage::getClipHeight);
		}
		return BaseImage::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& BaseImage::_getSetters() const
	{
		if (BaseImage::_setters.size() == 0)
		{
			BaseImage::_setters["name"] = new PropertyDescription::Set<BaseImage, hstr>(&BaseImage::setName);
			BaseImage::_setters["tag"] = new PropertyDescription::Set<BaseImage, hstr>(&BaseImage::setTag);
			BaseImage::_setters["size"] = new PropertyDescription::SetGvec2<BaseImage>(&BaseImage::setSrcSize);
			BaseImage::_setters["w"] = new PropertyDescription::Set<BaseImage, float>(&BaseImage::setSrcWidth);
			BaseImage::_setters["h"] = new PropertyDescription::Set<BaseImage, float>(&BaseImage::setSrcHeight);
			BaseImage::_setters["clip_rect"] = new PropertyDescription::SetGrect<BaseImage>(&BaseImage::setClipRect);
			BaseImage::_setters["clip_position"] = new PropertyDescription::SetGvec2<BaseImage>(&BaseImage::setClipPosition);
			BaseImage::_setters["clip_size"] = new PropertyDescription::SetGvec2<BaseImage>(&BaseImage::setClipSize);
			BaseImage::_setters["clip_x"] = new PropertyDescription::Set<BaseImage, float>(&BaseImage::setClipX);
			BaseImage::_setters["clip_y"] = new PropertyDescription::Set<BaseImage, float>(&BaseImage::setClipY);
			BaseImage::_setters["clip_w"] = new PropertyDescription::Set<BaseImage, float>(&BaseImage::setClipWidth);
			BaseImage::_setters["clip_h"] = new PropertyDescription::Set<BaseImage, float>(&BaseImage::setClipHeight);
		}
		return BaseImage::_setters;
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

	void BaseImage::setClipRect(cgrect value)
	{
		if (this->clipRect != value)
		{
			this->clipRect = value;
			this->_clipRectCalculated = false;
		}
	}

	void BaseImage::setClipX(const float& value)
	{
		if (this->clipRect.x != value)
		{
			this->clipRect.x = value;
			this->_clipRectCalculated = false;
		}
	}

	void BaseImage::setClipY(const float& value)
	{
		if (this->clipRect.y != value)
		{
			this->clipRect.y = value;
			this->_clipRectCalculated = false;
		}
	}

	void BaseImage::setClipWidth(const float& value)
	{
		if (this->clipRect.w != value)
		{
			this->clipRect.w = value;
			this->_clipRectCalculated = false;
		}
	}

	void BaseImage::setClipHeight(const float& value)
	{
		if (this->clipRect.h != value)
		{
			this->clipRect.h = value;
			this->_clipRectCalculated = false;
		}
	}

	void BaseImage::setClipPosition(cgvec2 value)
	{
		if (this->clipRect.getPosition() != value)
		{
			this->clipRect.setPosition(value);
			this->_clipRectCalculated = false;
		}
	}

	void BaseImage::setClipPosition(const float& x, const float& y)
	{
		if (this->clipRect.x != x || this->clipRect.y != y)
		{
			this->clipRect.setPosition(x, y);
			this->_clipRectCalculated = false;
		}
	}

	void BaseImage::setClipSize(cgvec2 value)
	{
		if (this->clipRect.getSize() != value)
		{
			this->clipRect.setSize(value);
			this->_clipRectCalculated = false;
		}
	}

	void BaseImage::setClipSize(const float& w, const float& h)
	{
		if (this->clipRect.w != w || this->clipRect.h != h)
		{
			this->clipRect.setSize(w, h);
			this->_clipRectCalculated = false;
		}
	}

	hstr BaseImage::getProperty(chstr name)
	{
		if (name == "dataset")
		{
			return (this->dataset != NULL ? this->dataset->getName() : "");
		}
		PropertyDescription::Accessor* getter = this->_getGetters().tryGet(name, NULL);
		if (getter != NULL)
		{
			hstr result;
			getter->execute(this, result);
			return result;
		}
		return "";
	}

	bool BaseImage::setProperty(chstr name, chstr value)
	{
		PropertyDescription::Accessor* setter = this->_getSetters().tryGet(name, NULL);
		if (setter != NULL)
		{
			hstr newValue = value;
			setter->execute(this, newValue);
			return true;
		}
		return false;
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
