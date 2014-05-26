/// @file
/// @version 3.0
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
	ImageButton::ImageButton(chstr name, grect rect) : ImageBox(name, rect), ButtonBase()
	{
		this->normalImage = NULL;
		this->pushedImage = NULL;
		this->hoverImage = NULL;
		this->disabledImage = NULL;
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

	void ImageButton::OnDraw()
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
		ImageBox::OnDraw();
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
	
	bool ImageButton::isCursorInside()
	{
		return ImageBox::isCursorInside();
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
		this->setHoverImage(this->dataset->getImage(name));
	}

	void ImageButton::setPushedImageByName(chstr name)
	{
		this->setPushedImage(this->dataset->getImage(name));
	}

	void ImageButton::setDisabledImageByName(chstr name)
	{
		this->setDisabledImage(this->dataset->getImage(name));
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
	
	hstr ImageButton::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "pushed_image")		return this->getPushedImageName();
		if (name == "hover_image")		return this->getHoverImageName();
		if (name == "disabled_image")	return this->getDisabledImageName();
		bool exists = false;
		hstr result = ButtonBase::getProperty(name, &exists);
		if (!exists)
		{
			result = ImageBox::getProperty(name, &exists);
		}
		if (propertyExists != NULL)
		{
			*propertyExists = exists;
		}
		return result;
	}

	bool ImageButton::setProperty(chstr name, chstr value)
	{
		if		(name == "pushed_image")	this->setPushedImageByName(value);
		else if	(name == "hover_image")		this->setHoverImageByName(value);
		else if	(name == "disabled_image")	this->setDisabledImageByName(value);
		else if (ButtonBase::setProperty(name, value)) { }
		else return ImageBox::setProperty(name, value);
		return true;
	}
	
	bool ImageButton::onMouseDown(april::Key keyCode)
	{
		if (Object::onMouseDown(keyCode)) // not a mistake, ImageBox does handle a MouseDown even and this behavior has to be overriden (will be refactored)
		{
			return true;
		}
		bool result = ButtonBase::onMouseDown(keyCode);
		if (result)
		{
			this->triggerEvent("MouseDown", keyCode);
		}
		return result;
	}

	bool ImageButton::onMouseUp(april::Key keyCode)
	{
		if (Object::onMouseUp(keyCode)) // not a mistake, ImageBox does handle a MouseUp event and this behavior has to be overriden (will be refactored)
		{
			return true;
		}
		bool click = ButtonBase::onMouseUp(keyCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent("MouseUp", keyCode);
		}
		if (click)
		{
			this->triggerEvent("Click", keyCode);
		}
		return (click || up);
	}
	
	bool ImageButton::onMouseMove()
	{
		return (ImageBox::onMouseMove() || ButtonBase::onMouseMove());
	}

	bool ImageButton::onButtonDown(april::Button buttonCode)
	{
		if (Object::onButtonDown(buttonCode)) // not a mistake, ImageBox does handle a MouseDown event and this behavior has to be overriden (will be refactored)
		{
			return true;
		}
		bool result = ButtonBase::onButtonDown(buttonCode);
		if (result)
		{
			this->triggerEvent("ButtonDown", buttonCode);
		}
		return result;
	}

	bool ImageButton::onButtonUp(april::Button buttonCode)
	{
		if (Object::onButtonUp(buttonCode)) // not a mistake, ImageBox does handle a MouseUp even and this behavior has to be overriden (will be refactored)
		{
			return true;
		}
		bool click = ButtonBase::onButtonUp(buttonCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent("ButtonUp", buttonCode);
		}
		if (click)
		{
			this->triggerEvent("ButtonClick", buttonCode);
		}
		return (click || up);
	}
	
	void ImageButton::mouseCancel()
	{
		ImageBox::mouseCancel();
		ButtonBase::mouseCancel();
	}
	
}
