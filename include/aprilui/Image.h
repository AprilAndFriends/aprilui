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
		Image(Texture* texture, chstr name, cgrect source);
		~Image();
		inline hstr getClassName() const { return "Image"; }

		static MinimalImage* createInstance(Texture* texture, chstr name, cgrect source);

		HL_DEFINE_GETSET(april::Color, color, Color);
		void setSymbolicColor(chstr value);
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_GETSET(april::ColorMode, colorMode, ColorMode);
		HL_DEFINE_GETSET(float, colorModeFactor, ColorModeFactor);

		harray<PropertyDescription> getPropertyDescriptions() const;

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void draw(cgrect rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);

	protected:
		april::Color color;
		april::BlendMode blendMode;
		april::ColorMode colorMode;
		float colorModeFactor;
		
	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

}
#endif
