/// @file
/// @author  Boris Mikic
/// @version 2.8
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "Image.h"
#include "ObjectToggleButton.h"

namespace aprilui
{
	// small optimization
	Image* tempNormalImage;
	Image* tempHoverImage;
	Image* tempPushedImage;
	Image* tempDisabledImage;

	ToggleButton::ToggleButton(chstr name, grect rect) :
		ImageButton(name, rect)
	{
		this->mToggled = false;
		this->mToggledNormalImage = NULL;
		this->mToggledHoverImage = NULL;
		this->mToggledPushedImage = NULL;
		this->mToggledDisabledImage = NULL;
	}

	ToggleButton::~ToggleButton()
	{
	}

	Object* ToggleButton::createInstance(chstr name, grect rect)
	{
		return new ToggleButton(name, rect);
	}

	void ToggleButton::setToggledNormalImageByName(chstr image)
	{
		this->setToggledNormalImage(this->mDataset->getImage(image));
	}

	void ToggleButton::setToggledHoverImageByName(chstr image)
	{
		this->setToggledHoverImage(this->mDataset->getImage(image));
	}

	void ToggleButton::setToggledPushedImageByName(chstr image)
	{
		this->setToggledPushedImage(this->mDataset->getImage(image));
	}

	void ToggleButton::setToggledDisabledImageByName(chstr image)
	{
		this->setToggledDisabledImage(this->mDataset->getImage(image));
	}

	void ToggleButton::OnDraw()
	{
		if (this->mToggled)
		{
			tempNormalImage = this->mNormalImage;
			tempHoverImage = this->mHoverImage;
			tempPushedImage = this->mPushedImage;
			tempDisabledImage = this->mDisabledImage;
			this->mNormalImage = this->mToggledNormalImage;
			this->mHoverImage = this->mToggledHoverImage;
			this->mPushedImage = this->mToggledPushedImage;
			this->mDisabledImage = this->mToggledDisabledImage;
			ImageButton::OnDraw();
			this->mNormalImage = tempNormalImage;
			this->mHoverImage = tempHoverImage;
			this->mPushedImage = tempPushedImage;
			this->mDisabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::OnDraw();
		}
	}

	void ToggleButton::update(float k)
	{
		if (this->mToggled)
		{
			tempNormalImage = this->mNormalImage;
			tempHoverImage = this->mHoverImage;
			tempPushedImage = this->mPushedImage;
			tempDisabledImage = this->mDisabledImage;
			this->mNormalImage = this->mToggledNormalImage;
			this->mHoverImage = this->mToggledHoverImage;
			this->mPushedImage = this->mToggledPushedImage;
			this->mDisabledImage = this->mToggledDisabledImage;
			ImageButton::update(k);
			this->mNormalImage = tempNormalImage;
			this->mHoverImage = tempHoverImage;
			this->mPushedImage = tempPushedImage;
			this->mDisabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::update(k);
		}
	}
	
	hstr ToggleButton::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "toggled_image")			return this->getToggledNormalImage()->getName();
		if (name == "toggled_hover_image")		return this->getToggledHoverImage()->getName();
		if (name == "toggled_pushed_image")		return this->getToggledPushedImage()->getName();
		if (name == "toggled_disabled_image")	return this->getToggledDisabledImage()->getName();
		return ImageButton::getProperty(name, propertyExists);
	}

	bool ToggleButton::setProperty(chstr name, chstr value)
	{
		if		(name == "toggled_image")			this->setToggledNormalImageByName(value);
		else if	(name == "toggled_hover_image")		this->setToggledHoverImageByName(value);
		else if	(name == "toggled_pushed_image")	this->setToggledPushedImageByName(value);
		else if	(name == "toggled_disabled_image")	this->setToggledDisabledImageByName(value);
		else return ImageButton::setProperty(name, value);
		return true;
	}
	
	bool ToggleButton::onMouseUp(april::Key keyCode)
	{
		bool result = ImageButton::onMouseUp(keyCode);
		if (result)
		{
			this->mToggled = !this->mToggled;
		}
		return result;
	}
	
}
