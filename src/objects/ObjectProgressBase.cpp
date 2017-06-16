/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "BaseImage.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	harray<PropertyDescription> ProgressBase::_propertyDescriptions;

	ProgressBase::ProgressBase()
	{
		this->progressImage = NULL;
		this->antiProgressImage = NULL;
		this->maskImage = NULL;
		this->progress = 1.0f;
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
	}

	ProgressBase::~ProgressBase()
	{
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

	harray<PropertyDescription> ProgressBase::getPropertyDescriptions() const
	{
		if (ProgressBase::_propertyDescriptions.size() == 0)
		{
			ProgressBase::_propertyDescriptions += PropertyDescription("progress_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions += PropertyDescription("anti_progress_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions += PropertyDescription("mask_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions += PropertyDescription("progress", PropertyDescription::Type::Float);
		}
		return ProgressBase::_propertyDescriptions;
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
	
	hstr ProgressBase::getProperty(chstr name)
	{
		if (name == "progress_image")		return this->getProgressImageName();
		if (name == "anti_progress_image")	return this->getAntiProgressImageName();
		if (name == "mask_image")			return this->getMaskImageName();
		if (name == "progress")				return this->getProgress();
		return "";
	}

	bool ProgressBase::setProperty(chstr name, chstr value)
	{
		if		(name == "progress_image")		this->trySetProgressImageByName(value);
		else if (name == "anti_progress_image")	this->trySetAntiProgressImageByName(value);
		else if (name == "mask_image")			this->trySetMaskImageByName(value);
		else if (name == "progress")			this->setProgress(value);
		else return false;
		return true;
	}
	
}
