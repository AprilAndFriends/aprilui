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
		SkinImage(Texture* texture, chstr name, cgrectf source);
		~SkinImage();
		inline hstr getClassName() const { return "SkinImage"; }

		static MinimalImage* createInstance(Texture* texture, chstr name, cgrectf source);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(grectf, skinRect, SkinRect);
		void setSkinRect(cgrectf value);
		HL_DEFINE_GET(float, skinRect.x, SkinX);
		void setSkinX(const float& value);
		HL_DEFINE_GET(float, skinRect.y, SkinY);
		void setSkinY(const float& value);
		HL_DEFINE_GET(float, skinRect.w, SkinWidth);
		void setSkinWidth(const float& value);
		HL_DEFINE_GET(float, skinRect.h, SkinHeight);
		void setSkinHeight(const float& value);
		inline gvec2f getSkinPosition() const { return this->skinRect.getPosition(); }
		void setSkinPosition(cgvec2f value);
		void setSkinPosition(const float& x, const float& y);
		inline gvec2f getSkinSize() const { return this->skinRect.getSize(); }
		void setSkinSize(cgvec2f value);
		void setSkinSize(const float& w, const float& h);
		HL_DEFINE_GET(gvec2f, borderIndent, BorderIndent);
		void setBorderIndent(cgvec2f value);
		void setBorderIndent(const float& x, const float& y);
		HL_DEFINE_GET(float, borderIndent.x, BorderIndentX);
		void setBorderIndentX(const float& value);
		HL_DEFINE_GET(float, borderIndent.y, BorderIndentY);
		void setBorderIndentY(const float& value);
		HL_DEFINE_IS(tiledBorders, TiledBorders);
		void setTiledBorders(const bool& value);

		void draw(cgrectf rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);
		
		static int maxRectCache;

	protected:
		grectf skinRect;
		gvec2f borderIndent;
		bool tiledBorders;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

		typedef std::pair<grectf, harray<april::TexturedVertex> > RectVertices;

		bool _skinCoordinatesCalculated;
		harray<RectVertices> _rectVertices;
		harray<april::TexturedVertex> _vertices;

	};

}
#endif
