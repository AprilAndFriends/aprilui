/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	ProgressBase::ProgressBase()
	{
		this->progressImage = NULL;
		this->antiProgressImage = NULL;
		this->maskImage = NULL;
		this->progress = 1.0f;
	}

	ProgressBase::~ProgressBase()
	{
	}

	void ProgressBase::setProgressImage(Image* image)
	{
		this->progressImage = image;
		this->progressImageName = (image != NULL ? image->getName() : "");
	}

	void ProgressBase::setAntiProgressImage(Image* image)
	{
		this->antiProgressImage = image;
		this->antiProgressImageName = (image != NULL ? image->getName() : "");
	}

	void ProgressBase::setMaskImage(Image* image)
	{
		this->maskImage = image;
		this->maskImageName = (image != NULL ? image->getName() : "");
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
		if		(name == "progress_image")		this->setProgressImageByName(value);
		else if (name == "anti_progress_image")	this->setAntiProgressImageByName(value);
		else if (name == "mask_image")			this->setMaskImageByName(value);
		else if (name == "progress")			this->setProgress(value);
		else return false;
		return true;
	}
	
}
