/// @file
/// @version 5.0
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
	static BaseImage* tempPushedImage = NULL;
	static BaseImage* tempHoverImage = NULL;
	static BaseImage* tempDisabledImage = NULL;

	hmap<hstr, PropertyDescription> ToggleButton::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ToggleButton::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ToggleButton::_setters;

	ToggleButton::ToggleButton(chstr name) :
		ImageButton(name)
	{
		this->toggled = false;
		this->toggledNormalImage = NULL;
		this->toggledPushedImage = NULL;
		this->toggledHoverImage = NULL;
		this->toggledDisabledImage = NULL;
	}

	ToggleButton::ToggleButton(const ToggleButton& other) :
		ImageButton(other)
	{
		this->toggled = other.toggled;
		this->toggledNormalImage = other.toggledNormalImage;
		this->toggledNormalImageName = other.toggledNormalImageName;
		this->toggledPushedImage = other.toggledPushedImage;
		this->toggledPushedImageName = other.toggledPushedImageName;
		this->toggledHoverImage = other.toggledHoverImage;
		this->toggledHoverImageName = other.toggledHoverImageName;
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

	hmap<hstr, PropertyDescription>& ToggleButton::getPropertyDescriptions() const
	{
		if (ToggleButton::_propertyDescriptions.size() == 0)
		{
			ToggleButton::_propertyDescriptions = ImageButton::getPropertyDescriptions();
			ToggleButton::_propertyDescriptions["toggled"] = PropertyDescription("toggled", PropertyDescription::Type::Bool);
			ToggleButton::_propertyDescriptions["toggled_image"] = PropertyDescription("toggled_image", PropertyDescription::Type::String);
			ToggleButton::_propertyDescriptions["toggled_pushed_image"] = PropertyDescription("toggled_pushed_image", PropertyDescription::Type::String);
			ToggleButton::_propertyDescriptions["toggled_hover_image"] = PropertyDescription("toggled_hover_image", PropertyDescription::Type::String);
			ToggleButton::_propertyDescriptions["toggled_disabled_image"] = PropertyDescription("toggled_disabled_image", PropertyDescription::Type::String);
		}
		return ToggleButton::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ToggleButton::_getGetters() const
	{
		if (ToggleButton::_getters.size() == 0)
		{
			ToggleButton::_getters = ImageButton::_getGetters();
			ToggleButton::_getters["toggled"] = new PropertyDescription::Get<ToggleButton, bool>(&ToggleButton::isToggled);
			ToggleButton::_getters["toggled_image"] = new PropertyDescription::Get<ToggleButton, hstr>(&ToggleButton::getToggledNormalImageName);
			ToggleButton::_getters["toggled_pushed_image"] = new PropertyDescription::Get<ToggleButton, hstr>(&ToggleButton::getToggledPushedImageName);
			ToggleButton::_getters["toggled_hover_image"] = new PropertyDescription::Get<ToggleButton, hstr>(&ToggleButton::getToggledHoverImageName);
			ToggleButton::_getters["toggled_disabled_image"] = new PropertyDescription::Get<ToggleButton, hstr>(&ToggleButton::getToggledDisabledImageName);
		}
		return ToggleButton::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ToggleButton::_getSetters() const
	{
		if (ToggleButton::_setters.size() == 0)
		{
			ToggleButton::_setters = ImageButton::_getSetters();
			ToggleButton::_setters["toggled"] = new PropertyDescription::Set<ToggleButton, bool>(&ToggleButton::setToggled);
			ToggleButton::_setters["toggled_image"] = new PropertyDescription::TrySet<ToggleButton, hstr>(&ToggleButton::trySetToggledNormalImageByName);
			ToggleButton::_setters["toggled_pushed_image"] = new PropertyDescription::TrySet<ToggleButton, hstr>(&ToggleButton::trySetToggledPushedImageByName);
			ToggleButton::_setters["toggled_hover_image"] = new PropertyDescription::TrySet<ToggleButton, hstr>(&ToggleButton::trySetToggledHoverImageByName);
			ToggleButton::_setters["toggled_disabled_image"] = new PropertyDescription::TrySet<ToggleButton, hstr>(&ToggleButton::trySetToggledDisabledImageByName);
		}
		return ToggleButton::_setters;
	}

	void ToggleButton::setToggledNormalImage(BaseImage* image)
	{
		this->toggledNormalImage = image;
		this->toggledNormalImageName = (image != NULL ? image->getFullName() : "");
	}

	void ToggleButton::setToggledPushedImage(BaseImage* image)
	{
		this->toggledPushedImage = image;
		this->toggledPushedImageName = (image != NULL ? image->getFullName() : "");
	}

	void ToggleButton::setToggledHoverImage(BaseImage* image)
	{
		this->toggledHoverImage = image;
		this->toggledHoverImageName = (image != NULL ? image->getFullName() : "");
	}

	void ToggleButton::setToggledDisabledImage(BaseImage* image)
	{
		this->toggledDisabledImage = image;
		this->toggledDisabledImageName = (image != NULL ? image->getFullName() : "");
	}

	void ToggleButton::setToggledNormalImageByName(chstr name)
	{
		this->setToggledNormalImage(name != "" ? this->dataset->getImage(name) : NULL);
	}

	void ToggleButton::setToggledPushedImageByName(chstr name)
	{
		this->setToggledPushedImage(name != "" ? this->dataset->getImage(name) : NULL);
	}

	void ToggleButton::setToggledHoverImageByName(chstr name)
	{
		this->setToggledHoverImage(name != "" ? this->dataset->getImage(name) : NULL);
	}

	void ToggleButton::setToggledDisabledImageByName(chstr name)
	{
		this->setToggledDisabledImage(name != "" ? this->dataset->getImage(name) : NULL);
	}

	bool ToggleButton::trySetToggledNormalImageByName(chstr name)
	{
		if (this->toggledHoverImageName != name)
		{
			this->setToggledNormalImageByName(name);
			return true;
		}
		return false;
	}
	
	bool ToggleButton::trySetToggledPushedImageByName(chstr name)
	{
		if (this->toggledPushedImageName != name)
		{
			this->setToggledPushedImageByName(name);
			return true;
		}
		return false;
	}
	
	bool ToggleButton::trySetToggledHoverImageByName(chstr name)
	{
		if (this->toggledHoverImageName != name)
		{
			this->setToggledHoverImageByName(name);
			return true;
		}
		return false;
	}

	bool ToggleButton::trySetToggledDisabledImageByName(chstr name)
	{
		if (this->toggledDisabledImageName != name)
		{
			this->setToggledDisabledImageByName(name);
			return true;
		}
		return false;
	}
	
	harray<BaseImage*> ToggleButton::_getUsedImages() const
	{
		harray<BaseImage*> images = ImageButton::_getUsedImages();
		images += this->toggledNormalImage;
		images += this->toggledPushedImage;
		images += this->toggledHoverImage;
		images += this->toggledDisabledImage;
		return images;
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
			tempPushedImage = this->pushedImage;
			tempHoverImage = this->hoverImage;
			tempDisabledImage = this->disabledImage;
			this->normalImage = this->toggledNormalImage;
			this->pushedImage = this->toggledPushedImage;
			this->hoverImage = this->toggledHoverImage;
			this->disabledImage = this->toggledDisabledImage;
			ImageButton::_draw();
			this->normalImage = tempNormalImage;
			this->pushedImage = tempPushedImage;
			this->hoverImage = tempHoverImage;
			this->disabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::_draw();
		}
	}

	void ToggleButton::_update(float timeDelta)
	{
		if (this->toggled)
		{
			tempNormalImage = this->normalImage;
			tempPushedImage = this->pushedImage;
			tempHoverImage = this->hoverImage;
			tempDisabledImage = this->disabledImage;
			this->normalImage = this->toggledNormalImage;
			this->pushedImage = this->toggledPushedImage;
			this->hoverImage = this->toggledHoverImage;
			this->disabledImage = this->toggledDisabledImage;
			ImageButton::_update(timeDelta);
			this->normalImage = tempNormalImage;
			this->pushedImage = tempPushedImage;
			this->hoverImage = tempHoverImage;
			this->disabledImage = tempDisabledImage;
		}
		else
		{
			ImageButton::_update(timeDelta);
		}
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
