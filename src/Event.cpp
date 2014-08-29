/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Event.h"

#define TYPE_TO_NAME_CHECK(enumType, name) \
	if (type == enumType) \
	{ \
		return name; \
	}
#define NAME_TO_TYPE_CHECK(type, enumName) \
	if (name == enumName) \
	{ \
		return type; \
	}
#define NAME_TO_TYPE_CHECK_DEPRECATED(type, enumName, properName) \
	if (name == enumName) \
	{ \
		hlog::warnf(aprilui::logTag, "The event '%s' is deprecated. Used '%s' instead."); \
		return type; \
	}

namespace aprilui
{
	Event::Event()
	{
	}

	Event::~Event()
	{
	}

	hstr Event::typeToName(Event::Type type, bool ignoreWarning)
	{
		TYPE_TO_NAME_CHECK(REGISTERED_IN_DATASET, "RegisteredInDataset");
		TYPE_TO_NAME_CHECK(UNREGISTERED_FROM_DATASET, "UnregisteredFromDataset");
		TYPE_TO_NAME_CHECK(ATTACHED_TO_OBJECT, "AttachedToObject");
		TYPE_TO_NAME_CHECK(DETACHED_FROM_OBJECT, "DetachedFromObject");
		TYPE_TO_NAME_CHECK(MOUSE_DOWN, "MouseDown");
		TYPE_TO_NAME_CHECK(MOUSE_UP, "MouseUp");
		TYPE_TO_NAME_CHECK(MOUSE_MOVE, "MouseMove");
		TYPE_TO_NAME_CHECK(MOUSE_SCROLL, "MouseScroll");
		TYPE_TO_NAME_CHECK(MOUSE_CANCEL, "MouseCancel");
		TYPE_TO_NAME_CHECK(CLICK, "Click");
		TYPE_TO_NAME_CHECK(KEY_DOWN, "KeyDown");
		TYPE_TO_NAME_CHECK(KEY_UP, "KeyUp");
		TYPE_TO_NAME_CHECK(BUTTON_DOWN, "ButtonDown");
		TYPE_TO_NAME_CHECK(BUTTON_UP, "ButtonUp");
		TYPE_TO_NAME_CHECK(BUTTON_TRIGGER, "ButtonTrigger");
		TYPE_TO_NAME_CHECK(ENABLED_CHANGED, "EnabledChanged");
		TYPE_TO_NAME_CHECK(RESIZED, "Resized");
		TYPE_TO_NAME_CHECK(FOCUS_GAINED, "FocusGained");
		TYPE_TO_NAME_CHECK(FOCUS_LOST, "FocusLost");
		TYPE_TO_NAME_CHECK(TEXT_CHANGED, "TextChanged");
		TYPE_TO_NAME_CHECK(TEXT_KEY_CHANGED, "TextKeyChanged");
		TYPE_TO_NAME_CHECK(LOCALIZATION_CHANGED, "LocalizationChanged");
		TYPE_TO_NAME_CHECK(SUBMIT_EDIT_TEXT, "SubmitEditText");
		TYPE_TO_NAME_CHECK(UPDATE_IMAGE, "UpdateImage");
		TYPE_TO_NAME_CHECK(SCROLL_SKIN_CHANGED, "ScrollSkinChanged");
		TYPE_TO_NAME_CHECK(SET_SLIDER_VALUE, "SetSliderValue");
		TYPE_TO_NAME_CHECK(DELAY_EXPIRED, "DelayExpired");
		if (!ignoreWarning)
		{
			hlog::warnf(aprilui::logTag, "There is no system event with enum value %d!", (int)type);
		}
		return "";
	}

	Event::Type Event::nameToType(chstr name, bool ignoreWarning)
	{
		NAME_TO_TYPE_CHECK(REGISTERED_IN_DATASET, "RegisteredInDataset");
		NAME_TO_TYPE_CHECK(UNREGISTERED_FROM_DATASET, "UnregisteredFromDataset");
		NAME_TO_TYPE_CHECK(ATTACHED_TO_OBJECT, "AttachedToObject");
		NAME_TO_TYPE_CHECK(DETACHED_FROM_OBJECT, "DetachedFromObject");
		NAME_TO_TYPE_CHECK(MOUSE_DOWN, "MouseDown");
		NAME_TO_TYPE_CHECK(MOUSE_UP, "MouseUp");
		NAME_TO_TYPE_CHECK(MOUSE_MOVE, "MouseMove");
		NAME_TO_TYPE_CHECK(MOUSE_SCROLL, "MouseScroll");
		NAME_TO_TYPE_CHECK(MOUSE_CANCEL, "MouseCancel");
		NAME_TO_TYPE_CHECK(CLICK, "Click");
		NAME_TO_TYPE_CHECK(KEY_DOWN, "KeyDown");
		NAME_TO_TYPE_CHECK(KEY_UP, "KeyUp");
		NAME_TO_TYPE_CHECK(BUTTON_DOWN, "ButtonDown");
		NAME_TO_TYPE_CHECK(BUTTON_UP, "ButtonUp");
		NAME_TO_TYPE_CHECK(BUTTON_TRIGGER, "ButtonTrigger");
		NAME_TO_TYPE_CHECK(ENABLED_CHANGED, "EnabledChanged");
		NAME_TO_TYPE_CHECK(RESIZED, "Resized");
		NAME_TO_TYPE_CHECK(FOCUS_GAINED, "FocusGained");
		NAME_TO_TYPE_CHECK(FOCUS_LOST, "FocusLost");
		NAME_TO_TYPE_CHECK(TEXT_CHANGED, "TextChanged");
		NAME_TO_TYPE_CHECK(TEXT_KEY_CHANGED, "TextKeyChanged");
		NAME_TO_TYPE_CHECK(LOCALIZATION_CHANGED, "LocalizationChanged");
		NAME_TO_TYPE_CHECK(SUBMIT_EDIT_TEXT, "SubmitEditText");
		NAME_TO_TYPE_CHECK(UPDATE_IMAGE, "UpdateImage");
		NAME_TO_TYPE_CHECK(SCROLL_SKIN_CHANGED, "ScrollSkinChanged");
		NAME_TO_TYPE_CHECK(SET_SLIDER_VALUE, "SetSliderValue");
		NAME_TO_TYPE_CHECK(DELAY_EXPIRED, "DelayExpired");
		// DEPRECATED
		NAME_TO_TYPE_CHECK_DEPRECATED(REGISTERED_IN_DATASET, "RegisterInDataset", "RegisteredInDataset");
		NAME_TO_TYPE_CHECK_DEPRECATED(UNREGISTERED_FROM_DATASET, "UnregisterFromDataset", "UnregisteredFromDataset");
		NAME_TO_TYPE_CHECK_DEPRECATED(ATTACHED_TO_OBJECT, "AttachToObject", "AttachedToObject");
		NAME_TO_TYPE_CHECK_DEPRECATED(DETACHED_FROM_OBJECT, "DetachFromObject", "DetachedFromObject");
		NAME_TO_TYPE_CHECK_DEPRECATED(ENABLED_CHANGED, "OnEnableChanged", "EnabledChanged");
		NAME_TO_TYPE_CHECK_DEPRECATED(FOCUS_GAINED, "GainFocus", "FocusGained");
		NAME_TO_TYPE_CHECK_DEPRECATED(FOCUS_LOST, "LoseFocus", "FocusLost");
		NAME_TO_TYPE_CHECK_DEPRECATED(TEXT_CHANGED, "onTextChanged", "TextChanged");
		NAME_TO_TYPE_CHECK_DEPRECATED(TEXT_KEY_CHANGED, "onTextKeyChanged", "TextKeyChanged");
		NAME_TO_TYPE_CHECK_DEPRECATED(LOCALIZATION_CHANGED, "onLocalizationChanged", "LocalizationChanged");
		NAME_TO_TYPE_CHECK_DEPRECATED(SUBMIT_EDIT_TEXT, "Submit", "SubmitEditText");
		NAME_TO_TYPE_CHECK_DEPRECATED(SCROLL_SKIN_CHANGED, "SkinChange", "ScrollSkinChanged");
		NAME_TO_TYPE_CHECK_DEPRECATED(SET_SLIDER_VALUE, "Set", "SetSliderValue");
		NAME_TO_TYPE_CHECK_DEPRECATED(DELAY_EXPIRED, "OnDelayEnd", "DelayExpired");
		if (!ignoreWarning)
		{
			hlog::warnf(aprilui::logTag, "There is no system event the name %s!", name.c_str());
		}
		return UNDEFINED;
	}

}
