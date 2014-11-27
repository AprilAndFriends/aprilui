/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a base class for all objects with button functionality.

#ifndef APRILUI_BUTTON_BASE_H
#define APRILUI_BUTTON_BASE_H

#include <april/Keys.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;
	class Object;

	class apriluiExport ButtonBase
	{
	public:
		ButtonBase();
		virtual ~ButtonBase();

		HL_DEFINE_IS(hovered, Hovered);
		HL_DEFINE_IS(pushed, Pushed);
		virtual hstr getName() = 0;
		virtual bool isCursorInside() = 0;
		virtual int getFocusIndex() = 0;
		virtual Object* getParent() = 0;
		virtual Dataset* getDataset() = 0;

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

		virtual bool triggerEvent(chstr type, april::Key keyCode) = 0;
		virtual bool triggerEvent(chstr type, april::Key keyCode, chstr string) = 0;
		virtual bool triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, chstr string, void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, void* userData = NULL) = 0;

		static void setAllowedKeys(harray<april::Key> keys);
		static void setAllowedButtons(harray<april::Button> buttons);
		
	protected:
		bool hovered;
		bool pushed;
		
		virtual void update(float timeDelta);
		virtual aprilui::Object* _findHoverObject();
		void _updateHover();

		virtual bool _mouseDown(april::Key keyCode);
		virtual bool _mouseUp(april::Key keyCode);
		virtual void _mouseCancel(april::Key keyCode);
		virtual bool _mouseMove();
		virtual bool _buttonDown(april::Button buttonCode);
		virtual bool _buttonUp(april::Button buttonCode);

		DEPRECATED_ATTRIBUTE virtual bool _checkHover() { return (this->_findHoverObject() != NULL); }

	private:
		Object* _thisObject; // used for internal optimization to avoid dynamic_cast

	};
	
}

#endif
