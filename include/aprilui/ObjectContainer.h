/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a generic object container.

#ifndef APRILUI_DUMMY_OBJECT_H
#define APRILUI_DUMMY_OBJECT_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport Container : public Object
	{
	public:
		enum apriluiExport ScrollMode
		{
			Disabled = 0,
			Invisible,
			ActiveOnly,
			Always
		};

		Container(chstr name, grect rect);
		~Container();
		static Object* createInstance(chstr name, grect rect);

		bool getClipping() { return mClipping; }
		void setClipping(bool value) { mClipping = value; }
		ScrollMode getScrollMode() { return mScrollMode; }
		void setScrollMode(ScrollMode value) { mScrollMode = value; }
		
		void update(float k);
		void OnDraw();

		hstr getProperty(chstr name, bool* property_exists);
		bool setProperty(chstr name, chstr value);
		bool onMouseDown(float x, float y, int button);
		bool onMouseUp(float x, float y, int button);
		void onMouseMove(float x, float y);

	protected:
		bool mClipping;
		ScrollMode mScrollMode;

	};
}

#endif
