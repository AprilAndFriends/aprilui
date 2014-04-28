/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.1
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
	class Image;
	
	class apriluiExport ImageBox : public Object
	{
	public:
		ImageBox(chstr name, grect rect);
		~ImageBox();
		static Object* createInstance(chstr name, grect rect);
		
		virtual Image* getImage() { return this->image; }
		virtual void setImage(Image* value);
		virtual HL_DEFINE_GET(hstr, imageName, ImageName);
		virtual void setImageByName(chstr name);
		
		virtual harray<Image*> getUsedImages();

		/// @brief Optimized version.
		virtual bool trySetImageByName(chstr name);

		void resizeToFitImage();

		void notifyEvent(chstr name, void* params);
		hstr getProperty(chstr name, bool* propertyExists = NULL);
		bool setProperty(chstr name, chstr value);

		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		
	protected:
		Image* image;
		hstr imageName;
		
		void OnDraw();
		
	};
}

#endif
