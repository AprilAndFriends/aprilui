/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a class for minimal image definitions.

#ifndef APRILUI_MINIMAL_IMAGE_H
#define APRILUI_MINIMAL_IMAGE_H

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
	
	class apriluiExport MinimalImage : public BaseImage
	{
		APRILUI_CLONEABLE(MinimalImage);
	public:
		MinimalImage(Texture* texture, chstr name, cgrectf source);
		inline hstr getClassName() const { return "MinimalImage"; }

		static MinimalImage* createInstance(Texture* texture, chstr name, cgrectf source);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(Texture*, texture, Texture);
		HL_DEFINE_GET(grectf, srcRect, SrcRect);
		void setSrcRect(cgrectf value);
		void setSrcRect(const float& x, const float& y, const float& width, const float& height);
		HL_DEFINE_GET(float, srcRect.x, SrcX);
		void setSrcX(const float& value);
		HL_DEFINE_GET(float, srcRect.y, SrcY);
		void setSrcY(const float& value);
		HL_DEFINE_GET(float, srcRect.w, SrcWidth);
		void setSrcWidth(const float& value);
		HL_DEFINE_GET(float, srcRect.h, SrcHeight);
		void setSrcHeight(const float& value);
		inline gvec2f getSrcPosition() const { return this->srcRect.getPosition(); }
		void setSrcPosition(cgvec2f value);
		void setSrcPosition(const float& x, const float& y);
		inline gvec2f getSrcSize() const { return this->srcRect.getSize(); }
		void setSrcSize(cgvec2f value);
		void setSrcSize(const float& w, const float& h);
		inline const april::TexturedVertex* getVertices() const { return this->vertices; } // use with care!

		hstr getProperty(chstr name);

		void draw(cgrectf rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);

		void tryLoadTextureCoordinates();

	protected:
		Texture* texture;
		grectf srcRect;
		april::TexturedVertex vertices[APRILUI_IMAGE_MAX_VERTICES];
		bool _textureCoordinatesLoaded;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		virtual grectf _makeClippedSrcRect() const;
		
		void _setDeviceTexture();
		
	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
