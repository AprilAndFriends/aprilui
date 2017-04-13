/// @file
/// @version 4.2
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
		~BaseImage();
		virtual inline hstr getClassName() const { return "BaseObject"; }
		
		HL_DEFINE_GET(hstr, name, Name);
		void setName(chstr value);
		HL_DEFINE_GETSET(hstr, tag, Tag);
		HL_DEFINE_GET(Dataset*, dataset, Dataset);
		HL_DEFINE_GET(grect, clipRect, ClipRect);
		void setClipRect(cgrect value);
		HL_DEFINE_GET(float, clipRect.x, ClipX);
		void setClipX(float value);
		HL_DEFINE_GET(float, clipRect.y, ClipY);
		void setClipY(float value);
		HL_DEFINE_GET(float, clipRect.w, ClipWidth);
		void setClipWidth(float value);
		HL_DEFINE_GET(float, clipRect.h, ClipHeight);
		void setClipHeight(float value);
		inline gvec2 getClipPosition() const { return this->clipRect.getPosition(); }
		void setClipPosition(cgvec2 value);
		void setClipPosition(float x, float y);
		inline gvec2 getClipSize() const { return this->clipRect.getSize(); }
		void setClipSize(cgvec2 value);
		void setClipSize(float w, float h);
		hstr getFullName() const;

		virtual gvec2 getSrcSize() const = 0;
		virtual void setSrcSize(cgvec2 value) = 0;
		virtual float getSrcWidth() const = 0;
		virtual void setSrcWidth(const float& value) = 0;
		virtual float getSrcHeight() const = 0;
		virtual void setSrcHeight(const float& value) = 0;

		virtual harray<PropertyDescription> getPropertyDescriptions() const;

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		virtual void draw(cgrect rect, april::Color color = april::Color::White) = 0;
		virtual void draw(const harray<april::TexturedVertex>& vertices, april::Color color = april::Color::White) = 0;

		static harray<Texture*> findTextures(harray<BaseImage*> baseImages);

	protected:
		hstr name;
		hstr tag;
		Dataset* dataset;
		grect clipRect;

		bool _textureCoordinatesLoaded;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};

}
#endif
