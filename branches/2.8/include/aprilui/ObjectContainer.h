/// @file
/// @author  Boris Mikic
/// @version 2.8
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
		ScrollArea* mScrollArea;
		ScrollBarH* mScrollBarH;
		ScrollBarV* mScrollBarV;

		ScrollArea* _getScrollArea() { return this->mScrollArea; }
		void _setScrollArea(ScrollArea* value) { this->mScrollArea = value; }
		ScrollBarH* _getScrollBarH() { return this->mScrollBarH; }
		void _setScrollBarH(ScrollBarH* value) { this->mScrollBarH = value; }
		ScrollBarV* _getScrollBarV() { return this->mScrollBarV; }
		void _setScrollBarV(ScrollBarV* value) { this->mScrollBarV = value; }

	};
}

#endif
