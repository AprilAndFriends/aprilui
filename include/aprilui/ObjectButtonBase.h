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
/// Defines a base class for all objects with button functionality.

#ifndef APRILUI_BUTTON_BASE_H
#define APRILUI_BUTTON_BASE_H

#include <april/Color.h>
#include <april/Keys.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Cloneable.h"
#include "PropertyDescription.h"

namespace aprilui
{
	class Dataset;
	class Object;

	class apriluiExport ButtonBase
	{
		APRILUI_CLONEABLE_ABSTRACT(ButtonBase);
	public:
		ButtonBase();
		virtual ~ButtonBase();

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GETSET(float, pushDeadZone, PushDeadZone);
		HL_DEFINE_GET(april::Color, hoverColor, HoverColor);
		void setHoverColor(const april::Color& value);
		void setHoverSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, pushedColor, PushedColor);
		void setPushedColor(const april::Color& value);
		void setPushedSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, disabledColor, DisabledColor);
		void setDisabledColor(const april::Color& value);
		void setDisabledSymbolicColor(chstr value);
		HL_DEFINE_IS(hovered, Hovered);
		HL_DEFINE_IS(pushed, Pushed);
		virtual hstr getName() const = 0;
		virtual bool isCursorInside() const = 0;
		virtual int getFocusIndex() const = 0;
		virtual Object* getParent() const = 0;
		virtual Dataset* getDataset() const = 0;

		virtual bool triggerEvent(chstr type, april::Key keyCode) = 0;
		virtual bool triggerEvent(chstr type, april::Key keyCode, chstr string) = 0;
		virtual bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, chstr string, void* userData = NULL) = 0;
		virtual bool triggerEvent(chstr type, void* userData = NULL) = 0;

		static harray<april::Key> allowedKeys;
		static harray<april::Button> allowedButtons;

	protected:
		float pushDeadZone;
		april::Color hoverColor;
		april::Color pushedColor;
		april::Color disabledColor;
		bool hovered;
		bool pushed;
		
		bool _useHoverColor;
		bool _usePushedColor;
		bool _useDisabledColor;

		virtual void _update(float timeDelta);
		virtual aprilui::Object* _findHoverObject();
		void _updateHover();

		virtual bool _mouseDown(april::Key keyCode);
		virtual bool _mouseUp(april::Key keyCode);
		virtual void _mouseCancel(april::Key keyCode);
		virtual bool _mouseMove();
		virtual bool _buttonDown(april::Button buttonCode);
		virtual bool _buttonUp(april::Button buttonCode);

		template <typename T>
		static hmap<hstr, PropertyDescription::Accessor*> _generateGetters()
		{
			hmap<hstr, PropertyDescription::Accessor*> result;
			result["push_dead_zone"] = new PropertyDescription::Get<T, float>(&T::getPushDeadZone);
			result["hover_color"] = new PropertyDescription::GetColor<T>(&T::getHoverColor);
			result["pushed_color"] = new PropertyDescription::GetColor<T>(&T::getPushedColor);
			result["disabled_color"] = new PropertyDescription::GetColor<T>(&T::getDisabledColor);
			return result;
		}

		template <typename T>
		static hmap<hstr, PropertyDescription::Accessor*> _generateSetters()
		{
			hmap<hstr, PropertyDescription::Accessor*> result;
			result["push_dead_zone"] = new PropertyDescription::Set<T, float>(&T::setPushDeadZone);
			result["hover_color"] = new PropertyDescription::Set<T, hstr>(&T::setHoverSymbolicColor);
			result["pushed_color"] = new PropertyDescription::Set<T, hstr>(&T::setPushedSymbolicColor);
			result["disabled_color"] = new PropertyDescription::Set<T, hstr>(&T::setDisabledSymbolicColor);
			return result;
		}

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

		gvec2f _mouseDownPosition;
		Object* _thisObject; // used for internal optimization to avoid dynamic_cast

	};
	
}
#endif
