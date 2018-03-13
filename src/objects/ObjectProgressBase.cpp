/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiUtil.h"
#include "BaseImage.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> ProgressBase::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ProgressBase::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ProgressBase::_setters;

	ProgressBase::ProgressBase()
	{
		this->progressImage = NULL;
		this->antiProgressImage = NULL;
		this->maskImage = NULL;
		this->progress = 1.0f;
		this->progressColor = april::Color::White;
		this->antiProgressColor = april::Color::White;
		this->maskColor = april::Color::White;
	}

	ProgressBase::ProgressBase(const ProgressBase& other)
	{
		this->progressImage = other.progressImage;
		this->progressImageName = other.progressImageName;
		this->antiProgressImage = other.antiProgressImage;
		this->antiProgressImageName = other.antiProgressImageName;
		this->maskImage = other.maskImage;
		this->maskImageName = other.maskImageName;
		this->progress = other.progress;
		this->progressColor = other.progressColor;
		this->antiProgressColor = other.antiProgressColor;
		this->maskColor = other.maskColor;
	}

	ProgressBase::~ProgressBase()
	{
	}

	hmap<hstr, PropertyDescription>& ProgressBase::getPropertyDescriptions() const
	{
		if (ProgressBase::_propertyDescriptions.size() == 0)
		{
			ProgressBase::_propertyDescriptions["progress_image"] = PropertyDescription("progress_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions["anti_progress_image"] = PropertyDescription("anti_progress_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions["mask_image"] = PropertyDescription("mask_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions["progress"] = PropertyDescription("progress", PropertyDescription::Type::Float);
			ProgressBase::_propertyDescriptions["progress_color"] = PropertyDescription("progress_color", PropertyDescription::Type::Color);
			ProgressBase::_propertyDescriptions["anti_progress_color"] = PropertyDescription("anti_progress_color", PropertyDescription::Type::Color);
			ProgressBase::_propertyDescriptions["mask_color"] = PropertyDescription("mask_color", PropertyDescription::Type::Color);
		}
		return ProgressBase::_propertyDescriptions;
	}

	void ProgressBase::setProgressImage(BaseImage* image)
	{
		this->progressImage = image;
		this->progressImageName = (image != NULL ? image->getFullName() : "");
	}

	void ProgressBase::setAntiProgressImage(BaseImage* image)
	{
		this->antiProgressImage = image;
		this->antiProgressImageName = (image != NULL ? image->getFullName() : "");
	}

	void ProgressBase::setMaskImage(BaseImage* image)
	{
		this->maskImage = image;
		this->maskImageName = (image != NULL ? image->getFullName() : "");
	}

	void ProgressBase::setProgressImageByName(chstr name)
	{
		this->setProgressImage(name != "" ? this->getDataset()->getImage(name) : NULL);
	}

	void ProgressBase::setAntiProgressImageByName(chstr name)
	{
		this->setAntiProgressImage(name != "" ? this->getDataset()->getImage(name) : NULL);
	}

	void ProgressBase::setMaskImageByName(chstr name)
	{
		this->setMaskImage(name != "" ? this->getDataset()->getImage(name) : NULL);
	}

	harray<BaseImage*> ProgressBase::_getUsedImages() const
	{
		harray<BaseImage*> images;
		images += this->progressImage;
		images += this->antiProgressImage;
		images += this->maskImage;
		return images;
	}

	bool ProgressBase::trySetProgressImageByName(chstr name)
	{
		if (this->progressImageName != name)
		{
			this->setProgressImageByName(name);
			return true;
		}
		return false;
	}
	
	bool ProgressBase::trySetAntiProgressImageByName(chstr name)
	{
		if (this->antiProgressImageName != name)
		{
			this->setAntiProgressImageByName(name);
			return true;
		}
		return false;
	}

	bool ProgressBase::trySetMaskImageByName(chstr name)
	{
		if (this->maskImageName != name)
		{
			this->setMaskImageByName(name);
			return true;
		}
		return false;
	}
	
	void ProgressBase::setProgressSymbolicColor(chstr value)
	{
		this->setProgressColor(aprilui::_makeColor(value));
	}
	
	void ProgressBase::setAntiProgressSymbolicColor(chstr value)
	{
		this->setAntiProgressColor(aprilui::_makeColor(value));
	}
	
	void ProgressBase::setMaskSymbolicColor(chstr value)
	{
		this->setMaskColor(aprilui::_makeColor(value));
	}
	
	april::Color ProgressBase::_makeDrawProgressColor() const
	{
		return this->_makeDrawColor(this->progressColor);	
	}
	
	april::Color ProgressBase::_makeDrawAntiProgressColor() const
	{
		return this->_makeDrawColor(this->antiProgressColor);
	}
	
	april::Color ProgressBase::_makeDrawMaskColor() const
	{
		return this->_makeDrawColor(this->maskColor);		
	}
	
}
