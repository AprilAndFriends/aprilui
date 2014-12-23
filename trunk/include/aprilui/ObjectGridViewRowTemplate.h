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
/// Defines a class for a grid view row template.

#ifndef APRILUI_GRID_VIEW_ROW_TEMPLATE_H
#define APRILUI_GRID_VIEW_ROW_TEMPLATE_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectGridViewRow.h"

namespace aprilui
{
	class GridView;
	class GridViewCell;

	class apriluiExport GridViewRowTemplate : public GridViewRow
	{
		APRILUI_CLONEABLE(GridViewRowTemplate);
	public:
		friend class GridView;
		friend class GridViewCell;

		GridViewRowTemplate(chstr name);
		~GridViewRowTemplate();
		inline hstr getClassName() const { return "GridViewRowTemplate"; }

		static Object* createInstance(chstr name);

		void update(float timeDelta);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		void _draw();

		GridViewRow* _createRow(chstr name);

	};

}

#endif
