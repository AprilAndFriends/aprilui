/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Event.h"

#define SYSTEM_EVENT_CHECK(checkType) \
	if (type == checkType || !caseSensitive && type.lower() == checkType.lower()) \
	{ \
		return true; \
	}

namespace aprilui
{
	hstr Event::RegisteredInDataset = "RegisteredInDataset";
	hstr Event::UnregisteredFromDataset = "UnregisteredFromDataset";
	hstr Event::AttachedToObject = "AttachedToObject";
	hstr Event::DetachedFromObject = "DetachedFromObject";
	hstr Event::MouseDown = "MouseDown";
	hstr Event::MouseUp = "MouseUp";
	hstr Event::MouseMove = "MouseMove";
	hstr Event::MouseScroll = "MouseScroll";
	hstr Event::MouseCancel = "MouseCancel";
	hstr Event::Click = "Click";
	hstr Event::KeyDown = "KeyDown";
	hstr Event::KeyUp = "KeyUp";
	hstr Event::ButtonDown = "ButtonDown";
	hstr Event::ButtonUp = "ButtonUp";
	hstr Event::ButtonTrigger = "ButtonTrigger";
	hstr Event::EnabledChanged = "EnabledChanged";
	hstr Event::Resized = "Resized";
	hstr Event::FocusGained = "FocusGained";
	hstr Event::FocusLost = "FocusLost";
	hstr Event::HoverStarted = "HoverStarted";
	hstr Event::HoverFinished = "HoverFinished";
	hstr Event::TextChanged = "TextChanged";
	hstr Event::TextKeyChanged = "TextKeyChanged";
	hstr Event::LocalizationChanged = "LocalizationChanged";
	hstr Event::SubmitEditText = "SubmitEditText";
	hstr Event::ScrollSkinChanged = "ScrollSkinChanged";
	hstr Event::SetProgressValue = "SetProgressValue";
	hstr Event::SelectedChanged = "SelectedChanged";
	hstr Event::NodeExpanded = "NodeExpanded";
	hstr Event::NodeCollapsed = "NodeCollapsed";
	hstr Event::AnimationDelayExpired = "AnimationDelayExpired";
	hstr Event::AnimationExpired = "AnimationExpired";

	Event::Event() : Cloneable()
	{
	}

	Event::Event(const Event& other) : Cloneable()
	{
	}

	Event::~Event()
	{
	}

	bool Event::isSystemEvent(chstr type, bool caseSensitive)
	{
		SYSTEM_EVENT_CHECK(RegisteredInDataset);
		SYSTEM_EVENT_CHECK(UnregisteredFromDataset);
		SYSTEM_EVENT_CHECK(AttachedToObject);
		SYSTEM_EVENT_CHECK(DetachedFromObject);
		SYSTEM_EVENT_CHECK(MouseDown);
		SYSTEM_EVENT_CHECK(MouseUp);
		SYSTEM_EVENT_CHECK(MouseMove);
		SYSTEM_EVENT_CHECK(MouseScroll);
		SYSTEM_EVENT_CHECK(MouseCancel);
		SYSTEM_EVENT_CHECK(Click);
		SYSTEM_EVENT_CHECK(KeyDown);
		SYSTEM_EVENT_CHECK(KeyUp);
		SYSTEM_EVENT_CHECK(ButtonDown);
		SYSTEM_EVENT_CHECK(ButtonUp);
		SYSTEM_EVENT_CHECK(ButtonTrigger);
		SYSTEM_EVENT_CHECK(EnabledChanged);
		SYSTEM_EVENT_CHECK(Resized);
		SYSTEM_EVENT_CHECK(FocusGained);
		SYSTEM_EVENT_CHECK(FocusLost);
		SYSTEM_EVENT_CHECK(HoverStarted);
		SYSTEM_EVENT_CHECK(HoverFinished);
		SYSTEM_EVENT_CHECK(TextChanged);
		SYSTEM_EVENT_CHECK(TextKeyChanged);
		SYSTEM_EVENT_CHECK(LocalizationChanged);
		SYSTEM_EVENT_CHECK(SubmitEditText);
		SYSTEM_EVENT_CHECK(ScrollSkinChanged);
		SYSTEM_EVENT_CHECK(SetProgressValue);
		SYSTEM_EVENT_CHECK(SelectedChanged);
		SYSTEM_EVENT_CHECK(NodeExpanded);
		SYSTEM_EVENT_CHECK(NodeCollapsed);
		SYSTEM_EVENT_CHECK(AnimationDelayExpired);
		SYSTEM_EVENT_CHECK(AnimationExpired);
		hlog::warn(aprilui::logTag, "There is no system event with name: " + type);
		return false;
	}

}
