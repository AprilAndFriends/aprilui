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
#include "ObjectButtonBase.h"

namespace aprilui
{
	class apriluiExport ScrollArea : public Object, public ButtonBase
	{
	public:
		ScrollArea(chstr name, grect rect);
		~ScrollArea();
		static Object* createInstance(chstr name, grect rect);

		bool isAllowDrag() { return mAllowDrag; }
		void setAllowDrag(bool value) { mAllowDrag = value; }
		float getDragThreshold() { return mDragThreshold; }
		void setDragThreshold(float value) { mDragThreshold = value; }
		float getDragInertia() { return mDragInertia; }
		void setDragInertia(float value) { mDragInertia = value; }
		bool isDragging() { return mDragging; }
		hstr getName();
		bool isCursorInside();
		Object* getParent();
		Dataset* getDataset();
		bool isScrolling();

		void update(float k);

		void notifyEvent(chstr name, void* params);
		
		hstr getProperty(chstr name, bool* property_exists);
		bool setProperty(chstr name, chstr value);

		bool onMouseDown(float x, float y, int button);
		bool onMouseUp(float x, float y, int button);
		void onMouseMove(float x, float y);

		static float DragThreshold;
		static float DragInertia;

	protected:
		bool mAllowDrag;
		float mDragThreshold;
		float mDragInertia;
		bool mDragging;

		virtual bool _checkHover();

	private:
		gvec2 _mClickPosition;
		gvec2 _mLastPosition;
		gvec2 _mAcceleration;

	};
}

#endif
