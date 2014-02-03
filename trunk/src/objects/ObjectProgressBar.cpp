/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBar.h"

namespace aprilui
{
	ProgressBar::ProgressBar(chstr name, grect rect) :
		ImageBox(name, rect)
	{
		this->progressImage = NULL;
		this->maskImage = NULL;
		this->progress = 1.0f;
		this->stretching = false;
		this->direction = Right;
	}

	ProgressBar::~ProgressBar()
	{
	}

	Object* ProgressBar::createInstance(chstr name, grect rect)
	{
		return new ProgressBar(name, rect);
	}

	void ProgressBar::OnDraw()
	{
		ImageBox::OnDraw();
		float progress = hclamp(this->progress, 0.0f, 1.0f);
		april::Color color = this->_getDrawColor();
		color.a = (unsigned char)(color.a * this->_getDisabledAlphaFactor());
		if (this->progressImage != NULL && progress > 0.0f)
		{
			grect srcRect = this->progressImage->getSrcRect();
			if (!this->stretching)
			{
				this->progressImage->setSrcRect(this->_calcRectDirection(srcRect, progress));
			}
			this->progressImage->draw(this->_calcRectDirection(this->_getDrawRect(), progress), color);
			this->progressImage->setSrcRect(srcRect);
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(this->_getDrawRect(), color);
		}
	}

	grect ProgressBar::_calcRectDirection(grect rect, float progress)
	{
		float size = 0.0f;
		switch (this->direction)
		{
		case Right:
			rect.w *= progress;
			break;
		case Left:
			size = rect.w * progress;
			rect.x += rect.w - size;
			rect.w = size;
			break;
		case Down:
			rect.h *= progress;
			break;
		case Up:
			size = rect.h * progress;
			rect.y += rect.h - size;
			rect.h = size;
		}
		return rect;
	}

	void ProgressBar::setProgressImage(Image* image)
	{
		this->progressImage = image;
		this->progressImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ProgressBar::setMaskImage(Image* image)
	{
		this->maskImage = image;
		this->maskImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ProgressBar::setProgressImageByName(chstr name)
	{
		this->setProgressImage(this->dataset->getImage(name));
	}

	void ProgressBar::setMaskImageByName(chstr name)
	{
		this->setMaskImage(this->dataset->getImage(name));
	}

	bool ProgressBar::trySetProgressImageByName(chstr name)
	{
		if (this->progressImageName != name)
		{
			// using c/p code because of performance reasons
			this->setProgressImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	bool ProgressBar::trySetMaskImageByName(chstr name)
	{
		if (this->maskImageName != name)
		{
			// using c/p code because of performance reasons
			this->setMaskImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	hstr ProgressBar::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "progress_image")	return this->getProgressImageName();
		if (name == "mask_image")		return this->getMaskImageName();
		if (name == "progress")			return this->getProgress();
		if (name == "stretching")		return this->isStretching();
		if (name == "direction")
		{
			if (this->direction == Right)	return "right";
			if (this->direction == Left)	return "left";
			if (this->direction == Down)	return "down";
			if (this->direction == Up)		return "up";
		}
		return ImageBox::getProperty(name, propertyExists);
	}

	bool ProgressBar::setProperty(chstr name, chstr value)
	{
		if		(name == "progress_image")	this->setProgressImageByName(value);
		else if (name == "mask_image")		this->setMaskImageByName(value);
		else if (name == "progress")		this->setProgress(value);
		else if (name == "stretching")		this->setStretching(value);
		else if (name == "direction")
		{
			if (value == "right")		this->setDirection(Right);
			else if (value == "left")	this->setDirection(Left);
			else if (value == "down")	this->setDirection(Down);
			else if (value == "up")		this->setDirection(Up);
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
