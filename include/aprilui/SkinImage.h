/// @file
/// @version 4.3
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
		SkinImage(Texture* texture, chstr name, cgrect source);
		~SkinImage();
		inline hstr getClassName() const { return "SkinImage"; }

		static Image* createInstance(Texture* texture, chstr name, cgrect source);

		HL_DEFINE_GET(grect, skinRect, SkinRect);
		void setSkinRect(cgrect value);
		HL_DEFINE_GET(float, skinRect.x, SkinX);
		void setSkinX(float value);
		HL_DEFINE_GET(float, skinRect.y, SkinY);
		void setSkinY(float value);
		HL_DEFINE_GET(float, skinRect.w, SkinWidth);
		void setSkinWidth(float value);
		HL_DEFINE_GET(float, skinRect.h, SkinHeight);
		void setSkinHeight(float value);
		inline gvec2 getSkinPosition() const { return this->skinRect.getPosition(); }
		void setSkinPosition(cgvec2 value);
		void setSkinPosition(float x, float y);
		inline gvec2 getSkinSize() const { return this->skinRect.getSize(); }
		void setSkinSize(cgvec2 value);
		void setSkinSize(float w, float h);
		HL_DEFINE_GET(gvec2, borderIndent, BorderIndent);
		void setBorderIndent(cgvec2 value);
		void setBorderIndent(float x, float y);
		HL_DEFINE_GET(float, borderIndent.x, BorderIndentX);
		void setBorderIndentX(float value);
		HL_DEFINE_GET(float, borderIndent.y, BorderIndentY);
		void setBorderIndentY(float value);
		HL_DEFINE_IS(tiledBorders, TiledBorders);
		void setTiledBorders(bool value);

		void draw(cgrect rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);
		
		harray<PropertyDescription> getPropertyDescriptions() const;

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		static int maxRectCache;

	protected:
		grect skinRect;
		gvec2 borderIndent;
		bool tiledBorders;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		typedef std::pair<grect, harray<april::TexturedVertex> > RectVertices;

		bool _skinCoordinatesCalculated;
		harray<RectVertices> _rectVertices;
		harray<april::TexturedVertex> _vertices;

	};

}
#endif
