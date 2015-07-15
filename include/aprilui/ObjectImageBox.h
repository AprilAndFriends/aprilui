/// @file
/// @version 4.1
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
	
	class apriluiExport ImageBox : public Object
	{
		APRILUI_CLONEABLE(ImageBox);
	public:
		ImageBox(chstr name);
		~ImageBox();
		inline hstr getClassName() const { return "ImageBox"; }

		static Object* createInstance(chstr name);
		
		harray<PropertyDescription> getPropertyDescriptions();

		virtual HL_DEFINE_GET(BaseImage*, image, Image);
		virtual void setImage(BaseImage* value);
		virtual HL_DEFINE_GET(hstr, imageName, ImageName);
		virtual void setImageByName(chstr name);
		
		/// @brief Optimized version.
		virtual bool trySetImageByName(chstr name);

		void resizeToFitImage();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		BaseImage* image;
		hstr imageName;
		
		harray<BaseImage*> _getUsedImages();

		void _draw();
		
	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif
