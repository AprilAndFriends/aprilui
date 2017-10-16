/// @file
/// @version 5.0
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
	class Container;

	class apriluiExport ScrollArea : public Object, public ButtonBase
	{
		APRILUI_CLONEABLE(ScrollArea);
	public:
		friend class Container;
		friend class ScrollBar;
		friend class ScrollBarH;
		friend class ScrollBarV;

		ScrollArea(chstr name);
		~ScrollArea();
		inline hstr getClassName() const { return "ScrollArea"; }
		static Object* createInstance(chstr name);

		harray<PropertyDescription> getPropertyDescriptions() const;

		HL_DEFINE_ISSET(allowDrag, AllowDrag);
		HL_DEFINE_ISSET(allowDirectionKeys, AllowDirectionKeys);
		HL_DEFINE_GETSET(float, inertia, Inertia);
		HL_DEFINE_GETSET(float, dragThreshold, DragThreshold);
		HL_DEFINE_GETSET(float, dragMaxSpeed, DragMaxSpeed);
		HL_DEFINE_GETSET(float, directionKeySpeed, DirectionKeySpeed);
		HL_DEFINE_ISSET(swapScrollWheels, SwapScrollWheels);
		HL_DEFINE_ISSET(optimizeOobChildrenVisible, OptimizeOobChildrenVisible);
		HL_DEFINE_ISSET(optimizeOobChildrenAwake, OptimizeOobChildrenAwake);
		HL_DEFINE_IS(dragging, Dragging);
		hstr getName() const;
		bool isCursorInside() const;
		int getFocusIndex() const;
		Object* getParent() const;
		Dataset* getDataset() const;
		bool isScrolling() const;
		bool isDirectionKeyScrolling() const;
		bool isScrollable() const;
		bool isScrollableX() const;
		bool isScrollableY() const;
		gvec2 getScrollOffset() const;
		void setScrollOffset(cgvec2 value);
		void setScrollOffset(float x, float y);
		float getScrollOffsetX() const;
		void setScrollOffsetX(float value);
		float getScrollOffsetY() const;
		void setScrollOffsetY(float value);

		void snapScrollOffset();
		void snapScrollOffsetX();
		void snapScrollOffsetY();
		void stopScrolling();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onMouseScroll(float x, float y);

		static float defaultInertia;
		static float defaultDragThreshold;
		static float defaultDragMaxSpeed;
		static float defaultDirectionKeySpeed;

	protected:
		bool allowDrag;
		bool allowDirectionKeys;
		float inertia;
		float dragThreshold;
		float dragMaxSpeed;
		float directionKeySpeed;
		bool swapScrollWheels;
		bool optimizeOobChildrenVisible;
		bool optimizeOobChildrenAwake;
		bool dragging;
		bool directionKeyScrolling;

		Object* _findHoverObject();
		bool _isScrollableScrollArea(Object* object) const;
		bool _executeScroll(float x, float y, Container* parentContainer);
		void _adjustDragSpeed();

		void _update(float timeDelta);
		void _updateOobChildren();

		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();
		bool _keyDown(april::Key keyCode);
		bool _keyUp(april::Key keyCode);
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
		gvec2 _directionKeySpeed;
		bool _overrideHoverMode;

	};
}

#endif
