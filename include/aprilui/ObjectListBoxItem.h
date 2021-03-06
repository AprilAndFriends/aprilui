/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a class for a list box item.

#ifndef APRILUI_LIST_BOX_ITEM_H
#define APRILUI_LIST_BOX_ITEM_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectButtonBase.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class ListBox;

	class apriluiExport ListBoxItem : public Label, public ButtonBase
	{
		APRILUI_CLONEABLE(ListBoxItem);
	public:
		friend class ListBox;

		ListBoxItem(chstr name);
		inline hstr getClassName() const { return "ListBoxItem"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		hstr getName() const;
		int getFocusIndex() const;
		Object* getParent() const;
		Dataset* getDataset() const;
		bool isCursorInside() const;
		bool isSelected() const;

		bool isPointInside(cgvec2f position) const;

		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

	protected:
		void _update(float timeDelta);
		void _draw();

		april::Color _getCurrentBackgroundColor() const;
		void _setSelected();

		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();
		bool _touchDown(int index);
		bool _touchUp(int index);
		void _touchCancel(int index);
		bool _touchMove(int index);
		bool _buttonDown(april::Button buttonCode);
		bool _buttonUp(april::Button buttonCode);

	private:
		ListBox* _listBox;
		april::Color _backColor;
		april::Color _hoverColor;
		april::Color _pushedColor;

		static hmap<hstr, PropertyDescription> _propertyDescriptions;

	};

}
#endif
