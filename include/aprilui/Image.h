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
/// Defines a class for normal image definitions.

#ifndef APRILUI_IMAGE_H
#define APRILUI_IMAGE_H

#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "MinimalImage.h"
#include "PropertyDescription.h"

namespace aprilui
{
	class Texture;
	
	class apriluiExport Image : public MinimalImage
	{
		APRILUI_CLONEABLE(Image);
	public:
		Image(Texture* texture, chstr name, cgrectf source);
		inline hstr getClassName() const { return "Image"; }

		static MinimalImage* createInstance(Texture* texture, chstr name, cgrectf source);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GETSET(april::Color, colorTopLeft, Color);
		void setSymbolicColor(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorTopLeft.r, Red);
		HL_DEFINE_GETSET(unsigned char, colorTopLeft.g, Green);
		HL_DEFINE_GETSET(unsigned char, colorTopLeft.b, Blue);
		HL_DEFINE_GETSET(unsigned char, colorTopLeft.a, Alpha);
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
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_GETSET(april::ColorMode, colorMode, ColorMode);
		HL_DEFINE_GETSET(float, colorModeFactor, ColorModeFactor);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void draw(cgrectf rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);

	protected:
		april::Color colorTopLeft;
		april::Color colorTopRight;
		april::Color colorBottomLeft;
		april::Color colorBottomRight;
		bool useAdditionalColors;
		april::BlendMode blendMode;
		april::ColorMode colorMode;
		float colorModeFactor;
		april::ColoredTexturedVertex coloredVertices[APRILUI_IMAGE_MAX_VERTICES];

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		void _drawWithCorners(cgrectf rect, const april::Color& color);

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
