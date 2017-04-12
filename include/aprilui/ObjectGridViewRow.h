/// @file
/// @version 4.2
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
	class GridViewCell;
	class GridViewRowTemplate;

	class apriluiExport GridViewRow : public Container
	{
		APRILUI_CLONEABLE(GridViewRow);
	public:
		friend class GridView;
		friend class GridViewCell;
		friend class GridViewRowTemplate;

		GridViewRow(chstr name);
		~GridViewRow();
		inline hstr getClassName() const { return "GridViewRow"; }

		static Object* createInstance(chstr name);

		HL_DEFINE_GET(harray<GridViewCell*>, _gridViewCells, Cells);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		GridView* _gridView;
		harray<GridViewCell*> _gridViewCells;

	};

}

#endif
