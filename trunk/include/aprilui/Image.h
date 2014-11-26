/// @file
/// @version 4.0
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
		inline gvec2 getSrcPosition() { return this->srcRect.getPosition(); }
		void setSrcPosition(gvec2 value);
		void setSrcPosition(float x, float y);
		inline gvec2 getSrcSize() { return this->srcRect.getSize(); }
		void setSrcSize(gvec2 value);
		void setSrcSize(float w, float h);
		HL_DEFINE_GET(grect, clipRect, ClipRect);
		void setClipRect(grect value);
		HL_DEFINE_GET(float, clipRect.x, ClipX);
		void setClipX(float value);
		HL_DEFINE_GET(float, clipRect.y, ClipY);
		void setClipY(float value);
		HL_DEFINE_GET(float, clipRect.w, ClipWidth);
		void setClipWidth(float value);
		HL_DEFINE_GET(float, clipRect.h, ClipHeight);
		void setClipHeight(float value);
		inline gvec2 getClipPosition() { return this->clipRect.getPosition(); }
		void setClipPosition(gvec2 value);
		void setClipPosition(float x, float y);
		inline gvec2 getClipSize() { return this->clipRect.getSize(); }
		void setClipSize(gvec2 value);
		void setClipSize(float w, float h);
		HL_DEFINE_GETSET(april::Color, color, Color);
		HL_DEFINE_ISSET(rotated, Rotated);
		HL_DEFINE_ISSET(invertX, InvertX);
		HL_DEFINE_ISSET(invertY, InvertY);
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_GETSET(april::ColorMode, colorMode, ColorMode);
		HL_DEFINE_GETSET(float, colorModeFactor, ColorModeFactor);
		hstr getFullName();
		inline const april::TexturedVertex* getVertices() const { return this->vertices; } // use with care!

		virtual harray<PropertyDescription> getPropertyDescriptions();

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		void tryLoadTextureCoordinates();

		DEPRECATED_ATTRIBUTE inline bool isVertical() { return this->rotated; }
		DEPRECATED_ATTRIBUTE inline void setVertical(bool value) { this->rotated = value; }
		
	protected:
		Dataset* dataset;
		Texture* texture;
		hstr name;
		hstr imageName;
		grect srcRect;
		grect clipRect;
		april::Color color;
		april::BlendMode blendMode;
		april::ColorMode colorMode;
		float colorModeFactor;
		bool rotated;
		bool invertX;
		bool invertY;

		bool _textureCoordinatesLoaded;
		
		grect _makeClippedSrcRect();
		
	private:
		static harray<PropertyDescription> _propertyDescriptions;

		april::TexturedVertex vertices[4];
		
	};

}
#endif
