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
/// Defines a class for minimal image definitions.

#ifndef APRILUI_COLOR_IMAGE_H
#define APRILUI_COLOR_IMAGE_H

#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "BaseImage.h"
#include "PropertyDescription.h"

#define APRILUI_COLOR_MAX_VERTICES 6

namespace aprilui
{
	class Texture;
	
	class apriluiExport ColorImage : public BaseImage
	{
		APRILUI_CLONEABLE(ColorImage);
	public:
		ColorImage(chstr name, const harray<april::Color>& colors);
		inline hstr getClassName() const { return "ColorImage"; }

		HL_DEFINE_GETSET(april::Color, colorTopLeft, ColorTopLeft);
		void setSymbolicColorTopLeft(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorTopLeft.r, RedTopLeft);
		HL_DEFINE_GETSET(unsigned char, colorTopLeft.g, GreenTopLeft);
		HL_DEFINE_GETSET(unsigned char, colorTopLeft.b, BlueTopLeft);
		HL_DEFINE_GETSET(unsigned char, colorTopLeft.a, AlphaTopLeft);
		HL_DEFINE_GETSET(april::Color, colorTopRight, ColorTopRight);
		void setSymbolicColorTopRight(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.r, RedTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.g, GreenTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.b, BlueTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.a, AlphaTopRight);
		HL_DEFINE_GETSET(april::Color, colorBottomLeft, ColorBottomLeft);
		void setSymbolicColorBottomLeft(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.r, RedBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.g, GreenBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.b, BlueBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.a, AlphaBottomLeft);
		HL_DEFINE_GETSET(april::Color, colorBottomRight, ColorBottomRight);
		void setSymbolicColorBottomRight(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.r, RedBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.g, GreenBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.b, BlueBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.a, AlphaBottomRight);

		inline float getSrcWidth() const { return 0.0f; };
		inline void setSrcWidth(const float& value) { }
		inline float getSrcHeight() const { return 0.0f; };
		inline void setSrcHeight(const float& value) { }
		inline gvec2f getSrcSize() const { return gvec2f(); }
		inline void setSrcSize(cgvec2f value) { }

		void draw(cgrectf rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);

	protected:
		april::Color colorTopRight;
		april::Color colorTopLeft;
		april::Color colorBottomRight;
		april::Color colorBottomLeft;
		april::ColoredVertex vertices[APRILUI_COLOR_MAX_VERTICES];

	};

}
#endif
