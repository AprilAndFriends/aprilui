/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.05
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
		
		virtual Image* getImage() { return this->mImage; };
		virtual void setImage(Image* value);
		hstr getImageName() { return this->mImageName; }
		void setImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetImageByName(chstr name);

		void notifyEvent(chstr name, void* params);
		void resizeToFitImage();
		
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

		bool onMouseDown(int button);
		bool onMouseUp(int button);
		
		DEPRECATED_ATTRIBUTE bool onMouseDown(float x, float y, int button) { return this->onMouseDown(button); }
		DEPRECATED_ATTRIBUTE bool onMouseUp(float x, float y, int button) { return this->onMouseUp(button); }
		
	protected:
		Image* mImage;
		hstr mImageName;
		
		void OnDraw();
		
	};
}

#endif
