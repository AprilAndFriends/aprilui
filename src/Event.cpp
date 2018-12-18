/// @file
/// @version 5.1
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
	if (type == checkType || !caseSensitive && type.lowered() == checkType.lowered()) \
	{ \
		return true; \
	}

namespace aprilui
{
	hstr Event::DatasetLoaded = "DatasetLoaded";
	hstr Event::DatasetUnloaded = "DatasetUnloaded";
	hstr Event::RegisteredInDataset = "RegisteredInDataset";
	hstr Event::UnregisteredFromDataset = "UnregisteredFromDataset";
	hstr Event::TextureLoaded = "TextureLoaded";
	hstr Event::TextureUnloaded = "TextureUnloaded";
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
	hstr Event::AwakeChanged = "AwakeChanged";
	hstr Event::PositionChanged = "PositionChanged";
	hstr Event::SizeChanged = "SizeChanged";
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

	Event::Event() :
		Cloneable()
	{
	}

	Event::Event(const Event& other) :
		Cloneable(other)
	{
	}

	bool Event::isSystemEvent(chstr type, bool caseSensitive)
	{
		if (type == DEPRECATED_EVENT_RESIZED)
		{
			hlog::errorf(logTag, "Event '%s' is deprecated, use '%s' instead!", DEPRECATED_EVENT_RESIZED, Event::SizeChanged.cStr());
			((hstr)type).replace(DEPRECATED_EVENT_RESIZED, Event::SizeChanged);
		}
		SYSTEM_EVENT_CHECK(DatasetLoaded);
		SYSTEM_EVENT_CHECK(DatasetUnloaded);
		SYSTEM_EVENT_CHECK(RegisteredInDataset);
		SYSTEM_EVENT_CHECK(UnregisteredFromDataset);
		SYSTEM_EVENT_CHECK(TextureLoaded);
		SYSTEM_EVENT_CHECK(TextureUnloaded);
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
		SYSTEM_EVENT_CHECK(AwakeChanged);
		SYSTEM_EVENT_CHECK(PositionChanged);
		SYSTEM_EVENT_CHECK(SizeChanged);
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
		hlog::warn(logTag, "There is no system event with name: " + type);
		return false;
	}

}
