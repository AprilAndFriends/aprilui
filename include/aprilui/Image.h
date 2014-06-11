/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for all special image classes and a basic image class.

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
#include "PropertyDescription.h"

#define APRILUI_IMAGE_NAME_NULL "null"

namespace aprilui
{
	class Dataset;
	class Texture;
	
	class apriluiExport Image
	{
	public:
		friend class Dataset;

		Image(Texture* texture, chstr name, grect source);
		Image(Image& other, chstr name);
		virtual ~Image();
		
		virtual void draw(grect rect, april::Color color = april::Color::White);
		virtual void draw(harray<april::TexturedVertex> vertices, april::Color color = april::Color::White);
		
		HL_DEFINE_GET(Dataset*, dataset, Dataset);
		HL_DEFINE_GET(Texture*, texture, Texture);
		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(grect, srcRect, SrcRect);
		void setSrcRect(grect value);
		HL_DEFINE_GET(float, srcRect.x, SrcX);
		void setSrcX(float value);
		HL_DEFINE_GET(float, srcRect.y, SrcY);
		void setSrcY(float value);
		HL_DEFINE_GET(float, srcRect.w, SrcWidth);
		void setSrcWidth(float value);
		HL_DEFINE_GET(float, srcRect.h, SrcHeight);
		void setSrcHeight(float value);
		inline gvec2 getPosition() { return this->srcRect.getPosition(); }
		void setPosition(gvec2 value);
		void setPosition(float x, float y);
		inline gvec2 getSize() { return this->srcRect.getSize(); }
		void setSize(gvec2 value);
		void setSize(float x, float y);
		HL_DEFINE_GETSET(april::Color, color, Color);
		HL_DEFINE_ISSET(rotated, Rotated);
		HL_DEFINE_ISSET(invertX, InvertX);
		HL_DEFINE_ISSET(invertY, InvertY);
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_GETSET(april::ColorMode, colorMode, ColorMode);
		HL_DEFINE_GETSET(float, colorModeFactor, ColorModeFactor);
		hstr getFullName();
		
		virtual harray<PropertyDescription> getPropertyDescriptions();

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		DEPRECATED_ATTRIBUTE inline bool isVertical() { return this->rotated; }
		DEPRECATED_ATTRIBUTE inline void setVertical(bool value) { this->rotated = value; }
		
	protected:
		Dataset* dataset;
		Texture* texture;
		hstr name;
		hstr imageName;
		grect srcRect;
		april::Color color;
		april::BlendMode blendMode;
		april::ColorMode colorMode;
		float colorModeFactor;
		bool rotated;
		bool invertX;
		bool invertY;

		bool _textureCoordinatesLoaded;
		
		void _tryLoadTexCoords();
		
	private:
		static harray<PropertyDescription> _propertyDescriptions;

		april::TexturedVertex _tVertices[4];
		
	};

}
#endif
