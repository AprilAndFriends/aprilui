/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "BaseImage.h"
#include "Dataset.h"
#include "ObjectProgressCircle.h"

#define MAKE_VERTEX(vec2) april::TexturedVertex(rect.x + (vec2).x * rect.w, rect.y + (vec2).y * rect.h, 0.0f, (vec2).x, (vec2).y)

namespace aprilui
{
	HL_ENUM_CLASS_DEFINE(ProgressCircle::Direction,
	(
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Clockwise270, 2);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Clockwise180, 4);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Clockwise, 6);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Clockwise90, 8);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, ClockwiseMax, 10);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Counterclockwise270, 20);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Counterclockwise180, 40);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Counterclockwise, 60);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Counterclockwise90, 80);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, CounterclockwiseMax, 100);

		int ProgressCircle::Direction::getAngle() const
		{
			if (*this == Clockwise270 || *this == Counterclockwise270)
			{
				return 270;
			}
			if (*this == Clockwise180 || *this == Counterclockwise180)
			{
				return 180;
			}
			if (*this == Clockwise90 || *this == Counterclockwise90)
			{
				return 90;
			}
			return 0;
		}

		bool ProgressCircle::Direction::isClockwise() const
		{
			return (this->value <= ClockwiseMax.value);
		}

		bool ProgressCircle::Direction::isCounterclockwise() const
		{
			return (this->value > ClockwiseMax.value);
		}

	));

	harray<PropertyDescription> ProgressCircle::_propertyDescriptions;

	ProgressCircle::ProgressCircle(chstr name) : ImageBox(name), ProgressBase()
	{
		this->direction = Direction::Clockwise;
	}

	ProgressCircle::ProgressCircle(const ProgressCircle& other) : ImageBox(other), ProgressBase(other)
	{
		this->direction = other.direction;
	}

	ProgressCircle::~ProgressCircle()
	{
	}

	Object* ProgressCircle::createInstance(chstr name)
	{
		return new ProgressCircle(name);
	}

	harray<PropertyDescription> ProgressCircle::getPropertyDescriptions() const
	{
		if (ProgressCircle::_propertyDescriptions.size() == 0)
		{
			ProgressCircle::_propertyDescriptions += PropertyDescription("direction", PropertyDescription::Type::Enum);
		}
		return (ImageBox::getPropertyDescriptions() + ProgressBase::getPropertyDescriptions() + ProgressCircle::_propertyDescriptions);
	}

	Dataset* ProgressCircle::getDataset() const
	{
		return ImageBox::getDataset();
	}

	BaseImage* ProgressCircle::getImage() const
	{
		return ImageBox::getImage();
	}

	void ProgressCircle::setImage(BaseImage* image)
	{
		ImageBox::setImage(image);
	}

	hstr ProgressCircle::getImageName() const
	{
		return ImageBox::getImageName();
	}

	void ProgressCircle::setImageByName(chstr name)
	{
		ImageBox::setImageByName(name);
	}

	harray<BaseImage*> ProgressCircle::_getUsedImages() const
	{
		return (ProgressBase::_getUsedImages() + ImageBox::_getUsedImages());
	}
	
	april::Color ProgressCircle::_makeDrawColor(const april::Color& color) const
	{
		return ImageBox::_makeDrawColor(color);
	}

	bool ProgressCircle::trySetImageByName(chstr name)
	{
		return ImageBox::trySetImageByName(name);
	}

	void ProgressCircle::_draw()
	{
		ImageBox::_draw();
		float progress = hclamp(this->progress, 0.0f, 1.0f);
		grect drawRect = this->_makeDrawRect();
		if (this->antiProgressImage != NULL)
		{
			if (progress <= 0.0f)
			{
				this->antiProgressImage->draw(drawRect, this->_makeDrawAntiProgressColor());
			}
			else if (progress < 1.0f)
			{
				Direction antiDirection = Direction::fromInt(this->direction.value < Direction::ClockwiseMax.value ? this->direction.value * 10 : this->direction.value / 10);
				this->antiProgressImage->draw(this->_calcVertices(drawRect, 1.0f - progress, antiDirection), this->_makeDrawAntiProgressColor());
			}
		}
		if (this->progressImage != NULL)
		{
			if (progress >= 1.0f)
			{
				this->progressImage->draw(drawRect, this->_makeDrawProgressColor());
			}
			else if (progress > 0.0f)
			{
				this->progressImage->draw(this->_calcVertices(drawRect, progress, this->direction), this->_makeDrawProgressColor());
			}
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(drawRect, this->_makeDrawMaskColor());
		}
	}

	harray<april::TexturedVertex> ProgressCircle::_calcVertices(cgrect rect, float progress, Direction direction)
	{
		harray<april::TexturedVertex> result;
		gvec2 splitCenter;
		gvec2 topLeft;
		gvec2 topRight;
		gvec2 bottomLeft;
		gvec2 bottomRight;
		if (direction == Direction::Clockwise)
		{
			splitCenter.set(1.0f, 0.5f);	topLeft.set(0.0f, 1.0f);	topRight.set(1.0f, 1.0f);	bottomLeft.set(0.0f, 0.0f);		bottomRight.set(1.0f, 0.0f);
		}
		else if (direction == Direction::Clockwise90)
		{
			splitCenter.set(0.5f, 0.0f);	topLeft.set(1.0f, 1.0f);	topRight.set(1.0f, 0.0f);	bottomLeft.set(0.0f, 1.0f);		bottomRight.set(0.0f, 0.0f);
		}
		else if (direction == Direction::Clockwise180)
		{
			splitCenter.set(0.0f, 0.5f);	topLeft.set(1.0f, 0.0f);	topRight.set(0.0f, 0.0f);	bottomLeft.set(1.0f, 1.0f);		bottomRight.set(0.0f, 1.0f);
		}
		else if (direction == Direction::Clockwise270)
		{
			splitCenter.set(0.5f, 1.0f);	topLeft.set(0.0f, 0.0f);	topRight.set(0.0f, 1.0f);	bottomLeft.set(1.0f, 0.0f);		bottomRight.set(1.0f, 1.0f);
		}
		else if (direction == Direction::Counterclockwise)
		{
			splitCenter.set(1.0f, 0.5f);	topLeft.set(0.0f, 0.0f);	topRight.set(1.0f, 0.0f);	bottomLeft.set(0.0f, 1.0f);		bottomRight.set(1.0f, 1.0f);
		}
		else if (direction == Direction::Counterclockwise90)
		{
			splitCenter.set(0.5f, 0.0f);	topLeft.set(0.0f, 1.0f);	topRight.set(0.0f, 0.0f);	bottomLeft.set(1.0f, 1.0f);		bottomRight.set(1.0f, 0.0f);
		}
		else if (direction == Direction::Counterclockwise180)
		{
			splitCenter.set(0.0f, 0.5f);	topLeft.set(1.0f, 1.0f);	topRight.set(0.0f, 1.0f);	bottomLeft.set(1.0f, 0.0f);		bottomRight.set(0.0f, 0.0f);
		}
		else if (direction == Direction::Counterclockwise270)
		{
			splitCenter.set(0.5f, 1.0f);	topLeft.set(1.0f, 0.0f);	topRight.set(1.0f, 1.0f);	bottomLeft.set(0.0f, 0.0f);		bottomRight.set(0.0f, 1.0f);
		}
		harray<april::TexturedVertex> vertices;
		result += MAKE_VERTEX(gvec2(0.5f, 0.5f));
		result += MAKE_VERTEX(splitCenter);
		april::TexturedVertex vertex;
		gvec2 p0 = bottomRight;
		gvec2 p1 = topRight;
		if (progress >= 0.125f)
		{
			vertex = MAKE_VERTEX(topRight);
			result += vertex;
			result += result.first();
			result += vertex;
			p0 = topRight;
			p1 = topLeft;
		}
		if (progress >= 0.375f)
		{
			vertex = MAKE_VERTEX(topLeft);
			result += vertex;
			result += result.first();
			result += vertex;
			p0 = topLeft;
			p1 = bottomLeft;
		}
		if (progress >= 0.625f)
		{
			vertex = MAKE_VERTEX(bottomLeft);
			result += vertex;
			result += result.first();
			result += vertex;
			p0 = bottomLeft;
			p1 = bottomRight;
		}
		if (progress >= 0.875f)
		{
			vertex = MAKE_VERTEX(bottomRight);
			result += vertex;
			result += result.first();
			result += vertex;
			p0 = bottomRight;
			p1 = topRight;
		}
		double angle = hmodf(progress * 360.0f + 45.0f, 90.0f) - 45.0f;
		// angle will always be between -45° and 45° so there is no tan() risk here
		float ratio = (float)htan(angle) * 0.5f + 0.5f;
		p0 += (p1 - p0) * ratio;
		result += MAKE_VERTEX(p0);
		return result;
	}

	hstr ProgressCircle::getProperty(chstr name)
	{
		if (name == "direction")	return this->direction.getName().lowered();
		hstr result = ProgressBase::getProperty(name);
		if (result == "")
		{
			result = ImageBox::getProperty(name);
		}
		return result;
	}

	bool ProgressCircle::setProperty(chstr name, chstr value)
	{
		if		(name == "direction")
		{
			if (value == "clockwise")					this->setDirection(Direction::Clockwise);
			else if (value == "clockwise90")			this->setDirection(Direction::Clockwise90);
			else if (value == "clockwise180")			this->setDirection(Direction::Clockwise180);
			else if (value == "clockwise270")			this->setDirection(Direction::Clockwise270);
			else if (value == "counterclockwise")		this->setDirection(Direction::Counterclockwise);
			else if (value == "counterclockwise90")		this->setDirection(Direction::Counterclockwise90);
			else if (value == "counterclockwise180")	this->setDirection(Direction::Counterclockwise180);
			else if (value == "counterclockwise270")	this->setDirection(Direction::Counterclockwise270);
			else
			{
				hlog::warn(logTag, "'direction=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (ProgressBase::setProperty(name, value)) { }
		else return ImageBox::setProperty(name, value);
		return true;
	}
	
}
