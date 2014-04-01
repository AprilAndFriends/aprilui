/// @file
/// @author  Boris Mikic
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressCircle.h"

#define MAKE_VERTEX(vec2) april::TexturedVertex(rect.x + (vec2).x * rect.w, rect.y + (vec2).y * rect.h, 0.0f, (vec2).x, (vec2).y)

namespace aprilui
{
	ProgressCircle::ProgressCircle(chstr name, grect rect) : ImageBox(name, rect), ProgressBase()
	{
		this->direction = Clockwise;
	}

	ProgressCircle::~ProgressCircle()
	{
	}

	Object* ProgressCircle::createInstance(chstr name, grect rect)
	{
		return new ProgressCircle(name, rect);
	}

	Dataset* ProgressCircle::getDataset()
	{
		return ImageBox::getDataset();
	}

	void ProgressCircle::OnDraw()
	{
		ImageBox::OnDraw();
		float progress = hclamp(this->progress, 0.0f, 1.0f);
		april::Color color = this->_getDrawColor();
		color.a = (unsigned char)(color.a * this->_getDisabledAlphaFactor());
		if (this->progressImage != NULL)
		{
			grect rect = this->_getDrawRect();
			if (progress == 1.0f)
			{
				this->progressImage->draw(rect, color);
			}
			else if (progress > 0.0f)
			{
				gvec2 p0;
				gvec2 p1;
				gvec2 splitCenter;
				gvec2 topLeft;
				gvec2 topRight;
				gvec2 bottomLeft;
				gvec2 bottomRight;
				switch (this->direction)
				{
				case Clockwise:
					splitCenter.set(1.0f, 0.5f);	topLeft.set(0.0f, 1.0f);	topRight.set(1.0f, 1.0f);	bottomLeft.set(0.0f, 0.0f);		bottomRight.set(1.0f, 0.0f);
					break;
				case Clockwise90:
					splitCenter.set(0.5f, 0.0f);	topLeft.set(1.0f, 1.0f);	topRight.set(1.0f, 0.0f);	bottomLeft.set(0.0f, 1.0f);		bottomRight.set(0.0f, 0.0f);
					break;
				case Clockwise180:
					splitCenter.set(0.0f, 0.5f);	topLeft.set(1.0f, 0.0f);	topRight.set(0.0f, 0.0f);	bottomLeft.set(1.0f, 1.0f);		bottomRight.set(0.0f, 1.0f);
					break;
				case Clockwise270:
					splitCenter.set(0.5f, 1.0f);	topLeft.set(0.0f, 0.0f);	topRight.set(0.0f, 1.0f);	bottomLeft.set(1.0f, 0.0f);		bottomRight.set(1.0f, 1.0f);
					break;
				case Counterclockwise:
					splitCenter.set(1.0f, 0.5f);	topLeft.set(0.0f, 0.0f);	topRight.set(1.0f, 0.0f);	bottomLeft.set(0.0f, 1.0f);		bottomRight.set(1.0f, 1.0f);
					break;
				case Counterclockwise90:
					splitCenter.set(0.5f, 0.0f);	topLeft.set(0.0f, 1.0f);	topRight.set(0.0f, 0.0f);	bottomLeft.set(1.0f, 1.0f);		bottomRight.set(1.0f, 0.0f);
					break;
				case Counterclockwise180:
					splitCenter.set(0.0f, 0.5f);	topLeft.set(1.0f, 1.0f);	topRight.set(0.0f, 1.0f);	bottomLeft.set(1.0f, 0.0f);		bottomRight.set(0.0f, 0.0f);
					break;
				case Counterclockwise270:
					splitCenter.set(0.5f, 1.0f);	topLeft.set(1.0f, 0.0f);	topRight.set(1.0f, 1.0f);	bottomLeft.set(0.0f, 0.0f);		bottomRight.set(0.0f, 1.0f);
					break;
				}
				harray<april::TexturedVertex> vertices;
				vertices += MAKE_VERTEX(gvec2(0.5f, 0.5f));
				vertices += MAKE_VERTEX(splitCenter);
				april::TexturedVertex vertex;
				p0 = bottomRight;
				p1 = topRight;
				if (progress > 0.125f)
				{
					vertex = MAKE_VERTEX(topRight);
					vertices += vertex;
					vertices += vertices.first();
					vertices += vertex;
					p0 = topRight;
					p1 = topLeft;
				}
				if (progress > 0.375f)
				{
					vertex = MAKE_VERTEX(topLeft);
					vertices += vertex;
					vertices += vertices.first();
					vertices += vertex;
					p0 = topLeft;
					p1 = bottomLeft;
				}
				if (progress > 0.625f)
				{
					vertex = MAKE_VERTEX(bottomLeft);
					vertices += vertex;
					vertices += vertices.first();
					vertices += vertex;
					p0 = bottomLeft;
					p1 = bottomRight;
				}
				if (progress > 0.875f)
				{
					vertex = MAKE_VERTEX(bottomRight);
					vertices += vertex;
					vertices += vertices.first();
					vertices += vertex;
					p0 = bottomRight;
					p1 = topRight;
				}
				double angle = hmodf(progress * 360.0f + 45.0f, 90.0f) - 45.0f;
				// angle will always be between -45° and 45° so there is no risk here
				float ratio = (float)dtan(angle) * 0.5f + 0.5f;
				p0 += (p1 - p0) * ratio;
				vertices += MAKE_VERTEX(p0);
				this->progressImage->draw(vertices, color);
			}
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(this->_getDrawRect(), color);
		}
	}

	hstr ProgressCircle::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "direction")
		{
			if (this->direction == Clockwise)			return "clockwise";
			if (this->direction == Clockwise90)			return "clockwise90";
			if (this->direction == Clockwise180)		return "clockwise180";
			if (this->direction == Clockwise270)		return "clockwise270";
			if (this->direction == Counterclockwise)	return "counterclockwise";
			if (this->direction == Counterclockwise90)	return "counterclockwise90";
			if (this->direction == Counterclockwise180)	return "counterclockwise180";
			if (this->direction == Counterclockwise270)	return "counterclockwise270";
		}
		bool exists = false;
		hstr result = ProgressBase::getProperty(name, &exists);
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

	bool ProgressCircle::setProperty(chstr name, chstr value)
	{
		if		(name == "direction")
		{
			if (value == "clockwise")					this->setDirection(Clockwise);
			else if (value == "clockwise90")			this->setDirection(Clockwise90);
			else if (value == "clockwise180")			this->setDirection(Clockwise180);
			else if (value == "clockwise270")			this->setDirection(Clockwise270);
			else if (value == "counterclockwise")		this->setDirection(Counterclockwise);
			else if (value == "counterclockwise90")		this->setDirection(Counterclockwise90);
			else if (value == "counterclockwise180")	this->setDirection(Counterclockwise180);
			else if (value == "counterclockwise270")	this->setDirection(Counterclockwise270);
			else
			{
				hlog::warn(aprilui::logTag, "'direction=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (ProgressBase::setProperty(name, value)) { }
		else return ImageBox::setProperty(name, value);
		return true;
	}
	
}
