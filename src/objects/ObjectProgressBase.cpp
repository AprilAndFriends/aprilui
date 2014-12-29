/// @file
/// @version 4.0
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
		this->maskImage = NULL;
		this->progress = 1.0f;
	}

	ProgressBase::ProgressBase(const ProgressBase& other)
	{
		this->progressImage = other.progressImage;
		this->progressImageName = other.progressImageName;
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
		this->progressImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ProgressBase::setMaskImage(BaseImage* image)
	{
		this->maskImage = image;
		this->maskImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ProgressBase::setProgressImageByName(chstr name)
	{
		this->setProgressImage(this->getDataset()->getImage(name));
	}

	void ProgressBase::setMaskImageByName(chstr name)
	{
		this->setMaskImage(this->getDataset()->getImage(name));
	}

	harray<BaseImage*> ProgressBase::getUsedImages()
	{
		harray<BaseImage*> images;
		if (this->progressImage != NULL)
		{
			images += this->progressImage;
		}
		if (this->maskImage != NULL)
		{
			images += this->maskImage;
		}
		return images.removed_duplicates();
	}

	harray<PropertyDescription> ProgressBase::getPropertyDescriptions()
	{
		if (ProgressBase::_propertyDescriptions.size() == 0)
		{
			ProgressBase::_propertyDescriptions += PropertyDescription("progress_image", PropertyDescription::STRING);
			ProgressBase::_propertyDescriptions += PropertyDescription("mask_image", PropertyDescription::STRING);
			ProgressBase::_propertyDescriptions += PropertyDescription("progress", PropertyDescription::FLOAT);
		}
		return ProgressBase::_propertyDescriptions;
	}

	bool ProgressBase::trySetProgressImageByName(chstr name)
	{
		if (this->progressImageName != name)
		{
			// using c/p code because of performance reasons
			this->setProgressImage(this->getDataset()->getImage(name));
			return true;
		}
		return false;
	}
	
	bool ProgressBase::trySetMaskImageByName(chstr name)
	{
		if (this->maskImageName != name)
		{
			// using c/p code because of performance reasons
			this->setMaskImage(this->getDataset()->getImage(name));
			return true;
		}
		return false;
	}
	
	hstr ProgressBase::getProperty(chstr name)
	{
		if (name == "progress_image")	return this->getProgressImageName();
		if (name == "mask_image")		return this->getMaskImageName();
		if (name == "progress")			return this->getProgress();
		return "";
	}

	bool ProgressBase::setProperty(chstr name, chstr value)
	{
		if		(name == "progress_image")	this->trySetProgressImageByName(value);
		else if (name == "mask_image")		this->trySetMaskImageByName(value);
		else if (name == "progress")		this->setProgress(value);
		else return false;
		return true;
	}
	
}
