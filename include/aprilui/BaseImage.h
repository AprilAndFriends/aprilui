/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for all image classes.

#ifndef APRILUI_BASE_IMAGE_H
#define APRILUI_BASE_IMAGE_H

#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Cloneable.h"
#include "PropertyDescription.h"

namespace aprilui
{
	class Dataset;
	class Texture;

	class apriluiExport BaseImage : public Cloneable
	{
		APRILUI_CLONEABLE_ABSTRACT(BaseImage);
	public:
		friend class Dataset;

		BaseImage(chstr name);
		virtual inline hstr getClassName() const { return "BaseImage"; }
		
		virtual hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(hstr, name, Name);
		void setName(chstr value);
		HL_DEFINE_GETSET(hstr, tag, Tag);
		HL_DEFINE_GET(Dataset*, dataset, Dataset);
		HL_DEFINE_GET(grectf, clipRect, ClipRect);
		void setClipRect(cgrectf value);
		HL_DEFINE_GET(float, clipRect.x, ClipX);
		void setClipX(const float& value);
		HL_DEFINE_GET(float, clipRect.y, ClipY);
		void setClipY(const float& value);
		HL_DEFINE_GET(float, clipRect.w, ClipWidth);
		void setClipWidth(const float& value);
		HL_DEFINE_GET(float, clipRect.h, ClipHeight);
		void setClipHeight(const float& value);
		inline gvec2f getClipPosition() const { return this->clipRect.getPosition(); }
		void setClipPosition(cgvec2f value);
		void setClipPosition(const float& x, const float& y);
		inline gvec2f getClipSize() const { return this->clipRect.getSize(); }
		void setClipSize(cgvec2f value);
		void setClipSize(const float& w, const float& h);
		hstr getFullName() const;

		virtual gvec2f getSrcSize() const = 0;
		virtual void setSrcSize(cgvec2f value) = 0;
		virtual float getSrcWidth() const = 0;
		virtual void setSrcWidth(const float& value) = 0;
		virtual float getSrcHeight() const = 0;
		virtual void setSrcHeight(const float& value) = 0;

		bool hasProperty(chstr name);

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		virtual void draw(cgrectf rect, const april::Color& color = april::Color::White) = 0;
		virtual void draw(const harray<april::TexturedVertex>& vertices, const april::Color& color = april::Color::White) = 0;

		static harray<Texture*> findTextures(harray<BaseImage*> baseImages);

	protected:
		hstr name;
		hstr tag;
		Dataset* dataset;
		grectf clipRect;

		bool _clipRectCalculated;

		virtual hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		virtual hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
