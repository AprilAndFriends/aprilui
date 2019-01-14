/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a filled rectangle object.

#ifndef APRILUI_COLORED_H
#define APRILUI_COLORED_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport Colored : public Object
	{
		APRILUI_CLONEABLE(Colored);
	public:
		Colored(chstr name);
		inline hstr getClassName() const { return "Colored"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(april::Color, colorTopRight, ColorTopRight);
		void setColorTopRight(const april::Color& value);
		void setSymbolicColorTopRight(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.r, RedTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.g, GreenTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.b, BlueTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.a, AlphaTopRight);
		HL_DEFINE_GET(april::Color, colorBottomLeft, ColorBottomLeft);
		void setColorBottomLeft(const april::Color& value);
		void setSymbolicColorBottomLeft(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.r, RedBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.g, GreenBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.b, BlueBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.a, AlphaBottomLeft);
		HL_DEFINE_GET(april::Color, colorBottomRight, ColorBottomRight);
		void setColorBottomRight(const april::Color& value);
		void setSymbolicColorBottomRight(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.r, RedBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.g, GreenBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.b, BlueBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.a, AlphaBottomRight);
		HL_DEFINE_ISSET(useAdditionalColors, UseAdditionalColors);

	protected:
		april::Color colorTopRight;
		april::Color colorBottomLeft;
		april::Color colorBottomRight;
		bool useAdditionalColors;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
