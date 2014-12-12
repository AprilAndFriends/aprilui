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
/// Defines a class for a grid view row.

#ifndef APRILUI_GRID_VIEW_ROW_H
#define APRILUI_GRID_VIEW_ROW_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class GridView;

	class apriluiExport GridViewRow : public Container
	{
		APRILUI_CLONEABLE(GridViewRow);
	public:
		friend class GridView;

		GridViewRow(chstr name);
		~GridViewRow();
		inline hstr getClassName() const { return "GridViewRow"; }

		static Object* createInstance(chstr name);

	private:
		GridView* _gridView;

	};

}

#endif
