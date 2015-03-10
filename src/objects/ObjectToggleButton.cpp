/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "BaseImage.h"
#include "Dataset.h"
#include "Event.h"
#include "EventArgs.h"
#include "ObjectToggleButton.h"

namespace aprilui
{
	// small optimization
	static BaseImage* tempNormalImage = NULL;
	static BaseImage* tempHoverImage = NULL;
	static BaseImage* tempPushedImage = NULL;
	static BaseImage* tempDisabledImage = NULL;

	harray<PropertyDescription> ToggleButton::_propertyDescriptions;

	ToggleButton::ToggleButton(chstr name) : ImageButton(name)
	{
		this->toggled = false;
		this->toggledNormalImage = NULL;
		this->toggledHoverImage = NULL;
		this->toggledPushedImage = NULL;
		this->toggledDisabledImage = NULL;
	}

	ToggleButton::ToggleButton(const ToggleButton& other) : ImageButton(other)
	{
		this->toggled = other.toggled;
		this->toggledNormalImage = other.toggledNormalImage;
		this->toggledNormalImageName = other.toggledNormalImageName;
		this->toggledHoverImage = other.toggledHoverImage;
		this->toggledHoverImageName = other.toggledHoverImageName;
		this->toggledPushedImage = other.toggledPushedImage;
		this->toggledPushedImageName = other.toggledPushedImageName;
		this->toggledDisabledImage = other.toggledDisabledImage;
		this->toggledDisabledImageName = other.toggledDisabledImageName;
	}

	ToggleButton::~ToggleButton()
	{
	}

	Object* ToggleButton::createInstance(chstr name)
	{
		return new ToggleButton(name);
	}

	void ToggleButton::setToggledNormalImage(BaseImage* image)
	{
		this->toggledNormalImage = image;
		this->toggledNormalImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ToggleButton::setToggledHoverImage(BaseImage* image)
	{
		this->toggledHoverImage = image;
		this->toggledHoverImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ToggleButton::setToggledPushedImage(BaseImage* image)
	{
		this->toggledPushedImage = image;
		this->toggledPushedImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ToggleButton::setToggledDisabledImage(BaseImage* image)
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
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled", PropertyDescription::BOOL);
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled_image", PropertyDescription::STRING);
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled_hover_image", PropertyDescription::STRING);
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled_pushed_image", PropertyDescription::STRING);
			ToggleButton::_propertyDescriptions += PropertyDescription("toggled_disabled_image", PropertyDescription::STRING);
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
	
	harray<BaseImage*> ToggleButton::getUsedImages()
	{
		harray<BaseImage*> images = ImageButton::getUsedImages();
		if (this->toggledNormalImage != NULL)
		{
			images += this->toggledNormalImage;
		}
		if (this->toggledPushedImage != NULL)
		{
			images += this->toggledPushedImage;
		}
		if (this->toggledHoverImage != NULL)
		{
			images += this->toggledHoverImage;
		}
		if (this->toggledDisabledImage != NULL)
		{
			images += this->toggledDisabledImage;
		}
		return images.removedDuplicates();
	}

	void ToggleButton::toggle()
	{
		this->toggled ? this->turnOff() : this->turnOn();
	}

	void ToggleButton::turnOn()
	{
		this->toggled = true;
	}

	void ToggleButton::turnOff()
	{
		this->toggled = false;
	}
		
	void ToggleButton::_draw()
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
			ImageButton::_draw();
			this->normalImage = tempNormalImage;
			this->hoverImage = tempHoverImage;
			this->pushedImage = tempPushedImage;
			this->disabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::_draw();
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
	
	hstr ToggleButton::getProperty(chstr name)
	{
		if (name == "toggled")					return this->isToggled();
		if (name == "toggled_image")			return this->getToggledNormalImageName();
		if (name == "toggled_hover_image")		return this->getToggledHoverImageName();
		if (name == "toggled_pushed_image")		return this->getToggledPushedImageName();
		if (name == "toggled_disabled_image")	return this->getToggledDisabledImageName();
		return ImageButton::getProperty(name);
	}

	bool ToggleButton::setProperty(chstr name, chstr value)
	{
		if		(name == "toggled")					this->setToggled(value);
		else if (name == "toggled_image")			this->trySetToggledNormalImageByName(value);
		else if (name == "toggled_hover_image")		this->trySetToggledHoverImageByName(value);
		else if	(name == "toggled_pushed_image")	this->trySetToggledPushedImageByName(value);
		else if	(name == "toggled_disabled_image")	this->trySetToggledDisabledImageByName(value);
		else return ImageButton::setProperty(name, value);
		return true;
	}
	
	bool ToggleButton::_mouseUp(april::Key keyCode)
	{
		bool result = ImageButton::_mouseUp(keyCode);
		if (result)
		{
			this->toggle();
		}
		return (result || ImageButton::_mouseUp(keyCode));
	}
	
}
