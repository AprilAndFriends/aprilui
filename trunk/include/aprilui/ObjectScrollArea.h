/// @file
/// @author  Boris Mikic
/// @version 1.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines a special container that can be scrolled.

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
		friend class ScrollBar;
		friend class ScrollBarH;
		friend class ScrollBarV;

		ScrollArea(chstr name, grect rect);
		~ScrollArea();
		static Object* createInstance(chstr name, grect rect);

		bool isAllowDrag() { return mAllowDrag; }
		void setAllowDrag(bool value) { mAllowDrag = value; }
		float getInertia() { return mInertia; }
		void setInertia(float value) { mInertia = value; }
		float getDragThreshold() { return mDragThreshold; }
		void setDragThreshold(float value) { mDragThreshold = value; }
		float getDragMaxSpeed() { return mDragMaxSpeed; }
		void setDragMaxSpeed(float value) { mDragMaxSpeed = value; }
		bool isSwapScrollWheels() { return mSwapScrollWheels; }
		void setSwapScrollWheels(bool value) { mSwapScrollWheels = value; }
		bool isDragging() { return mDragging; }
		hstr getName();
		bool isCursorInside();
		Object* getParent();
		Dataset* getDataset();
		bool isScrolling();
		gvec2 getScrollOffset();
		void setScrollOffset(gvec2 value);
		void setScrollOffset(float x, float y);
		float getScrollOffsetX();
		void setScrollOffsetX(float value);
		float getScrollOffsetY();
		void setScrollOffsetY(float value);

		void update(float k);

		void notifyEvent(chstr name, void* params);
		
		hstr getProperty(chstr name, bool* property_exists);
		bool setProperty(chstr name, chstr value);

		bool onMouseDown(int button);
		bool onMouseUp(int button);
		void onMouseMove();

		static float Inertia;
		static float DragThreshold;
		static float DragMaxSpeed;

	protected:
		bool mAllowDrag;
		float mInertia;
		float mDragThreshold;
		float mDragMaxSpeed;
		bool mSwapScrollWheels;
		bool mDragging;

		virtual bool _checkHover();
		void _adjustDragSpeed();
		void _snapScrollOffset();

	private:
		gvec2 _mClickPosition;
		gvec2 _mLastPosition;
		gvec2 _mLastScrollOffset;
		gvec2 _mDragSpeed;
		gvec2 _mDragTimer;

	};
}

#endif
