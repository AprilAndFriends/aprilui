/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.91
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a text label.

#ifndef APRILUI_LABEL_H
#define APRILUI_LABEL_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectLabelBase.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport Label : public Object, public LabelBase
	{
	public:
		Label(chstr name, grect rect);
		~Label();
		static Object* createInstance(chstr name, grect rect);
		
		Dataset* getDataset();

		bool onMouseDown(int button);
		bool onMouseUp(int button);

		void notifyEvent(chstr name, void* params);
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
		DEPRECATED_ATTRIBUTE bool onMouseDown(float x, float y, int button) { return this->onMouseDown(button); }
		DEPRECATED_ATTRIBUTE bool onMouseUp(float x, float y, int button) { return this->onMouseUp(button); }

	protected:
		void OnDraw();
		
	};
	
}

#endif
