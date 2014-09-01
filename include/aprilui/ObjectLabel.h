/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a text label.

#ifndef APRILUI_LABEL_H
#define APRILUI_LABEL_H

#include <april/Keys.h>
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
		hstr getClassName() const { return "Label"; }

		static Object* createInstance(chstr name, grect rect);
		
		Dataset* getDataset();
		unsigned char getDerivedAlpha();

		harray<PropertyDescription> getPropertyDescriptions();

		void notifyEvent(chstr type, EventArgs* args);
		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);

	protected:
		void OnDraw();
		
	};
	
}

#endif
