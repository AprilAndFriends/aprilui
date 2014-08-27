/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBar.h"

namespace aprilui
{
	harray<PropertyDescription> ProgressBar::_propertyDescriptions;

	ProgressBar::ProgressBar(chstr name, grect rect) : ImageBox(name, rect), ProgressBase()
	{
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

	Dataset* ProgressBar::getDataset()
	{
		return ImageBox::getDataset();
	}

	Image* ProgressBar::getImage()
	{
		return ImageBox::getImage();
	}

	void ProgressBar::setImage(Image* image)
	{
		ImageBox::setImage(image);
	}

	hstr ProgressBar::getImageName()
	{
		return ImageBox::getImageName();
	}

	void ProgressBar::setImageByName(chstr name)
	{
		ImageBox::setImageByName(name);
	}

	harray<PropertyDescription> ProgressBar::getPropertyDescriptions()
	{
		if (ProgressBar::_propertyDescriptions.size() == 0)
		{
			ProgressBar::_propertyDescriptions += PropertyDescription("stretching", PropertyDescription::BOOL);
			ProgressBar::_propertyDescriptions += PropertyDescription("direction", PropertyDescription::ENUM);
		}
		return (ImageBox::getPropertyDescriptions() + ProgressBase::getPropertyDescriptions() + ProgressBar::_propertyDescriptions);
	}

	bool ProgressBar::trySetImageByName(chstr name)
	{
		return ImageBox::trySetImageByName(name);
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

	hstr ProgressBar::getProperty(chstr name)
	{
		if (name == "stretching")	return this->isStretching();
		if (name == "direction")
		{
			if (this->direction == Right)	return "right";
			if (this->direction == Left)	return "left";
			if (this->direction == Down)	return "down";
			if (this->direction == Up)		return "up";
		}
		hstr result = ProgressBase::getProperty(name);
		if (result == "")
		{
			result = ImageBox::getProperty(name);
		}
		return result;
	}

	bool ProgressBar::setProperty(chstr name, chstr value)
	{
		if		(name == "stretching")	this->setStretching(value);
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
		else if (ProgressBase::setProperty(name, value)) { }
		else return ImageBox::setProperty(name, value);
		return true;
	}
	
}
