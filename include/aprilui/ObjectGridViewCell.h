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

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		hstr getName() const;
		int getFocusIndex() const;
		Object* getParent() const;
		Dataset* getDataset() const;
		bool isCursorInside() const;

		HL_DEFINE_ISSET(selectable, Selectable);
		bool isSelected() const;

		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

	protected:
		bool selectable;
		GridView* gridView;
		GridViewRow* gridViewRow;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		void _update(float timeDelta);
		void _draw();

		april::Color _getCurrentBackgroundColor() const;
		void _setSelected();
		aprilui::Object* _findHoverObject();

		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();
		bool _buttonDown(april::Button buttonCode);
		bool _buttonUp(april::Button buttonCode);

	private:
		april::Color _hoverColor;
		april::Color _pushedColor;

		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
