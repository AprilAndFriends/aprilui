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
/// Defines a class for crop image definitions.

#ifndef APRILUI_CROP_IMAGE_H
#define APRILUI_CROP_IMAGE_H

#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Image.h"
#include "PropertyDescription.h"

namespace aprilui
{
	class Texture;
}

namespace aprilui
{
	class apriluiExport CropImage : public Image
	{
		APRILUI_CLONEABLE(CropImage);
	public:
		CropImage(Texture* texture, chstr name, cgrectf source);
		~CropImage();
		inline hstr getClassName() const { return "CropImage"; }

		static MinimalImage* createInstance(Texture* texture, chstr name, cgrectf source);

		HL_DEFINE_GETSET(gvec2f, drawPosition, DrawPosition);
		inline void setDrawPosition(const float& x, const float& y) { this->drawPosition.set(x, y); }
		HL_DEFINE_GETSET(float, drawPosition.x, DrawX);
		HL_DEFINE_GETSET(float, drawPosition.y, DrawY);
		HL_DEFINE_GET(gvec2f, realSrcSize, RealSrcSize);
		void setRealSrcSize(cgvec2f value);
		void setRealSrcSize(const float& w, const float& h);
		HL_DEFINE_GET(float, realSrcSize.x, RealSrcWidth);
		void setRealSrcWidth(float const& value);
		HL_DEFINE_GET(float, realSrcSize.y, RealSrcHeight);
		void setRealSrcHeight(float const& value);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		void draw(cgrectf rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);

	protected:
		gvec2f drawPosition;
		gvec2f realSrcSize;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		grectf _makeRealSrcRect() const;
		grectf _makeClippedSrcRect() const;
		
	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
