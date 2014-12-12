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
/// Defines a class for a grid view cell.

#ifndef APRILUI_GRID_VIEW_CELL_H
#define APRILUI_GRID_VIEW_CELL_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class GridView;

	class apriluiExport GridViewCell : public Container
	{
		APRILUI_CLONEABLE(GridViewCell);
	public:
		friend class GridView;

		GridViewCell(chstr name);
		~GridViewCell();
		inline hstr getClassName() const { return "GridViewCell"; }

		static Object* createInstance(chstr name);

	private:
		GridView* _gridView;

	};

}

#endif
