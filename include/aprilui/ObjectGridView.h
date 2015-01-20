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
/// Defines a class for a grid view.

#ifndef APRILUI_GRID_VIEW_H
#define APRILUI_GRID_VIEW_H

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectSelectionContainer.h"

namespace aprilui
{
	class GridViewCell;
	class GridViewRow;
	class GridViewRowTemplate;

	class apriluiExport GridView : public SelectionContainer
	{
		APRILUI_CLONEABLE(GridView);
	public:
		friend class GridViewCell;
		friend class GridViewRow;
		friend class GridViewRowTemplate;

		GridView(chstr name);
		~GridView();
		inline hstr getClassName() const { return "GridView"; }

		static Object* createInstance(chstr name);

		HL_DEFINE_GET(float, spacingWidth, SpacingWidth);
		void setSpacingWidth(float value);
		HL_DEFINE_GET(float, spacingHeight, SpacingHeight);
		void setSpacingHeight(float value);
		HL_DEFINE_GET(GridViewRowTemplate*, rowTemplate, RowTemplate);
		HL_DEFINE_GET(harray<GridViewRow*>, rows, Rows);
		HL_DEFINE_GET(harray<GridViewCell*>, cells, Cells);
		GridViewCell* getSelected();
		int getRowCount();
		int getCellCount();
		int getItemCount();

		harray<PropertyDescription> getPropertyDescriptions();

		virtual GridViewRow* createRow(int index, chstr name = "");
		bool deleteRow(int index);
		GridViewRow* getRowAt(int index);
		GridViewCell* getItemAt(int index);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		float spacingWidth;
		float spacingHeight;
		GridViewRowTemplate* rowTemplate;
		harray<GridViewRow*> rows;
		harray<GridViewCell*> cells;

		void _updateDisplay();
		void _updateRow(int index);
		void _updateItem(int index);
		void _updateScrollArea();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif