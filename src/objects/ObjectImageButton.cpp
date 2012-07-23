/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	ImageButton::ImageButton(chstr name, grect rect) :
		ButtonBase(),
		ImageBox(name, rect)
	{
		this->mNormalImage = NULL;
		this->mPushedImage = NULL;
		this->mHoverImage = NULL;
		this->mDisabledImage = NULL;
	}

	ImageButton::~ImageButton()
	{
	}

	Object* ImageButton::createInstance(chstr name, grect rect)
	{
		return new ImageButton(name, rect);
	}

	hstr ImageButton::getName()
	{
		return ImageBox::getName();
	}

	Object* ImageButton::getParent()
	{
		return ImageBox::getParent();
	}

	Dataset* ImageButton::getDataset()
	{
		return ImageBox::getDataset();
	}

	void ImageButton::OnDraw()
	{
		grect rect = this->_getDrawRect();
		bool enabled = this->_isDerivedEnabled();
		if (!enabled && this->mDisabledImage != NULL)
		{
			this->mDisabledImage->draw(rect, this->_getDrawColor());
			return;
		}
		bool cursorInside = this->isCursorInside();
		
		// this is a fallback feature if you haven't defined a pushed image. this solution works for most use cases
		// so why bother providing a pushed image when this can work. also it covers situations where people forget to set a pushed image
		if (this->mPushed && this->mPushedImage == NULL && cursorInside)
		{
			this->mImage->draw(rect, april::Color(this->_getDrawColor() * 0.75f, this->getDerivedAlpha()));
			return;
		}
		ImageBox::OnDraw();
		// the same thing for a hover image fallback solution
		if (enabled && this->mHovered && !this->mPushed && this->mHoverImage == NULL && cursorInside && aprilui::isHoverEffectEnabled())
		{
			april::BlendMode blendMode = this->mImage->getBlendMode();
			this->mImage->setBlendMode(april::ADD);
			this->mImage->draw(rect, april::Color(this->_getDrawColor(), this->getDerivedAlpha() / 4));
			this->mImage->setBlendMode(blendMode);
		}
	}

	void ImageButton::update(float k)
	{
		ButtonBase::update(k);
		this->mImage = this->mNormalImage;
		if (this->mImage == NULL)
		{
			this->mImage = this->mDataset->getImage("null");
		}
		if (!this->_isDerivedEnabled())
		{
			if (this->mDisabledImage != NULL)
			{
				this->mImage = this->mDisabledImage;
			}
		}
		else if (this->mHovered)
		{
			if (this->mPushed)
			{
				if (this->mPushedImage != NULL)
				{
					this->mImage = this->mPushedImage;
				}
			}
			else if (this->mHoverImage != NULL && aprilui::isHoverEffectEnabled())
			{
				this->mImage = this->mHoverImage;
			}
		}
		ImageBox::update(k);
	}
	
	bool ImageButton::isCursorInside()
	{
		return Object::isCursorInside();
	}

	void ImageButton::setPushedImageByName(chstr image)
	{
		this->setPushedImage(this->mDataset->getImage(image));
	}

	void ImageButton::setHoverImageByName(chstr image)
	{
		this->setHoverImage(this->mDataset->getImage(image));
	}

	void ImageButton::setDisabledImageByName(chstr image)
	{
		this->setDisabledImage(this->mDataset->getImage(image));
	}

	void ImageButton::setImage(Image* image)
	{
		ImageBox::setImage(image);
		this->mNormalImage = image;
	}
	
	hstr ImageButton::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "image")			return this->getImageName();
		if (name == "pushed_image")		return this->getPushedImage()->getName();
		if (name == "hover_image")		return this->getHoverImage()->getName();
		if (name == "disabled_image")	return this->getDisabledImage()->getName();
		return Object::getProperty(name, property_exists);
	}

	bool ImageButton::setProperty(chstr name, chstr value)
	{
		if		(name == "image")			this->setImageByName(value);
		else if	(name == "pushed_image")	this->setPushedImageByName(value);
		else if	(name == "hover_image")		this->setHoverImageByName(value);
		else if	(name == "disabled_image")	this->setDisabledImageByName(value);
		else return Object::setProperty(name, value);
		return true;
	}
	
	bool ImageButton::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		bool result = ButtonBase::onMouseDown(button);
		if (result)
		{
			this->triggerEvent("MouseDown", button);
		}
		return result;
	}

	bool ImageButton::onMouseUp(int button)
	{
		if (Object::onMouseUp(button))
		{
			return true;
		}
		bool result = ButtonBase::onMouseUp(button);
		if (result)
		{
			this->triggerEvent("Click", button);
		}
		return result;
	}
	
	void ImageButton::onMouseMove()
	{
		Object::onMouseMove();
		ButtonBase::onMouseMove();
	}

	void ImageButton::cancelMouseDown()
	{
		Object::cancelMouseDown();
		ButtonBase::cancelMouseDown();
	}
	
}
