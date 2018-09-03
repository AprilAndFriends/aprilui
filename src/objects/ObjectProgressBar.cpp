/// @file
/// @version 5.0
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
#include "ObjectProgressBar.h"
#include "Texture.h"

#define MAX_DIRECTION 10 // used for optimizing some code

namespace aprilui
{
	HL_ENUM_CLASS_DEFINE(ProgressBar::Direction,
	(
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Down, 2);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Left, 4);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Right, 6);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Up, 8);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Horizontal, 98);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Vertical, 99);

		bool ProgressBar::Direction::isHorizontal() const
		{
			return (*this == Left || *this == Right || *this == Horizontal);
		}

		bool ProgressBar::Direction::isVertical() const
		{
			return (*this == Up || *this == Down || *this == Vertical);
		}

		bool ProgressBar::Direction::hasAntiProgress() const
		{
			return (*this != Horizontal && *this != Vertical);
		}

		ProgressBar::Direction ProgressBar::Direction::getOppositeDirection() const
		{
			if (*this == Horizontal || *this == Vertical)
			{
				return (*this);
			}
			return ProgressBar::Direction::fromInt(MAX_DIRECTION - this->value);
		}
	));

	hmap<hstr, PropertyDescription> ProgressBar::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ProgressBar::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ProgressBar::_setters;

	ProgressBar::ProgressBar(chstr name) :
		ImageBox(name),
		ProgressBase()
	{
		this->stretching = false;
		this->direction = Direction::Right;
		this->interactable = false;
		this->pushed = false;
	}

	ProgressBar::ProgressBar(const ProgressBar& other) :
		ImageBox(other),
		ProgressBase(other)
	{
		this->stretching = other.stretching;
		this->direction = other.direction;
		this->interactable = other.interactable;
		this->pushed = false;
	}

	Object* ProgressBar::createInstance(chstr name)
	{
		return new ProgressBar(name);
	}

	hmap<hstr, PropertyDescription>& ProgressBar::getPropertyDescriptions() const
	{
		if (ProgressBar::_propertyDescriptions.size() == 0)
		{
			ProgressBar::_propertyDescriptions = ImageBox::getPropertyDescriptions() + ProgressBase::getPropertyDescriptions();
			ProgressBar::_propertyDescriptions["stretching"] = PropertyDescription("stretching", PropertyDescription::Type::Bool);
			ProgressBar::_propertyDescriptions["direction"] = PropertyDescription("direction", PropertyDescription::Type::Enum);
			ProgressBar::_propertyDescriptions["interactable"] = PropertyDescription("interactable", PropertyDescription::Type::Bool);
		}
		return ProgressBar::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ProgressBar::_getGetters() const
	{
		if (ProgressBar::_getters.size() == 0)
		{
			ProgressBar::_getters = ImageBox::_getGetters() + ProgressBase::_generateGetters<ProgressBar>();
			ProgressBar::_getters["stretching"] = new PropertyDescription::Get<ProgressBar, bool>(&ProgressBar::isStretching);
			ProgressBar::_getters["interactable"] = new PropertyDescription::Get<ProgressBar, bool>(&ProgressBar::isInteractable);
		}
		return ProgressBar::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ProgressBar::_getSetters() const
	{
		if (ProgressBar::_setters.size() == 0)
		{
			ProgressBar::_setters = ImageBox::_getSetters() + ProgressBase::_generateSetters<ProgressBar>();
			ProgressBar::_setters["stretching"] = new PropertyDescription::Set<ProgressBar, bool>(&ProgressBar::setStretching);
			ProgressBar::_setters["interactable"] = new PropertyDescription::Set<ProgressBar, bool>(&ProgressBar::setInteractable);
		}
		return ProgressBar::_setters;
	}

	Dataset* ProgressBar::getDataset() const
	{
		return ImageBox::getDataset();
	}

	BaseImage* ProgressBar::getImage() const
	{
		return ImageBox::getImage();
	}

	void ProgressBar::setImage(BaseImage* image)
	{
		ImageBox::setImage(image);
	}

	hstr ProgressBar::getImageName() const
	{
		return ImageBox::getImageName();
	}

	void ProgressBar::setImageByName(chstr name)
	{
		ImageBox::setImageByName(name);
	}
	
	harray<BaseImage*> ProgressBar::_getUsedImages() const
	{
		return (ProgressBase::_getUsedImages() + ImageBox::_getUsedImages());
	}

	bool ProgressBar::trySetImageByName(chstr name)
	{
		return ImageBox::trySetImageByName(name);
	}

	void ProgressBar::_update(float timeDelta)
	{
		ImageBox::_update(timeDelta);
		if (this->interactable && this->pushed)
		{
			this->_updateInteractablePosition();
		}
	}

	void ProgressBar::_draw()
	{
		ImageBox::_draw();
		float progress = hclamp(this->progress, 0.0f, 1.0f);
		grectf drawRect = this->_makeDrawRect();
		grectf directionRect;
		if (this->antiProgressImage != NULL && progress < 1.0f && this->direction.hasAntiProgress())
		{
			april::Color drawAntiProgressColor = this->_makeDrawAntiProgressColor();
			Direction antiDirection = this->direction.getOppositeDirection();
			float antiProgress = 1.0f - progress;
			if (this->stretching)
			{
				this->antiProgressImage->draw(this->_calcRectDirection(drawRect, antiProgress, antiDirection), drawAntiProgressColor);
			}
			else
			{
				grectf clipRect = (this->progressImage != NULL ? this->progressImage->getClipRect() : this->rect);
				directionRect = this->_calcRectDirection(grectf(0.0f, 0.0f, this->antiProgressImage->getSrcSize()), antiProgress, antiDirection);
				this->antiProgressImage->setClipRect(directionRect);
				this->antiProgressImage->draw(drawRect, drawAntiProgressColor);
				this->antiProgressImage->setClipRect(clipRect);
			}
		}
		if (this->progressImage != NULL && progress > 0.0f)
		{
			april::Color drawProgressColor = this->_makeDrawProgressColor();
			if (this->stretching)
			{
				this->progressImage->draw(this->_calcRectDirection(drawRect, progress, this->direction), drawProgressColor);
			}
			else
			{
				grectf clipRect = this->progressImage->getClipRect();
				directionRect = this->_calcRectDirection(grectf(0.0f, 0.0f, this->progressImage->getSrcSize()), progress, this->direction);
				this->progressImage->setClipRect(directionRect);
				this->progressImage->draw(drawRect, drawProgressColor);
				this->progressImage->setClipRect(clipRect);
			}
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(drawRect, this->_makeDrawMaskColor());
		}
	}
	
	april::Color ProgressBar::_makeDrawColor(const april::Color& color) const
	{
		return ImageBox::_makeDrawColor(color);
	}

	grectf ProgressBar::_calcRectDirection(cgrectf rect, float progress, Direction direction)
	{
		grectf result = rect;
		float size = 0.0f;
		if (direction == Direction::Right)
		{
			result.w *= progress;
		}
		else if (direction == Direction::Left)
		{
			size = result.w * progress;
			result.x += result.w - size;
			result.w = size;
		}
		else if (direction == Direction::Horizontal)
		{
			size = result.w * progress;
			result.x += (result.w - size) * 0.5f;
			result.w = size;
		}
		else if (direction == Direction::Down)
		{
			result.h *= progress;
		}
		else if (direction == Direction::Up)
		{
			size = result.h * progress;
			result.y += result.h - size;
			result.h = size;
		}
		else if (direction == Direction::Vertical)
		{
			size = result.h * progress;
			result.y += (result.h - size) * 0.5f;
			result.h = size;
		}
		return result;
	}

	hstr ProgressBar::getProperty(chstr name)
	{
		if (name == "direction")	return this->direction.getName().lowered();
		return ImageBox::getProperty(name);
	}

	bool ProgressBar::setProperty(chstr name, chstr value)
	{
		if (name == "direction")
		{
			if (value == "right")			this->setDirection(Direction::Right);
			else if (value == "left")		this->setDirection(Direction::Left);
			else if (value == "down")		this->setDirection(Direction::Down);
			else if (value == "up")			this->setDirection(Direction::Up);
			else if (value == "horizontal")	this->setDirection(Direction::Horizontal);
			else if (value == "vertical")	this->setDirection(Direction::Vertical);
			else
			{
				hlog::warn(logTag, "'direction=' does not support value '" + value + "'.");
				return false;
			}
			return true;
		}
		return ImageBox::setProperty(name, value);
	}
	
	bool ProgressBar::_mouseDown(april::Key keyCode)
	{
		if (this->interactable && this->isCursorInside())
		{
			this->pushed = true;
			this->_updateInteractablePosition();
			// TODO - will be removed once the hover refactor has been done
			this->triggerEvent(Event::MouseDown, keyCode);
			return true;
		}
		return ImageBox::_mouseDown(keyCode);
	}

	bool ProgressBar::_mouseUp(april::Key keyCode)
	{
		if (this->interactable && this->pushed)
		{
			// TODO - will be removed once the hover refactor has been done
			this->triggerEvent(Event::MouseUp, keyCode);
			if (this->isCursorInside())
			{
				this->pushed = false;
				return true;
			}
		}
		this->pushed = false;
		return ImageBox::_mouseUp(keyCode);
	}

	void ProgressBar::_mouseCancel(april::Key keyCode)
	{
		this->pushed = false;
		ImageBox::_mouseCancel(keyCode);
	}

	bool ProgressBar::_mouseMove()
	{
		if (this->pushed)
		{
			this->_updateInteractablePosition();
		}
		return ImageBox::_mouseMove();
	}

	void ProgressBar::_updateInteractablePosition()
	{
		gvec2f position = this->transformToLocalSpace(aprilui::getCursorPosition());
		float newProgress = 0.0f;
		if (this->direction == Direction::Right)
		{
			newProgress = position.x / this->rect.w;
		}
		else if (this->direction == Direction::Left)
		{
			newProgress = 1.0f - position.x / this->rect.w;
		}
		else if (this->direction == Direction::Down)
		{
			newProgress = position.y / this->rect.h;
		}
		else if (this->direction == Direction::Up)
		{
			newProgress = 1.0f - position.y / this->rect.h;
		}
		newProgress = hclamp(newProgress, 0.0f, 1.0f);
		if (this->progress != newProgress)
		{
			this->progress = newProgress;
			this->progress = newProgress;
			this->triggerEvent(Event::SetProgressValue);
		}
	}

}
