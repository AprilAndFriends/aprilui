/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "ObjectColored.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> Colored::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> Colored::_getters;
	hmap<hstr, PropertyDescription::Accessor*> Colored::_setters;

	Colored::Colored(chstr name) :
		Object(name),
		colorTopRight(april::Color::White),
		colorBottomLeft(april::Color::White),
		colorBottomRight(april::Color::White),
		useAdditionalColors(false)
	{
	}

	Colored::Colored(const Colored& other) :
		Object(other)
	{
		this->colorTopRight = other.colorTopRight;
		this->colorBottomLeft = other.colorBottomLeft;
		this->colorBottomRight = other.colorBottomRight;
		this->useAdditionalColors = other.useAdditionalColors;
	}

	Object* Colored::createInstance(chstr name)
	{
		return new Colored(name);
	}

	hmap<hstr, PropertyDescription>& Colored::getPropertyDescriptions() const
	{
		if (Colored::_propertyDescriptions.size() == 0)
		{
			Colored::_propertyDescriptions = Object::getPropertyDescriptions();
			Colored::_propertyDescriptions["color_top_right"] = PropertyDescription("color_top_right", PropertyDescription::Type::Color);
			Colored::_propertyDescriptions["red_top_right"] = PropertyDescription("red_top_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["green_top_right"] = PropertyDescription("green_top_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["blue_top_right"] = PropertyDescription("blue_top_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["alpha_top_right"] = PropertyDescription("alpha_top_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["color_bottom_left"] = PropertyDescription("color_bottom_left", PropertyDescription::Type::Color);
			Colored::_propertyDescriptions["red_bottom_left"] = PropertyDescription("red_bottom_left", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["green_bottom_left"] = PropertyDescription("green_bottom_left", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["blue_bottom_left"] = PropertyDescription("blue_bottom_left", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["alpha_bottom_left"] = PropertyDescription("alpha_bottom_left", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["color_bottom_right"] = PropertyDescription("color_bottom_right", PropertyDescription::Type::Color);
			Colored::_propertyDescriptions["red_bottom_right"] = PropertyDescription("red_bottom_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["green_bottom_right"] = PropertyDescription("green_bottom_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["blue_bottom_right"] = PropertyDescription("blue_bottom_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["alpha_bottom_right"] = PropertyDescription("alpha_bottom_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["use_additional_colors"] = PropertyDescription("use_additional_colors", PropertyDescription::Type::Bool);
		}
		return Colored::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Colored::_getGetters() const
	{
		if (Colored::_getters.size() == 0)
		{
			Colored::_getters = Object::_getGetters();
			Colored::_getters["color_top_right"] = new PropertyDescription::GetColor<Colored>(&Colored::getColorTopRight);
			Colored::_getters["red_top_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getRedTopRight);
			Colored::_getters["green_top_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getGreenTopRight);
			Colored::_getters["blue_top_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getBlueTopRight);
			Colored::_getters["alpha_top_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getAlphaTopRight);
			Colored::_getters["color_bottom_left"] = new PropertyDescription::GetColor<Colored>(&Colored::getColorBottomLeft);
			Colored::_getters["red_bottom_left"] = new PropertyDescription::GetUChar<Colored>(&Colored::getRedBottomLeft);
			Colored::_getters["green_bottom_left"] = new PropertyDescription::GetUChar<Colored>(&Colored::getGreenBottomLeft);
			Colored::_getters["blue_bottom_left"] = new PropertyDescription::GetUChar<Colored>(&Colored::getBlueBottomLeft);
			Colored::_getters["alpha_bottom_left"] = new PropertyDescription::GetUChar<Colored>(&Colored::getAlphaBottomLeft);
			Colored::_getters["color_bottom_right"] = new PropertyDescription::GetColor<Colored>(&Colored::getColorBottomRight);
			Colored::_getters["red_bottom_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getRedBottomRight);
			Colored::_getters["green_bottom_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getGreenBottomRight);
			Colored::_getters["blue_bottom_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getBlueBottomRight);
			Colored::_getters["alpha_bottom_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getAlphaBottomRight);
			Colored::_getters["use_additional_colors"] = new PropertyDescription::Get<Colored, bool>(&Colored::isUseAdditionalColors);
		}
		return Colored::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Colored::_getSetters() const
	{
		if (Colored::_setters.size() == 0)
		{
			Colored::_setters = Object::_getSetters();
			Colored::_setters["color_top_right"] = new PropertyDescription::Set<Colored, hstr>(&Colored::setSymbolicColorTopRight);
			Colored::_setters["red_top_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setRedTopRight);
			Colored::_setters["green_top_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setGreenTopRight);
			Colored::_setters["blue_top_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setBlueTopRight);
			Colored::_setters["alpha_top_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setAlphaTopRight);
			Colored::_setters["color_bottom_left"] = new PropertyDescription::Set<Colored, hstr>(&Colored::setSymbolicColorBottomLeft);
			Colored::_setters["red_bottom_left"] = new PropertyDescription::SetUChar<Colored>(&Colored::setRedBottomLeft);
			Colored::_setters["green_bottom_left"] = new PropertyDescription::SetUChar<Colored>(&Colored::setGreenBottomLeft);
			Colored::_setters["blue_bottom_left"] = new PropertyDescription::SetUChar<Colored>(&Colored::setBlueBottomLeft);
			Colored::_setters["alpha_bottom_left"] = new PropertyDescription::SetUChar<Colored>(&Colored::setAlphaBottomLeft);
			Colored::_setters["color_bottom_right"] = new PropertyDescription::Set<Colored, hstr>(&Colored::setSymbolicColorBottomRight);
			Colored::_setters["red_bottom_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setRedBottomRight);
			Colored::_setters["green_bottom_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setGreenBottomRight);
			Colored::_setters["blue_bottom_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setBlueBottomRight);
			Colored::_setters["alpha_bottom_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setAlphaBottomRight);
			Colored::_setters["use_additional_colors"] = new PropertyDescription::Set<Colored, bool>(&Colored::setUseAdditionalColors);
		}
		return Colored::_setters;
	}

	void Colored::setColorTopRight(const april::Color& value)
	{
		this->colorTopRight = value;
		this->useAdditionalColors = true;
	}

	void Colored::setSymbolicColorTopRight(chstr value)
	{
		this->setColorTopRight(aprilui::_makeColor(value));
	}

	void Colored::setColorBottomLeft(const april::Color& value)
	{
		this->colorBottomLeft = value;
		this->useAdditionalColors = true;
	}

	void Colored::setSymbolicColorBottomLeft(chstr value)
	{
		this->setColorBottomLeft(aprilui::_makeColor(value));
	}

	void Colored::setColorBottomRight(const april::Color& value)
	{
		this->colorBottomRight = value;
		this->useAdditionalColors = true;
	}

	void Colored::setSymbolicColorBottomRight(chstr value)
	{
		this->setColorBottomRight(aprilui::_makeColor(value));
	}

}
