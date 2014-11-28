/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	harray<PropertyDescription> ImageButton::_propertyDescriptions;

	ImageButton::ImageButton(chstr name) : ImageBox(name), ButtonBase()
	{
		this->normalImage = NULL;
		this->pushedImage = NULL;
		this->hoverImage = NULL;
		this->disabledImage = NULL;
	}

	ImageButton::ImageButton(const ImageButton& other) : ImageBox(other), ButtonBase(other)
	{
		this->normalImage = other.normalImage;
		this->pushedImage = other.pushedImage;
		this->hoverImage = other.hoverImage;
		this->disabledImage = other.disabledImage;
		this->normalImageName = other.normalImageName;
		this->hoverImageName = other.hoverImageName;
		this->pushedImageName = other.pushedImageName;
		this->disabledImageName = other.disabledImageName;
	}

	ImageButton::~ImageButton()
	{
	}

	Object* ImageButton::createInstance(chstr name)
	{
		return new ImageButton(name);
	}

	hstr ImageButton::getName()
	{
		return ImageBox::getName();
	}

	int ImageButton::getFocusIndex()
	{
		return ImageBox::getFocusIndex();
	}

	Object* ImageButton::getParent()
	{
		return ImageBox::getParent();
	}

	Dataset* ImageButton::getDataset()
	{
		return ImageBox::getDataset();
	}

	bool ImageButton::isCursorInside()
	{
		return ImageBox::isCursorInside();
	}

	harray<PropertyDescription> ImageButton::getPropertyDescriptions()
	{
		if (ImageButton::_propertyDescriptions.size() == 0)
		{
			ImageButton::_propertyDescriptions += PropertyDescription("pushed_image", PropertyDescription::STRING);
			ImageButton::_propertyDescriptions += PropertyDescription("hover_image", PropertyDescription::STRING);
			ImageButton::_propertyDescriptions += PropertyDescription("disabled_image", PropertyDescription::STRING);
		}
		return (ImageBox::getPropertyDescriptions() + ImageButton::_propertyDescriptions);
	}

	void ImageButton::_draw()
	{
		grect rect = this->_getDrawRect();
		bool enabled = this->isDerivedEnabled();
		if (!enabled && this->disabledImage != NULL)
		{
			this->disabledImage->draw(rect, this->_getDrawColor());
			return;
		}
		// this is a fallback feature if you haven't defined a pushed image. this solution works for most use cases
		// so why bother providing a pushed image when this can work. also it covers situations where people forget to set a pushed image
		if (this->pushed && this->pushedImage == NULL && this->isCursorInside())
		{
			this->image->draw(rect, april::Color(this->_getDrawColor() * 0.75f, this->getDerivedAlpha()));
			return;
		}
		ImageBox::_draw();
		// the same thing for a hover image fallback solution
		if (enabled && this->hovered && !this->pushed && this->hoverImage == NULL && aprilui::isHoverEffectEnabled())
		{
			april::BlendMode blendMode = this->image->getBlendMode();
			this->image->setBlendMode(april::BM_ADD);
			this->image->draw(rect, april::Color(this->_getDrawColor(), this->getDerivedAlpha() / 4));
			this->image->setBlendMode(blendMode);
		}
	}

	void ImageButton::update(float timeDelta)
	{
		ButtonBase::update(timeDelta);
		this->image = this->normalImage;
		if (this->image == NULL)
		{
			this->image = this->dataset->getImage(APRILUI_IMAGE_NAME_NULL);
		}
		if (!this->isDerivedEnabled())
		{
			if (this->disabledImage != NULL)
			{
				this->image = this->disabledImage;
			}
		}
		else if (this->hovered)
		{
			if (this->pushed)
			{
				if (this->pushedImage != NULL)
				{
					this->image = this->pushedImage;
				}
			}
			else if (this->hoverImage != NULL && aprilui::isHoverEffectEnabled())
			{
				this->image = this->hoverImage;
			}
		}
		ImageBox::update(timeDelta);
	}
	
	void ImageButton::setPushedImage(Image* image)
	{
		this->pushedImage = image;
		this->pushedImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ImageButton::setHoverImage(Image* image)
	{
		this->hoverImage = image;
		this->hoverImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ImageButton::setDisabledImage(Image* image)
	{
		this->disabledImage = image;
		this->disabledImageName = (image != NULL ? image->getFullName() : APRILUI_IMAGE_NAME_NULL);
	}

	void ImageButton::setHoverImageByName(chstr name)
	{
		if (name == "")
		{
			this->hoverImage = NULL;
		}
		else
		{
			this->setHoverImage(this->dataset->getImage(name));
		}
	}

	void ImageButton::setPushedImageByName(chstr name)
	{
		if (name == "")
		{
			this->pushedImage = NULL;
		}
		else
		{
			this->setPushedImage(this->dataset->getImage(name));
		}
	}

	void ImageButton::setDisabledImageByName(chstr name)
	{
		if (name == "")
		{
			this->disabledImage = NULL;
		}
		else
		{
			this->setDisabledImage(this->dataset->getImage(name));
		}
	}

	bool ImageButton::trySetPushedImageByName(chstr name)
	{
		if (this->pushedImageName != name)
		{
			this->setPushedImageByName(name);
			return true;
		}
		return false;
	}
	
	bool ImageButton::trySetHoverImageByName(chstr name)
	{
		if (this->hoverImageName != name)
		{
			this->setHoverImageByName(name);
			return true;
		}
		return false;
	}
	
	bool ImageButton::trySetDisabledImageByName(chstr name)
	{
		if (this->disabledImageName != name)
		{
			this->setDisabledImageByName(name);
			return true;
		}
		return false;
	}
	
	void ImageButton::setImage(Image* image)
	{
		ImageBox::setImage(image);
		this->normalImage = this->image;
		this->normalImageName = this->imageName;
	}
	
	harray<Image*> ImageButton::getUsedImages()
	{
		harray<Image*> images;
		if (this->normalImage != NULL)
		{
			images += this->normalImage;
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
	
	bool ImageButton::triggerEvent(chstr type, april::Key keyCode)
	{
		return ImageBox::triggerEvent(type, keyCode);
	}

	bool ImageButton::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return ImageBox::triggerEvent(type, keyCode, string);
	}

	bool ImageButton::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		return ImageBox::triggerEvent(type, keyCode, position, string, userData);
	}

	bool ImageButton::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return ImageBox::triggerEvent(type, buttonCode, string, userData);
	}

	bool ImageButton::triggerEvent(chstr type, chstr string, void* userData)
	{
		return ImageBox::triggerEvent(type, string, userData);
	}

	bool ImageButton::triggerEvent(chstr type, void* userData)
	{
		return ImageBox::triggerEvent(type, userData);
	}

	hstr ImageButton::getProperty(chstr name)
	{
		if (name == "pushed_image")		return this->getPushedImageName();
		if (name == "hover_image")		return this->getHoverImageName();
		if (name == "disabled_image")	return this->getDisabledImageName();
		hstr result = ButtonBase::getProperty(name);
		if (result == "")
		{
			result = ImageBox::getProperty(name);
		}
		return result;
	}

	bool ImageButton::setProperty(chstr name, chstr value)
	{
		if		(name == "pushed_image")	this->trySetPushedImageByName(value);
		else if	(name == "hover_image")		this->trySetHoverImageByName(value);
		else if	(name == "disabled_image")	this->trySetDisabledImageByName(value);
		else if (ButtonBase::setProperty(name, value)) { }
		else return ImageBox::setProperty(name, value);
		return true;
	}
	
	bool ImageButton::_mouseDown(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseDown(keyCode);
		if (result)
		{
			this->triggerEvent(Event::MouseDown, keyCode);
		}
		return (result || ImageBox::_mouseDown(keyCode));
	}

	bool ImageButton::_mouseUp(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseUp(keyCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::MouseUp, keyCode);
		}
		if (result)
		{
			this->triggerEvent(Event::Click, keyCode);
		}
		return (result || up || ImageBox::_mouseUp(keyCode));
	}
	
	bool ImageButton::_mouseMove()
	{
		return (ButtonBase::_mouseMove() || ImageBox::_mouseMove());
	}

	bool ImageButton::_buttonDown(april::Button buttonCode)
	{
		bool result = ButtonBase::_buttonDown(buttonCode);
		if (result)
		{
			this->triggerEvent(Event::ButtonDown, buttonCode);
		}
		return (result || ImageBox::_buttonDown(buttonCode));
	}

	bool ImageButton::_buttonUp(april::Button buttonCode)
	{
		if (ImageBox::onButtonUp(buttonCode))
		{
			return true;
		}
		bool result = ButtonBase::_buttonUp(buttonCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::ButtonUp, buttonCode);
		}
		if (result)
		{
			this->triggerEvent(Event::ButtonTrigger, buttonCode);
		}
		return (result || up || ImageBox::_buttonUp(buttonCode));
	}
	
	void ImageButton::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		ImageBox::_mouseCancel(keyCode);
	}
	
}
