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
		mToggled = false;
		mToggledNormalImage = NULL;
		mToggledHoverImage = NULL;
		mToggledPushedImage = NULL;
		mToggledDisabledImage = NULL;
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
		setToggledNormalImage(mDataset->getImage(image));
	}

	void ToggleButton::setToggledHoverImageByName(chstr image)
	{
		setToggledHoverImage(mDataset->getImage(image));
	}

	void ToggleButton::setToggledPushedImageByName(chstr image)
	{
		setToggledPushedImage(mDataset->getImage(image));
	}

	void ToggleButton::setToggledDisabledImageByName(chstr image)
	{
		setToggledDisabledImage(mDataset->getImage(image));
	}

	hstr ToggleButton::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "toggled_image")			return getToggledNormalImage()->getName();
		if (name == "toggled_hover_image")		return getToggledHoverImage()->getName();
		if (name == "toggled_pushed_image")		return getToggledPushedImage()->getName();
		if (name == "toggled_disabled_image")	return getToggledDisabledImage()->getName();
		return ImageButton::getProperty(name, property_exists);
	}

	bool ToggleButton::setProperty(chstr name, chstr value)
	{
		if		(name == "toggled_image")			setToggledNormalImageByName(value);
		else if (name == "toggled_hover_image")		setToggledHoverImageByName(value);
		else if (name == "toggled_pushed_image")	setToggledPushedImageByName(value);
		else if (name == "toggled_disabled_image")	setToggledDisabledImageByName(value);
        else return ImageButton::setProperty(name, value);
        return true;
	}
	
	void ToggleButton::OnDraw()
	{
		if (mToggled)
		{
			tempNormalImage = mNormalImage;
			tempHoverImage = mHoverImage;
			tempPushedImage = mPushedImage;
			tempDisabledImage = mDisabledImage;
			mNormalImage = mToggledNormalImage;
			mHoverImage = mToggledHoverImage;
			mPushedImage = mToggledPushedImage;
			mDisabledImage = mToggledDisabledImage;
			ImageButton::OnDraw();
			mNormalImage = tempNormalImage;
			mHoverImage = tempHoverImage;
			mPushedImage = tempPushedImage;
			mDisabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::OnDraw();
		}
	}

	void ToggleButton::update(float k)
	{
		if (mToggled)
		{
			tempNormalImage = mNormalImage;
			tempHoverImage = mHoverImage;
			tempPushedImage = mPushedImage;
			tempDisabledImage = mDisabledImage;
			mNormalImage = mToggledNormalImage;
			mHoverImage = mToggledHoverImage;
			mPushedImage = mToggledPushedImage;
			mDisabledImage = mToggledDisabledImage;
			ImageButton::update(k);
			mNormalImage = tempNormalImage;
			mHoverImage = tempHoverImage;
			mPushedImage = tempPushedImage;
			mDisabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::update(k);
		}
	}
	
	bool ToggleButton::onMouseUp(float x, float y, int button)
	{
		bool result = ImageButton::onMouseUp(x, y, button);
		if (result)
		{
			mToggled = !mToggled;
		}
		return result;
	}
	
}
