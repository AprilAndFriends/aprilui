/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a scrollable area.

#ifndef APRILUI_SCROLL_AREA_H
#define APRILUI_SCROLL_AREA_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport ScrollArea : public Object
	{
	public:
		ScrollArea(chstr name, grect rect);
		~ScrollArea();
		static Object* createInstance(chstr name, grect rect);

		bool getAllowTouch() { return mAllowTouch; }
		void setAllowTouch(bool value) { mAllowTouch = value; }

		void update(float k);
		void OnDraw();

		void notifyEvent(chstr name, void* params);
		
		hstr getProperty(chstr name, bool* property_exists);
		bool setProperty(chstr name, chstr value);

		bool onMouseDown(float x, float y, int button);
		bool onMouseUp(float x, float y, int button);
		void onMouseMove(float x, float y);

	protected:
		bool mAllowTouch;

	private:
		gvec2 _mLastPosition;

	};
}

#endif