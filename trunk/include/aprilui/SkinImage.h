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
/// Defines an image for skinning.

#ifndef APRILUI_SKIN_IMAGE_H
#define APRILUI_SKIN_IMAGE_H

#include "Image.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport SkinImage : public Image
	{
		APRILUI_CLONEABLE(SkinImage);
	public:
		SkinImage(Texture* texture, chstr name, grect source);
		~SkinImage();

		HL_DEFINE_GET(grect, skinRect, SkinRect);
		void setSkinRect(grect value);
		HL_DEFINE_GET(float, skinRect.x, SkinX);
		void setSkinX(float value);
		HL_DEFINE_GET(float, skinRect.y, SkinY);
		void setSkinY(float value);
		HL_DEFINE_GET(float, skinRect.w, SkinWidth);
		void setSkinWidth(float value);
		HL_DEFINE_GET(float, skinRect.h, SkinHeight);
		void setSkinHeight(float value);
		inline gvec2 getSkinPosition() { return this->skinRect.getPosition(); }
		void setSkinPosition(gvec2 value);
		void setSkinPosition(float x, float y);
		inline gvec2 getSkinSize() { return this->skinRect.getSize(); }
		void setSkinSize(gvec2 value);
		void setSkinSize(float w, float h);

		void draw(grect rect, april::Color color);
		void draw(harray<april::TexturedVertex> vertices, april::Color color);
		
		harray<PropertyDescription> getPropertyDescriptions();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		grect skinRect;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		grect _lastDrawRect;
		bool _skinCoordinatesCalculated;
		harray<april::TexturedVertex> _vertices;

	};

}
#endif
