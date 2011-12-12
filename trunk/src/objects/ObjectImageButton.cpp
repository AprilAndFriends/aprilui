/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
		mNormalImage = NULL;
		mPushedImage = NULL;
		mHoverImage = NULL;
		mDisabledImage = NULL;
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
		grect rect = _getDrawRect();
		bool enabled = _isDerivedEnabled();
		if (!enabled && mDisabledImage != NULL)
		{
			mDisabledImage->draw(rect, _getDrawColor());
			return;
		}
		bool cursorInside = isCursorInside();
		if (mPushed && mPushedImage == NULL && cursorInside)
		{
			mImage->draw(rect, april::Color(_getDrawColor() * 0.75f, getDerivedAlpha()));
			return;
		}
		ImageBox::OnDraw();
		if (enabled && mHovered && mHoverImage == NULL && cursorInside && aprilui::isHoverEffectEnabled()) // because "somebody" was too lazy to define hover images manually
		{
			april::BlendMode blendMode = mImage->getBlendMode();
			mImage->setBlendMode(april::ADD);
			mImage->draw(rect, april::Color(_getDrawColor(), getDerivedAlpha() / 4));
			mImage->setBlendMode(blendMode);
		}
	}

	void ImageButton::update(float k)
	{
		ButtonBase::update(k);
		mImage = mNormalImage;
		if (mImage == NULL)
		{
			mImage = mDataset->getImage("null");
		}
		if (!_isDerivedEnabled())
		{
			if (mDisabledImage != NULL)
			{
				mImage = mDisabledImage;
			}
		}
		else if (mHovered)
		{
			if (mPushed)
			{
				if (mPushedImage != NULL)
				{
					mImage = mPushedImage;
				}
			}
			else if (mHoverImage != NULL && aprilui::isHoverEffectEnabled())
			{
				mImage = mHoverImage;
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
		setPushedImage(mDataset->getImage(image));
	}

	void ImageButton::setHoverImageByName(chstr image)
	{
		setHoverImage(mDataset->getImage(image));
	}

	void ImageButton::setDisabledImageByName(chstr image)
	{
		setDisabledImage(mDataset->getImage(image));
	}

	void ImageButton::setImage(Image* image)
	{
		ImageBox::setImage(image);
		mNormalImage = image;
	}
	
	bool ImageButton::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button)) return true;
		bool result = ButtonBase::onMouseDown(x, y, button);
		if (result)
		{
            _triggerEvent("MouseDown", x, y, button);
		}
		return result;
	}

	bool ImageButton::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button)) return true;
		bool result = ButtonBase::onMouseUp(x, y, button);
		if (result)
		{
			_triggerEvent("Click", x, y, button);
		}
		return result;
	}
	
	void ImageButton::onMouseMove(float x, float y)
	{
		Object::onMouseMove(x, y);
		ButtonBase::onMouseMove(x, y);
	}
	
	hstr ImageButton::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "image")			return getImageName();
		if (name == "pushed_image")		return getPushedImage()->getName();
		if (name == "hover_image")		return getHoverImage()->getName();
		if (name == "disabled_image")	return getDisabledImage()->getName();
		return Object::getProperty(name, property_exists);
	}

	bool ImageButton::setProperty(chstr name, chstr value)
	{
		if      (name == "image")			setImageByName(value);
		else if (name == "pushed_image")	setPushedImageByName(value);
		else if (name == "hover_image")		setHoverImageByName(value);
		else if (name == "disabled_image")	setDisabledImageByName(value);
        else return Object::setProperty(name, value);
        return true;
	}
	
}
