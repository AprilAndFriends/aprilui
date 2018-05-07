/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an object for arguments of event callbacks.

#ifndef APRILUI_EVENT_ARGS_H
#define APRILUI_EVENT_ARGS_H

#include <april/Keys.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Animator;
	class BaseObject;
	class Dataset;
	class EventReceiver;
	class Object;

	class apriluiExport EventArgs
	{
	public:
		hstr eventType;
		EventReceiver* receiver;
		BaseObject* baseObject;
		Object* object;
		Animator* animator;
		Dataset* dataset;
		gvec2 position;
		hstr string;
		april::Key keyCode;
		april::Button buttonCode;
		void* userData;
		
		EventArgs(chstr eventType, EventReceiver* receiver, april::Key keyCode, cgvec2 position = gvec2(), chstr string = "", void* userData = NULL);
		EventArgs(chstr eventType, EventReceiver* receiver, april::Button buttonCode, chstr string = "", void* userData = NULL);
		EventArgs(chstr eventType, EventReceiver* receiver, chstr string, void* userData = NULL);
		EventArgs(chstr eventType, EventReceiver* receiver, void* userData = NULL);
		EventArgs(chstr eventType, Object* object, april::Key keyCode, cgvec2 position = gvec2(), chstr string = "", void* userData = NULL);
		EventArgs(chstr eventType, Object* object, april::Button buttonCode, chstr string = "", void* userData = NULL);
		EventArgs(chstr eventType, Object* object, chstr string, void* userData = NULL);
		EventArgs(chstr eventType, Object* object, void* userData = NULL);
		EventArgs(chstr eventType, Animator* animator, chstr string, void* userData = NULL);
		EventArgs(chstr eventType, Animator* animator, void* userData = NULL);
		EventArgs(chstr eventType, Dataset* dataset, chstr string, void* userData = NULL);
		EventArgs(chstr eventType, Dataset* dataset, void* userData = NULL);
		~EventArgs();

	protected:
		void _initDefaults(chstr eventType);

	};

}

#endif

