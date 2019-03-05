/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "BaseImage.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> ImageButton::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ImageButton::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ImageButton::_setters;

	ImageButton::ImageButton(chstr name) :
		ImageBox(name),
		ButtonBase()
	{
		this->normalImage = NULL;
		this->pushedImage = NULL;
		this->hoverImage = NULL;
		this->disabledImage = NULL;
		this->hoverAddFactor = 0.25f;
		this->pushedMultiplyFactor = 0.75f;
		this->hoverAddFadeSpeed = 0.0f;
		this->pushedMultiplyFadeSpeed = 0.0f;
		this->_hoverAddTime = 0.0f;
		this->_pushedMultiplyTime = 0.0f;
	}

	ImageButton::ImageButton(const ImageButton& other) :
		ImageBox(other),
		ButtonBase(other)
	{
		this->normalImage = other.normalImage;
		this->pushedImage = other.pushedImage;
		this->hoverImage = other.hoverImage;
		this->disabledImage = other.disabledImage;
		this->normalImageName = other.normalImageName;
		this->hoverImageName = other.hoverImageName;
		this->pushedImageName = other.pushedImageName;
		this->disabledImageName = other.disabledImageName;
		this->hoverAddFactor = other.hoverAddFactor;
		this->pushedMultiplyFactor = other.pushedMultiplyFactor;
		this->hoverAddFadeSpeed = other.hoverAddFadeSpeed;
		this->pushedMultiplyFadeSpeed = other.pushedMultiplyFadeSpeed;
		this->_hoverAddTime = other._hoverAddTime;
		this->_pushedMultiplyTime = other._pushedMultiplyTime;
	}

	Object* ImageButton::createInstance(chstr name)
	{
		return new ImageButton(name);
	}

	hmap<hstr, PropertyDescription>& ImageButton::getPropertyDescriptions() const
	{
		if (ImageButton::_propertyDescriptions.size() == 0)
		{
			ImageButton::_propertyDescriptions = ImageBox::getPropertyDescriptions() + ButtonBase::getPropertyDescriptions();
			ImageButton::_propertyDescriptions["pushed_image"] = PropertyDescription("pushed_image", PropertyDescription::Type::String);
			ImageButton::_propertyDescriptions["hover_image"] = PropertyDescription("hover_image", PropertyDescription::Type::String);
			ImageButton::_propertyDescriptions["disabled_image"] = PropertyDescription("disabled_image", PropertyDescription::Type::String);
			ImageButton::_propertyDescriptions["hover_add_factor"] = PropertyDescription("hover_add_factor", PropertyDescription::Type::Float);
			ImageButton::_propertyDescriptions["pushed_multiply_factor"] = PropertyDescription("pushed_multiply_factor", PropertyDescription::Type::Float);
			ImageButton::_propertyDescriptions["hover_add_fade_speed"] = PropertyDescription("hover_add_fade_speed", PropertyDescription::Type::Float);
			ImageButton::_propertyDescriptions["pushed_multiply_fade_speed"] = PropertyDescription("pushed_multiply_fade_speed", PropertyDescription::Type::Float);
		}
		return ImageButton::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ImageButton::_getGetters() const
	{
		if (ImageButton::_getters.size() == 0)
		{
			ImageButton::_getters = ImageBox::_getGetters() + ButtonBase::_generateGetters<ImageButton>();
			ImageButton::_getters["pushed_image"] = new PropertyDescription::Get<ImageButton, hstr>(&ImageButton::getPushedImageName);
			ImageButton::_getters["hover_image"] = new PropertyDescription::Get<ImageButton, hstr>(&ImageButton::getHoverImageName);
			ImageButton::_getters["disabled_image"] = new PropertyDescription::Get<ImageButton, hstr>(&ImageButton::getDisabledImageName);
			ImageButton::_getters["hover_add_factor"] = new PropertyDescription::Get<ImageButton, float>(&ImageButton::getHoverAddFactor);
			ImageButton::_getters["pushed_multiply_factor"] = new PropertyDescription::Get<ImageButton, float>(&ImageButton::getPushedMultiplyFactor);
			ImageButton::_getters["hover_add_fade_speed"] = new PropertyDescription::Get<ImageButton, float>(&ImageButton::getHoverAddFadeSpeed);
			ImageButton::_getters["pushed_multiply_fade_speed"] = new PropertyDescription::Get<ImageButton, float>(&ImageButton::getPushedMultiplyFadeSpeed);
		}
		return ImageButton::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ImageButton::_getSetters() const
	{
		if (ImageButton::_setters.size() == 0)
		{
			ImageButton::_setters = ImageBox::_getSetters() + ButtonBase::_generateSetters<ImageButton>();
			ImageButton::_setters["pushed_image"] = new PropertyDescription::TrySet<ImageButton, hstr>(&ImageButton::trySetPushedImageByName);
			ImageButton::_setters["hover_image"] = new PropertyDescription::TrySet<ImageButton, hstr>(&ImageButton::trySetHoverImageByName);
			ImageButton::_setters["disabled_image"] = new PropertyDescription::TrySet<ImageButton, hstr>(&ImageButton::trySetDisabledImageByName);
			ImageButton::_setters["hover_add_factor"] = new PropertyDescription::Set<ImageButton, float>(&ImageButton::setHoverAddFactor);
			ImageButton::_setters["pushed_multiply_factor"] = new PropertyDescription::Set<ImageButton, float>(&ImageButton::setPushedMultiplyFactor);
			ImageButton::_setters["hover_add_fade_speed"] = new PropertyDescription::Set<ImageButton, float>(&ImageButton::setHoverAddFadeSpeed);
			ImageButton::_setters["pushed_multiply_fade_speed"] = new PropertyDescription::Set<ImageButton, float>(&ImageButton::setPushedMultiplyFadeSpeed);
		}
		return ImageButton::_setters;
	}

	hstr ImageButton::getName() const
	{
		return ImageBox::getName();
	}

	int ImageButton::getFocusIndex() const
	{
		return ImageBox::getFocusIndex();
	}

	Object* ImageButton::getParent() const
	{
		return ImageBox::getParent();
	}

	Dataset* ImageButton::getDataset() const
	{
		return ImageBox::getDataset();
	}

	bool ImageButton::isCursorInside() const
	{
		return ImageBox::isCursorInside();
	}

	void ImageButton::setHoverAddFactor(const float& value)
	{
		this->hoverAddFactor = hclamp(value, 0.0f, 1.0f);
	}
	
	void ImageButton::setPushedMultiplyFactor(const float& value)
	{
		this->pushedMultiplyFactor = hclamp(value, 0.0f, 1.0f);
	}

	void ImageButton::setHoverAddFadeSpeed(const float& value)
	{
		this->hoverAddFadeSpeed = value;
		if (this->hoverAddFadeSpeed <= 0.0f)
		{
			this->_hoverAddTime = 0.0f;
		}
	}

	void ImageButton::setPushedMultiplyFadeSpeed(const float& value)
	{
		this->pushedMultiplyFadeSpeed = value;
		if (this->pushedMultiplyFadeSpeed <= 0.0f)
		{
			this->_pushedMultiplyTime = 0.0f;
		}
	}

	void ImageButton::setPushedImage(BaseImage* image)
	{
		this->pushedImage = image;
		this->pushedImageName = (image != NULL ? image->getFullName() : "");
	}

	void ImageButton::setHoverImage(BaseImage* image)
	{
		this->hoverImage = image;
		this->hoverImageName = (image != NULL ? image->getFullName() : "");
	}

	void ImageButton::setDisabledImage(BaseImage* image)
	{
		this->disabledImage = image;
		this->disabledImageName = (image != NULL ? image->getFullName() : "");
	}

	void ImageButton::setHoverImageByName(chstr name)
	{
		this->setHoverImage(name != "" ? this->dataset->getImage(name) : NULL);
	}

	void ImageButton::setPushedImageByName(chstr name)
	{
		this->setPushedImage(name != "" ? this->dataset->getImage(name) : NULL);
	}

	void ImageButton::setDisabledImageByName(chstr name)
	{
		this->setDisabledImage(name != "" ? this->dataset->getImage(name) : NULL);
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

	void ImageButton::setImage(BaseImage* image)
	{
		ImageBox::setImage(image);
		this->normalImage = this->image;
		this->normalImageName = this->imageName;
	}

	harray<BaseImage*> ImageButton::_getUsedImages() const
	{
		harray<BaseImage*> images = ImageBox::_getUsedImages();
		images += this->normalImage;
		images += this->pushedImage;
		images += this->hoverImage;
		images += this->disabledImage;
		return images;
	}

	bool ImageButton::isPointInside(cgvec2f position) const
	{
		return ImageBox::isPointInside(position);
	}

	void ImageButton::_draw()
	{
		grectf drawRect = this->_makeDrawRect();
		bool enabled = this->isDerivedEnabled();
		if (!enabled && this->disabledImage != NULL)
		{
			bool useDisabledAlpha = this->useDisabledAlpha;
			this->useDisabledAlpha = false;
			this->disabledImage->draw(drawRect, this->_makeDrawColor());
			this->useDisabledAlpha = useDisabledAlpha;
			return;
		}
		bool pushedFade = (this->_pushedMultiplyTime > 0.0f && this->_pushedMultiplyTime > 0.0f);
		bool hoverFade = (this->hoverAddFadeSpeed > 0.0f && this->_hoverAddTime > 0.0f);
		// this is a fallback feature if you haven't defined a pushed image. this solution works for most use cases
		// so why bother providing a pushed image when this can work. also it covers situations where people forget to set a pushed image
		if (this->pushedImage == NULL && this->image != NULL && (this->pushed && this->isCursorInside() || pushedFade))
		{
			april::Color color = this->color;
			float factor = 1.0f - (1.0f - this->pushedMultiplyFactor) * (this->_pushedMultiplyTime > 0.0f ? this->_pushedMultiplyTime : 1.0f);
			this->color.r = (unsigned char)(this->color.r * factor);
			this->color.g = (unsigned char)(this->color.g * factor);
			this->color.b = (unsigned char)(this->color.b * factor);
			ImageBox::_draw();
			this->color = color;
			if (!pushedFade)
			{
				return;
			}
		}
		else
		{
			ImageBox::_draw();
		}
		// the same thing for a hover image fallback solution
		if (this->hoverImage == NULL && this->image != NULL && enabled && aprilui::isHoverEffectEnabled() && (!this->pushed && this->hovered || hoverFade))
		{
			Image* blendableImage = dynamic_cast<Image*>(this->image);
			if (blendableImage != NULL)
			{
				april::Color drawColor = this->_makeDrawColor();
				drawColor.a = (unsigned char)(drawColor.a * this->hoverAddFactor * (this->hoverAddFadeSpeed > 0.0f ? this->_hoverAddTime : 1.0f));
				april::BlendMode blendMode = blendableImage->getBlendMode();
				blendableImage->setBlendMode(april::BlendMode::Add);
				blendableImage->draw(drawRect, drawColor);
				blendableImage->setBlendMode(blendMode);
			}
		}
	}

	void ImageButton::_update(float timeDelta)
	{
		ButtonBase::_update(timeDelta);
		this->image = this->normalImage;
		int pushedDirection = -1;
		int hoveredDirection = -1;
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
				if (this->pushedMultiplyFadeSpeed <= 0.0f)
				{
					this->_hoverAddTime = 0.0f;
				}
				pushedDirection = 1;
				if (this->pushedImage != NULL)
				{
					this->image = this->pushedImage;
				}
			}
			else
			{
				hoveredDirection = 1;
				if (this->hoverImage != NULL && aprilui::isHoverEffectEnabled())
				{
					this->image = this->hoverImage;
				}
			}
		}
		if (this->hoverAddFadeSpeed > 0.0f)
		{
			this->_hoverAddTime = hclamp(this->_hoverAddTime + timeDelta * this->hoverAddFadeSpeed * hoveredDirection, 0.0f, 1.0f);
		}
		if (this->pushedMultiplyFadeSpeed > 0.0f)
		{
			this->_pushedMultiplyTime = hclamp(this->_pushedMultiplyTime + timeDelta * this->pushedMultiplyFadeSpeed * pushedDirection, 0.0f, 1.0f);
		}
		ImageBox::_update(timeDelta);
	}
	
	bool ImageButton::triggerEvent(chstr type, april::Key keyCode)
	{
		return ImageBox::triggerEvent(type, keyCode);
	}

	bool ImageButton::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return ImageBox::triggerEvent(type, keyCode, string);
	}

	bool ImageButton::triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string, void* userData)
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
	
	void ImageButton::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		ImageBox::_mouseCancel(keyCode);
	}

	bool ImageButton::_mouseMove()
	{
		bool result = (ButtonBase::_mouseMove() || ImageBox::_mouseMove());
		if (this->pushed)
		{
			this->triggerEvent(Event::MouseMove);
		}
		return result;
	}

	bool ImageButton::_touchDown(int index)
	{
		bool result = ButtonBase::_touchDown(index);
		if (result)
		{
			this->triggerEvent(Event::TouchDown, april::Key::None, hstr(index));
		}
		return (result || ImageBox::_touchDown(index));
	}

	bool ImageButton::_touchUp(int index)
	{
		bool result = this->touched.has(index);
		bool released = ButtonBase::_touchUp(index);
		if (released)
		{
			this->triggerEvent(Event::TouchUp, april::Key::None, hstr(index));
			if (result)
			{
				this->triggerEvent(Event::Tap, april::Key::None, hstr(index));
			}
		}
		return (result || released || ImageBox::_touchUp(index));
	}

	void ImageButton::_touchCancel(int index)
	{
		ButtonBase::_touchCancel(index);
		ImageBox::_touchCancel(index);
	}

	bool ImageButton::_touchMove(int index)
	{
		bool result = (ButtonBase::_touchMove(index) || ImageBox::_touchMove(index));
		if (this->touched.has(index))
		{
			this->triggerEvent(Event::TouchMove, april::Key::None, hstr(index));
		}
		return result;
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
		if (ImageBox::_buttonUp(buttonCode))
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
	
}
