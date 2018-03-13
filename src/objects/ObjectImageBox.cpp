/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "BaseImage.h"
#include "Dataset.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> ImageBox::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ImageBox::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ImageBox::_setters;

	ImageBox::ImageBox(chstr name) : Object(name)
	{
		this->image = NULL;
		this->debugColor = april::Color::Clear;
	}

	ImageBox::ImageBox(const ImageBox& other) : Object(other)
	{
		this->image = other.image;
		this->imageName = other.imageName;
		this->debugColor = april::Color::Clear;
	}

	ImageBox::~ImageBox()
	{
	}

	Object* ImageBox::createInstance(chstr name)
	{
		return new ImageBox(name);
	}

	hmap<hstr, PropertyDescription>& ImageBox::getPropertyDescriptions() const
	{
		if (ImageBox::_propertyDescriptions.size() == 0)
		{
			ImageBox::_propertyDescriptions = Object::getPropertyDescriptions();
			ImageBox::_propertyDescriptions["image"] = PropertyDescription("image", PropertyDescription::Type::String);
		}
		return ImageBox::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ImageBox::_getGetters() const
	{
		if (ImageBox::_getters.size() == 0)
		{
			ImageBox::_getters = Object::_getGetters();
			ImageBox::_getters["image"] = new PropertyDescription::Get<ImageBox, hstr>(&ImageBox::getImageName);
		}
		return ImageBox::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ImageBox::_getSetters() const
	{
		if (ImageBox::_setters.size() == 0)
		{
			ImageBox::_setters = Object::_getSetters();
			ImageBox::_setters["image"] = new PropertyDescription::TrySet<ImageBox, hstr>(&ImageBox::trySetImageByName);
		}
		return ImageBox::_setters;
	}

	void ImageBox::setImage(BaseImage* image)
	{
		this->image = image;
		if (image != NULL)
		{
			gvec2 size = image->getSrcSize();
			if (this->rect.w == 0.0f)
			{
				this->rect.w = size.x;
				this->pivot.x = size.x * 0.5f;
			}
			if (this->rect.h == 0.0f)
			{
				this->rect.h = size.y;
				this->pivot.y = size.y * 0.5f;
			}
			this->imageName = image->getFullName();
		}
		else
		{
			this->imageName = "";
		}
	}

	void ImageBox::setImageByName(chstr name)
	{
		this->setImage(name != "" ? this->dataset->getImage(name) : NULL);
	}

	bool ImageBox::trySetImageByName(chstr name)
	{
		if (this->imageName != name)
		{
			this->setImageByName(name);
			return true;
		}
		return false;
	}
	
	harray<BaseImage*> ImageBox::_getUsedImages() const
	{
		return (Object::_getUsedImages() + this->image);
	}
	
	void ImageBox::_draw()
	{
		if (this->image != NULL)
		{
			this->image->draw(this->_makeDrawRect(), this->_makeDrawColor());
		}
		Object::_draw();
	}
	
	void ImageBox::resizeToFitImage()
	{
		if (this->image != NULL)
		{
			this->setSize(this->image->getSrcSize());
			this->resetPivot();
		}
	}

}
