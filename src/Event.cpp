/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "Event.h"

namespace aprilui
{
	// aprilui trunk compatibility
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

	Event::~Event()
	{
	}

}
