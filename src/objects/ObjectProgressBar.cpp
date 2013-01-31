/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
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
		this->mProgressImage = NULL;
		this->mProgressImageName = "";
		this->mProgress = 1.0f;
		this->mStretching = true;
		this->mDirection = Right;
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
		float progress = hclamp(this->mProgress, 0.0f, 1.0f);
		if (this->mProgressImage != NULL && progress > 0.0f)
		{
			april::Color color = this->_getDrawColor();
			color.a = (unsigned char)(color.a * this->_getDisabledAlphaFactor());
			grect srcRect = this->mProgressImage->getSrcRect();
			if (!this->mStretching)
			{
				this->mProgressImage->setSrcRect(this->_calcRectDirection(srcRect, progress));
			}
			this->mProgressImage->draw(this->_calcRectDirection(this->_getDrawRect(), progress), color);
			this->mProgressImage->setSrcRect(srcRect);
		}
	}

	grect ProgressBar::_calcRectDirection(grect rect, float progress)
	{
		static float size;
		switch (this->mDirection)
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
		this->mProgressImage = image;
		this->mProgressImageName = (image != NULL ? image->getFullName() : "null");
	}

	void ProgressBar::setProgressImageByName(chstr name)
	{
		this->setProgressImage(this->mDataset->getImage(name));
	}

	bool ProgressBar::trySetProgressImageByName(chstr name)
	{
		if (this->mProgressImageName != name)
		{
			this->setProgressImageByName(name);
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
		if (name == "progress")			return this->getProgress();
		if (name == "stretching")		return this->isStretching();
		if (name == "direction")
		{
			if (this->mDirection == Right)	return "right";
			if (this->mDirection == Left)	return "left";
			if (this->mDirection == Down)	return "down";
			if (this->mDirection == Up)		return "up";
		}
		return ImageBox::getProperty(name, propertyExists);
	}

	bool ProgressBar::setProperty(chstr name, chstr value)
	{
		if		(name == "progress_image")	this->setProgressImageByName(value);
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
