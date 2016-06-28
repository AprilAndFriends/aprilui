/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBar.h"
#include "Texture.h"

namespace aprilui
{
	ProgressBar::ProgressBar(chstr name, grect rect) : ImageBox(name, rect), ProgressBase()
	{
		this->stretching = false;
		this->direction = Right;
	}

	ProgressBar::~ProgressBar()
	{
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
	
	void ProgressBar::OnDraw(gvec2 offset)
	{
		ImageBox::OnDraw(offset);
		grect drawRect = this->_getDrawRect() + offset;
		float alpha = this->getDerivedAlpha();
		if (!isDerivedEnabled())
		{
			alpha /= 2;
		}
		april::Color drawColor = april::Color::White;
		drawColor.a = (unsigned char)(alpha * 255);
		grect directionRect;
		float progress = hclamp(this->progress, 0.0f, 1.0f);
		if (this->antiProgressImage != NULL && progress < 1.0f)
		{
			Direction antiDirection = (Direction)((int)DirectionMax - (int)this->direction);
			float antiProgress = 1.0f - progress;
			if (this->stretching)
			{
				this->antiProgressImage->draw(this->_calcRectDirection(drawRect, antiProgress, antiDirection), drawColor);
			}
			else
			{
				grect clipRect = this->progressImage->getClipRect();
				directionRect = this->_calcRectDirection(grect(0.0f, 0.0f, this->antiProgressImage->getSource().getSize()), antiProgress, antiDirection);
				this->antiProgressImage->setClipRect(directionRect);
				this->antiProgressImage->draw(drawRect, drawColor);
				this->antiProgressImage->setClipRect(clipRect);
			}
		}
		if (this->progressImage != NULL && progress > 0.0f)
		{
			if (this->stretching)
			{
				this->progressImage->draw(this->_calcRectDirection(drawRect, progress, this->direction), drawColor);
			}
			else
			{
				grect clipRect = this->progressImage->getClipRect();
				directionRect = this->_calcRectDirection(grect(0.0f, 0.0f, this->progressImage->getSource().getSize()), progress, this->direction);
				this->progressImage->setClipRect(directionRect);
				this->progressImage->draw(drawRect, drawColor);
				this->progressImage->setClipRect(clipRect);
			}
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(drawRect, drawColor);
		}
		april::rendersys->setBlendMode(april::BM_DEFAULT);
		april::rendersys->setColorMode(april::CM_DEFAULT);
	}

	grect ProgressBar::_calcRectDirection(grect rect, float progress, Direction direction)
	{
		float size = 0.0f;
		switch (direction)
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
			break;
		default:
			break;
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
		if		(name == "stretching")		this->setStretching(value);
		else if (name == "direction")
		{
			if (value == "right")			this->setDirection(Right);
			else if (value == "left")		this->setDirection(Left);
			else if (value == "down")		this->setDirection(Down);
			else if (value == "up")			this->setDirection(Up);
			else
			{
				aprilui::log("'direction=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (ProgressBase::setProperty(name, value)) { }
		else return ImageBox::setProperty(name, value);
		return true;
	}
	
}
