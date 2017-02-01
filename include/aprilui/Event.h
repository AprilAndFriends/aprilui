/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_EVENT_H
#define APRILUI_EVENT_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport Event
	{
	public:
		// aprilui trunk compatibility
		static hstr DatasetLoaded;
		static hstr DatasetUnloaded;
		static hstr RegisteredInDataset;
		static hstr UnregisteredFromDataset;
		static hstr TextureLoaded;
		static hstr TextureUnloaded;
		static hstr AttachedToObject;
		static hstr DetachedFromObject;
		static hstr MouseDown;
		static hstr MouseUp;
		static hstr MouseMove;
		static hstr MouseScroll;
		static hstr MouseCancel;
		static hstr Click;
		static hstr KeyDown;
		static hstr KeyUp;
		static hstr ButtonDown;
		static hstr ButtonUp;
		static hstr ButtonTrigger;
		static hstr EnabledChanged;
		static hstr AwakeChanged;
		static hstr Resized;
		static hstr FocusGained;
		static hstr FocusLost;
		static hstr HoverStarted;
		static hstr HoverFinished;
		static hstr TextChanged;
		static hstr TextKeyChanged;
		static hstr LocalizationChanged;
		static hstr SubmitEditText;
		static hstr ScrollSkinChanged;
		static hstr SetProgressValue;
		static hstr SelectedChanged;
		static hstr NodeExpanded;
		static hstr NodeCollapsed;
		static hstr AnimationDelayExpired;
		static hstr AnimationExpired;

		virtual void execute(void* params) = 0;
		virtual ~Event();
	};

}

#endif
