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
#include "ObjectButtonBase.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class GridView;
	class GridViewRow;

	class apriluiExport GridViewCell : public Container, public ButtonBase
	{
		APRILUI_CLONEABLE(GridViewCell);
	public:
		friend class GridView;
		friend class GridViewRow;

		GridViewCell(chstr name);
		~GridViewCell();
		inline hstr getClassName() const { return "GridViewCell"; }

		static Object* createInstance(chstr name);

		hstr getName();
		int getFocusIndex();
		Object* getParent();
		Dataset* getDataset();
		bool isCursorInside();

		HL_DEFINE_ISSET(selectable, Selectable);
		bool isSelected();

		harray<PropertyDescription> getPropertyDescriptions();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

		void notifyEvent(chstr type, EventArgs* args);

	protected:
		bool selectable;

		void _update(float timeDelta);
		void _draw();

		april::Color _getCurrentBackgroundColor();
		void _setSelected();
		aprilui::Object* _findHoverObject();

		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();
		bool _buttonDown(april::Button buttonCode);
		bool _buttonUp(april::Button buttonCode);

	private:
		GridView* _gridView;
		GridViewRow* _gridViewRow;
		april::Color _hoverColor;
		april::Color _pushedColor;

		static harray<PropertyDescription> _propertyDescriptions;

	};

}

#endif
