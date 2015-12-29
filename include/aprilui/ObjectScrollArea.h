/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
		APRILUI_CLONEABLE(ScrollArea);
	public:
		friend class ScrollBar;
		friend class ScrollBarH;
		friend class ScrollBarV;

		ScrollArea(chstr name);
		~ScrollArea();
		inline hstr getClassName() const { return "ScrollArea"; }

		static Object* createInstance(chstr name);

		HL_DEFINE_ISSET(allowDrag, AllowDrag);
		HL_DEFINE_GETSET(float, inertia, Inertia);
		HL_DEFINE_GETSET(float, dragThreshold, DragThreshold);
		HL_DEFINE_GETSET(float, dragMaxSpeed, DragMaxSpeed);
		HL_DEFINE_ISSET(swapScrollWheels, SwapScrollWheels);
		HL_DEFINE_IS(dragging, Dragging);
		hstr getName();
		bool isCursorInside();
		int getFocusIndex();
		Object* getParent();
		Dataset* getDataset();
		bool isScrolling();		
		bool isScrollable();
		bool isScrollableX();
		bool isScrollableY();
		
		gvec2 getScrollOffset();
		void setScrollOffset(gvec2 value);
		void setScrollOffset(float x, float y);
		float getScrollOffsetX();
		void setScrollOffsetX(float value);
		float getScrollOffsetY();
		void setScrollOffsetY(float value);

		harray<PropertyDescription> getPropertyDescriptions();

		void snapScrollOffset();
		void stopScrolling();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onMouseScroll(float x, float y);

		static float defaultInertia;
		static float defaultDragThreshold;
		static float defaultDragMaxSpeed;

		DEPRECATED_ATTRIBUTE void __stop() { return this->stopScrolling(); }

	protected:
		bool allowDrag;
		float inertia;
		float dragThreshold;
		float dragMaxSpeed;
		bool swapScrollWheels;
		bool dragging;

		aprilui::Object* _findHoverObject();
		void _adjustDragSpeed();
		bool _isScrollableScrollArea(aprilui::Object* object);

		void _update(float timeDelta);

		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();
		bool _buttonDown(april::Button buttonCode);
		bool _buttonUp(april::Button buttonCode);

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		gvec2 _clickPosition;
		gvec2 _lastPosition;
		gvec2 _clickScrollOffset;
		gvec2 _lastScrollOffset;
		gvec2 _dragSpeed;
		gvec2 _dragTimer;
		bool _overrideHoverMode;

	};
}

#endif
