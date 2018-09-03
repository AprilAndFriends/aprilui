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
/// Defines a special image that is composed of other images.

#ifndef APRILUI_COMPOSITE_IMAGE_H
#define APRILUI_COMPOSITE_IMAGE_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>

#include "BaseImage.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport CompositeImage : public BaseImage
	{
		APRILUI_CLONEABLE(CompositeImage);
	public:
		typedef std::pair<BaseImage*, grectf> ImageRef;

		CompositeImage(chstr name, cgvec2f size);
		inline hstr getClassName() const { return "CompositeImage"; }
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GETSET(gvec2f, size, SrcSize);
		HL_DEFINE_GETSET(float, size.x, SrcWidth);
		HL_DEFINE_GETSET(float, size.y, SrcHeight);
		HL_DEFINE_ISSET(restoreClipRects, RestoreClipRects);
		inline const harray<ImageRef>& getImages() { return this->images; }

		void addImageRef(BaseImage* image, cgrectf rect);
		void clearImages();
		
		void draw(cgrectf rect, const april::Color& color = april::Color::White);
		void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White);
		
	protected:
		gvec2f size;
		bool restoreClipRects;
		harray<ImageRef> images;
		
		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};
	
}
#endif
