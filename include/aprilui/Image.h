/// @file
/// @version 4.2
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
#include "BaseImage.h"
#include "PropertyDescription.h"

#define APRILUI_IMAGE_MAX_VERTICES 6

namespace aprilui
{
	class Texture;
	
	class apriluiExport Image : public BaseImage
	{
		APRILUI_CLONEABLE(Image);
	public:
		Image(Texture* texture, chstr name, cgrect source);
		~Image();
		inline hstr getClassName() const { return "Image"; }

		static Image* createInstance(Texture* texture, chstr name, cgrect source);

		HL_DEFINE_GET(Texture*, texture, Texture);
		HL_DEFINE_GET(grect, srcRect, SrcRect);
		void setSrcRect(cgrect value);
		HL_DEFINE_GET(float, srcRect.x, SrcX);
		void setSrcX(float value);
		HL_DEFINE_GET(float, srcRect.y, SrcY);
		void setSrcY(float value);
		HL_DEFINE_GET(float, srcRect.w, SrcWidth);
		void setSrcWidth(const float& value);
		HL_DEFINE_GET(float, srcRect.h, SrcHeight);
		void setSrcHeight(const float& value);
		inline gvec2 getSrcPosition() const { return this->srcRect.getPosition(); }
		void setSrcPosition(cgvec2 value);
		void setSrcPosition(float x, float y);
		inline gvec2 getSrcSize() const { return this->srcRect.getSize(); }
		void setSrcSize(cgvec2 value);
		void setSrcSize(float w, float h);
		HL_DEFINE_GETSET(april::Color, color, Color);
		void setSymbolicColor(chstr value);
		HL_DEFINE_ISSET(rotated, Rotated);
		HL_DEFINE_ISSET(invertX, InvertX);
		HL_DEFINE_ISSET(invertY, InvertY);
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_GETSET(april::ColorMode, colorMode, ColorMode);
		HL_DEFINE_GETSET(float, colorModeFactor, ColorModeFactor);
		inline const april::TexturedVertex* getVertices() const { return this->vertices; } // use with care!

		harray<PropertyDescription> getPropertyDescriptions() const;

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void draw(cgrect rect, april::Color color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, april::Color color = april::Color::White);

		void tryLoadTextureCoordinates();

	protected:
		Texture* texture;
		grect srcRect;
		april::Color color;
		april::BlendMode blendMode;
		april::ColorMode colorMode;
		float colorModeFactor;
		bool rotated;
		bool invertX;
		bool invertY;
		april::TexturedVertex vertices[APRILUI_IMAGE_MAX_VERTICES];

		grect _makeClippedSrcRect() const;
		
	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

}
#endif
