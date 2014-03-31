/// @file
/// @author  Boris Mikic
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressCircle.h"

namespace aprilui
{
	ProgressCircle::ProgressCircle(chstr name, grect rect) : ImageBox(name, rect), ProgressBase()
	{
		this->startAngle = 0.0f;
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
				harray<april::TexturedVertex> vertices;
				{
					april::TexturedVertex vertex(rect.centerX(), rect.centerY(), 0.0f);
					vertex.u = 0.5f;
					vertex.v = 0.5f;
					vertices += vertex;
				}
				{
					april::TexturedVertex vertex(rect.centerX(), rect.y, 0.0f);
					vertex.u = 0.5f;
					vertex.v = 0.0f;
					vertices += vertex;
				}
				if (progress > 0.125f)
				{
					april::TexturedVertex vertex(rect.right(), rect.y, 0.0f);
					vertex.u = 1.0f;
					vertex.v = 0.0f;
					vertices += vertex;
					vertices += vertices.first();
					vertices += vertex;
				}
				if (progress > 0.375f)
				{
					april::TexturedVertex vertex(rect.right(), rect.bottom(), 0.0f);
					vertex.u = 1.0f;
					vertex.v = 1.0f;
					vertices += vertex;
					vertices += vertices.first();
					vertices += vertex;
				}
				if (progress > 0.625f)
				{
					april::TexturedVertex vertex(rect.x, rect.bottom(), 0.0f);
					vertex.u = 0.0f;
					vertex.v = 1.0f;
					vertices += vertex;
					vertices += vertices.first();
					vertices += vertex;
				}
				if (progress > 0.875f)
				{
					april::TexturedVertex vertex(rect.x, rect.y, 0.0f);
					vertex.u = 0.0f;
					vertex.v = 0.0f;
					vertices += vertex;
					//vertices += vertices.first();
					//vertices += vertex;
					//vertices += vertices[1];
				}
				else
				{
					vertices += vertices.first();
				}
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
		if (name == "start_angle")	return this->getStartAngle();
		if (name == "direction")
		{
			if (this->direction == Clockwise)			return "clockwise";
			if (this->direction == Counterclockwise)	return "counterclockwise";
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
		if		(name == "start_angle")	this->setStartAngle(value);
		else if (name == "direction")
		{
			if (value == "clockwise")				this->setDirection(Clockwise);
			else if (value == "counterclockwise")	this->setDirection(Counterclockwise);
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
