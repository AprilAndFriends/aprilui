/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.52
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
		unsigned char getDerivedAlpha();

		bool onMouseDown(int button);
		bool onMouseUp(int button);

		void notifyEvent(chstr name, void* params);
		// TODO - this needs to be seriously refactored
		bool triggerEvent(chstr name, unsigned int keyCode = 0, chstr extra = "");
		// TODO - this needs to be seriously refactored
		bool triggerEvent(chstr name, float x, float y, unsigned int keyCode = 0, chstr extra = "");
		hstr getProperty(chstr name, bool* propertyExists = NULL);
		bool setProperty(chstr name, chstr value);

	protected:
		void OnDraw();
		
	};
	
}

#endif