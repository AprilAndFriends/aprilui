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
		APRILUI_CLONEABLE(Container);
	public:
		friend class ScrollArea;
		friend class ScrollBar;
		friend class ScrollBarH;
		friend class ScrollBarV;

		Container(chstr name);
		~Container();
		inline hstr getClassName() const { return "Container"; }

		static Object* createInstance(chstr name);
		
	protected:
		ScrollArea* scrollArea;
		ScrollBarH* scrollBarH;
		ScrollBarV* scrollBarV;

	};
}

#endif
