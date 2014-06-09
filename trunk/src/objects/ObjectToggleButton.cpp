/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
	static Image* tempNormalImage = NULL;
	static Image* tempHoverImage = NULL;
	static Image* tempPushedImage = NULL;
	static Image* tempDisabledImage = NULL;

	harray<PropertyDescription> ToggleButton::_propertyDescriptions;

	ToggleButton::ToggleButton(chstr name, grect rect) : ImageButton(name, rect)
	{
		this->toggled = false;
		this->toggledNormalImage = NULL;
		this->toggledHoverImage = NULL;
		this->toggledPushedImage = NULL;
		this->toggledDisabledImage = NULL;
	}

	ToggleButton::~ToggleButton()
	{
	}

	Object* ToggleButton::createInstance(chstr name, grect rect)
	{
		return new ToggleButton(name, rect);
	}

	void ToggleButton::setToggledNormalImage(Image* image)
	{
		this->toggledNormalImage = image;
		this->toggledNormalImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ToggleButton::setToggledHoverImage(Image* image)
	{
		this->toggledHoverImage = image;
		this->toggledHoverImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ToggleButton::setToggledPushedImage(Image* image)
	{
		this->toggledPushedImage = image;
		this->toggledPushedImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ToggleButton::setToggledDisabledImage(Image* image)
	{
		this->toggledDisabledImage = image;
		this->toggledDisabledImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ToggleButton::setToggledNormalImageByName(chstr name)
	{
		this->setToggledNormalImage(this->dataset->getImage(name));
	}

	void ToggleButton::setToggledHoverImageByName(chstr name)
	{
		this->setToggledHoverImage(this->dataset->getImage(name));
	}

	void ToggleButton::setToggledPushedImageByName(chstr name)
	{
		this->setToggledPushedImage(this->dataset->getImage(name));
	}

	void ToggleButton::setToggledDisabledImageByName(chstr name)
	{
		this->setToggledDisabledImage(this->dataset->getImage(name));
	}

	harray<PropertyDescription> ToggleButton::getPropertyDescriptions()
	{
		if (ToggleButton::_propertyDescriptions.size() == 0)
		{
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled_image", PropertyDescription::TYPE_STRING);
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled_hover_image", PropertyDescription::TYPE_STRING);
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled_pushed_image", PropertyDescription::TYPE_STRING);
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled_disabled_image", PropertyDescription::TYPE_STRING);
		}
		return (ImageButton::getPropertyDescriptions() + ToggleButton::_propertyDescriptions);
	}

	bool ToggleButton::trySetToggledNormalImageByName(chstr name)
	{
		if (this->toggledHoverImageName != name)
		{
			// using c/p code because of performance reasons
			this->setToggledNormalImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	bool ToggleButton::trySetToggledHoverImageByName(chstr name)
	{
		if (this->toggledHoverImageName != name)
		{
			// using c/p code because of performance reasons
			this->setToggledHoverImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	bool ToggleButton::trySetToggledPushedImageByName(chstr name)
	{
		if (this->toggledPushedImageName != name)
		{
			// using c/p code because of performance reasons
			this->setToggledPushedImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	bool ToggleButton::trySetToggledDisabledImageByName(chstr name)
	{
		if (this->toggledDisabledImageName != name)
		{
			// using c/p code because of performance reasons
			this->setToggledDisabledImage(this->dataset->getImage(name));
			return true;
		}
		return false;
	}
	
	harray<Image*> ToggleButton::getUsedImages()
	{
		harray<Image*> images;
		if (this->image != NULL)
		{
			images += this->image;
		}
		if (this->pushedImage != NULL)
		{
			images += this->pushedImage;
		}
		if (this->hoverImage != NULL)
		{
			images += this->hoverImage;
		}
		if (this->disabledImage != NULL)
		{
			images += this->disabledImage;
		}
		return images;
	}
	void ToggleButton::toggle()
	{
		this->toggled = !this->toggled;
	}

	void ToggleButton::turnOn()
	{
		this->toggled = true;
	}

	void ToggleButton::turnOff()
	{
		this->toggled = false;
	}
		
	void ToggleButton::OnDraw()
	{
		if (this->toggled)
		{
			tempNormalImage = this->normalImage;
			tempHoverImage = this->hoverImage;
			tempPushedImage = this->pushedImage;
			tempDisabledImage = this->disabledImage;
			this->normalImage = this->toggledNormalImage;
			this->hoverImage = this->toggledHoverImage;
			this->pushedImage = this->toggledPushedImage;
			this->disabledImage = this->toggledDisabledImage;
			ImageButton::OnDraw();
			this->normalImage = tempNormalImage;
			this->hoverImage = tempHoverImage;
			this->pushedImage = tempPushedImage;
			this->disabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::OnDraw();
		}
	}

	void ToggleButton::update(float timeDelta)
	{
		if (this->toggled)
		{
			tempNormalImage = this->normalImage;
			tempHoverImage = this->hoverImage;
			tempPushedImage = this->pushedImage;
			tempDisabledImage = this->disabledImage;
			this->normalImage = this->toggledNormalImage;
			this->hoverImage = this->toggledHoverImage;
			this->pushedImage = this->toggledPushedImage;
			this->disabledImage = this->toggledDisabledImage;
			ImageButton::update(timeDelta);
			this->normalImage = tempNormalImage;
			this->hoverImage = tempHoverImage;
			this->pushedImage = tempPushedImage;
			this->disabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::update(timeDelta);
		}
	}
	
	hstr ToggleButton::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "toggled_image")			return this->getToggledNormalImageName();
		if (name == "toggled_hover_image")		return this->getToggledHoverImageName();
		if (name == "toggled_pushed_image")		return this->getToggledPushedImageName();
		if (name == "toggled_disabled_image")	return this->getToggledDisabledImageName();
		return ImageButton::getProperty(name, propertyExists);
	}

	bool ToggleButton::setProperty(chstr name, chstr value)
	{
		if		(name == "toggled_image")			this->trySetToggledNormalImageByName(value);
		else if	(name == "toggled_hover_image")		this->trySetToggledHoverImageByName(value);
		else if	(name == "toggled_pushed_image")	this->trySetToggledPushedImageByName(value);
		else if	(name == "toggled_disabled_image")	this->trySetToggledDisabledImageByName(value);
		else return ImageButton::setProperty(name, value);
		return true;
	}
	
	bool ToggleButton::onMouseUp(april::Key keyCode)
	{
		bool result = ImageButton::onMouseUp(keyCode);
		if (result)
		{
			this->toggled = !this->toggled;
		}
		return result;
	}
	
}
