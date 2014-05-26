/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a generic object container.

#ifndef APRILUI_CONTAINER_H
#define APRILUI_CONTAINER_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class ScrollArea;
	class ScrollBarH;
	class ScrollBarV;

	class apriluiExport Container : public Object
	{
	public:
		friend class ScrollArea;
		friend class ScrollBar;
		friend class ScrollBarH;
		friend class ScrollBarV;

		Container(chstr name, grect rect);
		~Container();
		static Object* createInstance(chstr name, grect rect);
		
		void OnDraw();

	protected:
		ScrollArea* scrollArea;
		ScrollBarH* scrollBarH;
		ScrollBarV* scrollBarV;

		inline ScrollArea* _getScrollArea() { return this->scrollArea; }
		inline void _setScrollArea(ScrollArea* value) { this->scrollArea = value; }
		inline ScrollBarH* _getScrollBarH() { return this->scrollBarH; }
		inline void _setScrollBarH(ScrollBarH* value) { this->scrollBarH = value; }
		inline ScrollBarV* _getScrollBarV() { return this->scrollBarV; }
		inline void _setScrollBarV(ScrollBarV* value) { this->scrollBarV = value; }

	};
}

#endif
