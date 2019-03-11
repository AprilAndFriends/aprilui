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
/// Defines an image box.

#ifndef APRILUI_IMAGE_BOX_H
#define APRILUI_IMAGE_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class BaseImage;
	class TileImage;
	
	class apriluiExport ImageBox : public Object
	{
		APRILUI_CLONEABLE(ImageBox);
	public:
		ImageBox(chstr name);
		inline hstr getClassName() const { return "ImageBox"; }
		static Object* createInstance(chstr name);
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GETSET(gvec2f, tileScroll, TileScroll);
		HL_DEFINE_GETSET(float, tileScroll.x, TileScrollX);
		HL_DEFINE_GETSET(float, tileScroll.y, TileScrollY);

		virtual inline BaseImage* getImage() const { return this->image; }
		virtual void setImage(BaseImage* value);
		virtual inline hstr getImageName() const { return this->imageName; }
		virtual void setImageByName(chstr name);
		
		/// @brief Optimized version.
		virtual bool trySetImageByName(chstr name);

		void resizeToFitImage();

	protected:
		BaseImage* image;
		TileImage* tileImage;
		gvec2f tileScroll;
		hstr imageName;
		
		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		harray<BaseImage*> _getUsedImages() const;

		void _draw();
		
	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
