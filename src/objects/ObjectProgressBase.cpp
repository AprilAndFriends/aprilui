/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiUtil.h"
#include "BaseImage.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	harray<PropertyDescription> ProgressBase::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ProgressBase::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ProgressBase::_setters;

	ProgressBase::ProgressBase()
	{
		this->progressImage = NULL;
		this->antiProgressImage = NULL;
		this->maskImage = NULL;
		this->progress = 1.0f;
		this->progressColor = april::Color::White;
		this->antiProgressColor = april::Color::White;
		this->maskColor = april::Color::White;
	}

	ProgressBase::ProgressBase(const ProgressBase& other)
	{
		this->progressImage = other.progressImage;
		this->progressImageName = other.progressImageName;
		this->antiProgressImage = other.antiProgressImage;
		this->antiProgressImageName = other.antiProgressImageName;
		this->maskImage = other.maskImage;
		this->maskImageName = other.maskImageName;
		this->progress = other.progress;
		this->progressColor = other.progressColor;
		this->antiProgressColor = other.antiProgressColor;
		this->maskColor = other.maskColor;
	}

	ProgressBase::~ProgressBase()
	{
	}

	harray<PropertyDescription> ProgressBase::getPropertyDescriptions() const
	{
		if (ProgressBase::_propertyDescriptions.size() == 0)
		{
			ProgressBase::_propertyDescriptions += PropertyDescription("progress_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions += PropertyDescription("anti_progress_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions += PropertyDescription("mask_image", PropertyDescription::Type::String);
			ProgressBase::_propertyDescriptions += PropertyDescription("progress", PropertyDescription::Type::Float);
			ProgressBase::_propertyDescriptions += PropertyDescription("progress_color", PropertyDescription::Type::Color);
			ProgressBase::_propertyDescriptions += PropertyDescription("anti_progress_color", PropertyDescription::Type::Color);
			ProgressBase::_propertyDescriptions += PropertyDescription("mask_color", PropertyDescription::Type::Color);
		}
		return ProgressBase::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ProgressBase::_getGetters() const
	{
		if (ProgressBase::_getters.size() == 0)
		{
			ProgressBase::_getters["progress_image"] = new PropertyDescription::Get<ProgressBase, hstr>(&ProgressBase::getProgressImageName);
			ProgressBase::_getters["anti_progress_image"] = new PropertyDescription::Get<ProgressBase, hstr>(&ProgressBase::getAntiProgressImageName);
			ProgressBase::_getters["mask_image"] = new PropertyDescription::Get<ProgressBase, hstr>(&ProgressBase::getMaskImageName);
			ProgressBase::_getters["progress"] = new PropertyDescription::Get<ProgressBase, float>(&ProgressBase::getProgress);
			ProgressBase::_getters["progress_color"] = new PropertyDescription::GetColor<ProgressBase>(&ProgressBase::getProgressColor);
			ProgressBase::_getters["anti_progress_color"] = new PropertyDescription::GetColor<ProgressBase>(&ProgressBase::getAntiProgressColor);
			ProgressBase::_getters["mask_color"] = new PropertyDescription::GetColor<ProgressBase>(&ProgressBase::getMaskColor);
		}
		return ProgressBase::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ProgressBase::_getSetters() const
	{
		if (ProgressBase::_setters.size() == 0)
		{
			ProgressBase::_setters["progress_image"] = new PropertyDescription::TrySet<ProgressBase, hstr>(&ProgressBase::trySetProgressImageByName);
			ProgressBase::_setters["anti_progress_image"] = new PropertyDescription::TrySet<ProgressBase, hstr>(&ProgressBase::trySetAntiProgressImageByName);
			ProgressBase::_setters["mask_image"] = new PropertyDescription::TrySet<ProgressBase, hstr>(&ProgressBase::trySetMaskImageByName);
			ProgressBase::_setters["progress"] = new PropertyDescription::Set<ProgressBase, float>(&ProgressBase::setProgress);
			ProgressBase::_setters["progress_color"] = new PropertyDescription::SetColor<ProgressBase>(&ProgressBase::setProgressColor);
			ProgressBase::_setters["anti_progress_color"] = new PropertyDescription::SetColor<ProgressBase>(&ProgressBase::setAntiProgressColor);
			ProgressBase::_setters["mask_color"] = new PropertyDescription::SetColor<ProgressBase>(&ProgressBase::setMaskColor);
		}
		return ProgressBase::_setters;
	}

	void ProgressBase::setProgressImage(BaseImage* image)
	{
		this->progressImage = image;
		this->progressImageName = (image != NULL ? image->getFullName() : "");
	}

	void ProgressBase::setAntiProgressImage(BaseImage* image)
	{
		this->antiProgressImage = image;
		this->antiProgressImageName = (image != NULL ? image->getFullName() : "");
	}

	void ProgressBase::setMaskImage(BaseImage* image)
	{
		this->maskImage = image;
		this->maskImageName = (image != NULL ? image->getFullName() : "");
	}

	void ProgressBase::setProgressImageByName(chstr name)
	{
		this->setProgressImage(name != "" ? this->getDataset()->getImage(name) : NULL);
	}

	void ProgressBase::setAntiProgressImageByName(chstr name)
	{
		this->setAntiProgressImage(name != "" ? this->getDataset()->getImage(name) : NULL);
	}

	void ProgressBase::setMaskImageByName(chstr name)
	{
		this->setMaskImage(name != "" ? this->getDataset()->getImage(name) : NULL);
	}

	harray<BaseImage*> ProgressBase::_getUsedImages() const
	{
		harray<BaseImage*> images;
		images += this->progressImage;
		images += this->antiProgressImage;
		images += this->maskImage;
		return images;
	}

	bool ProgressBase::trySetProgressImageByName(chstr name)
	{
		if (this->progressImageName != name)
		{
			this->setProgressImageByName(name);
			return true;
		}
		return false;
	}
	
	bool ProgressBase::trySetAntiProgressImageByName(chstr name)
	{
		if (this->antiProgressImageName != name)
		{
			this->setAntiProgressImageByName(name);
			return true;
		}
		return false;
	}

	bool ProgressBase::trySetMaskImageByName(chstr name)
	{
		if (this->maskImageName != name)
		{
			this->setMaskImageByName(name);
			return true;
		}
		return false;
	}
	
	void ProgressBase::setProgressSymbolicColor(chstr value)
	{
		this->setProgressColor(aprilui::_makeColor(value));
	}
	
	void ProgressBase::setAntiProgressSymbolicColor(chstr value)
	{
		this->setAntiProgressColor(aprilui::_makeColor(value));
	}
	
	void ProgressBase::setMaskSymbolicColor(chstr value)
	{
		this->setMaskColor(aprilui::_makeColor(value));
	}
	
	april::Color ProgressBase::_makeDrawProgressColor() const
	{
		return this->_makeDrawColor(this->progressColor);	
	}
	
	april::Color ProgressBase::_makeDrawAntiProgressColor() const
	{
		return this->_makeDrawColor(this->antiProgressColor);
	}
	
	april::Color ProgressBase::_makeDrawMaskColor() const
	{
		return this->_makeDrawColor(this->maskColor);		
	}
	
}
