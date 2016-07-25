/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		
		virtual Image* getImage() { return mImage; };
		virtual void setImage(Image* value);
		hstr getImageName() { return mImageName; }
		void setImageByName(chstr name);
		void notifyEvent(chstr name, void* params);
		
		// compatibility methods
		void trySetImageByName(chstr image) { setImageByName(image); }

		void resizeToFitImage();
		
		bool setProperty(chstr name, chstr value);
		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		
	protected:
		Image* mImage;
		hstr mImageName;
		
		void OnDraw(gvec2 offset = gvec2());
		
	};
}

#endif
