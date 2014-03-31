/// @file
/// @author  Boris Mikic
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressCircle.h"

namespace aprilui
{
	ProgressCircle::ProgressCircle(chstr name, grect rect) :
		ImageBox(name, rect)
	{
		this->progressImage = NULL;
		this->maskImage = NULL;
		this->progress = 1.0f;
		this->startAngle = 0.0f;
		this->direction = Clockwise;
	}

	ProgressCircle::~ProgressCircle()
	{
	}

	Object* ProgressCircle::createInstance(chstr name, grect rect)
	{
		return new ProgressCircle(name, rect);
	}

	void ProgressCircle::OnDraw()
	{
		ImageBox::OnDraw();
		float progress = hclamp(this->progress, 0.0f, 1.0f);
		april::Color color = this->_getDrawColor();
		color.a = (unsigned char)(color.a * this->_getDisabledAlphaFactor());
		if (this->progressImage != NULL && progress > 0.0f)
		{
			grect srcRect = this->progressImage->getSrcRect();
			this->progressImage->draw(this->_calcRectDirection(this->_getDrawRect(), progress), color);
			this->progressImage->setSrcRect(srcRect);
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(this->_getDrawRect(), color);
		}
	}

	grect ProgressCircle::_calcRectDirection(grect rect, float progress)
	{
		float size = 0.0f;
		switch (this->direction)
		{
		case Clockwise:
			rect.w *= progress;
			break;
		case Counterclockwise:
			size = rect.w * progress;
			rect.x += rect.w - size;
			rect.w = size;
			break;
		}
		return rect;
	}

	void ProgressCircle::setProgressImage(Image* image)
	{
		this->progressImage = image;
		this->progressImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ProgressCircle::setMaskImage(Image* image)
	{
		this->maskImage = image;
		this->maskImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ProgressCircle::setProgressImageByName(chstr name)
	{
		this->setProgressImage(this->dataset->getImage(name));
	}

	void ProgressCircle::setMaskImageByName(chstr name)
	{
		this->setMaskImage(this->dataset->getImage(name));
	}

	bool ProgressCircle::trySetProgressImageByName(chstr name)
	{
		if (this->progressImageName != name)
		{
			// using c/p code because of performance reasons
			this->setProgressImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	bool ProgressCircle::trySetMaskImageByName(chstr name)
	{
		if (this->maskImageName != name)
		{
			// using c/p code because of performance reasons
			this->setMaskImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	hstr ProgressCircle::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "progress_image")	return this->getProgressImageName();
		if (name == "mask_image")		return this->getMaskImageName();
		if (name == "progress")			return this->getProgress();
		if (name == "start_angle")		return this->getStartAngle();
		if (name == "direction")
		{
			if (this->direction == Clockwise)			return "clockwise";
			if (this->direction == Counterclockwise)	return "counterclockwise";
		}
		return ImageBox::getProperty(name, propertyExists);
	}

	bool ProgressCircle::setProperty(chstr name, chstr value)
	{
		if		(name == "progress_image")	this->setProgressImageByName(value);
		else if (name == "mask_image")		this->setMaskImageByName(value);
		else if (name == "progress")		this->setProgress(value);
		else if (name == "start_angle")		this->setStartAngle(value);
		else if (name == "direction")
		{
			if (value == "clockwise")				this->setDirection(Clockwise);
			else if (value == "counterclockwise")	this->setDirection(Counterclockwise);
			else
			{
				hlog::warn(aprilui::logTag, "'direction=' does not support value '" + value + "'.");
				return false;
			}
		}
		else return ImageBox::setProperty(name, value);
		return true;
	}
	
}
