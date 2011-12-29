/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines macros that can be used for simpler registration of callbacks.

#ifndef APRILUI_EVENT_UTILS_H
#define APRILUI_EVENT_UTILS_H

#include "CallbackEvent.h"
#include "MemberCallbackEvent.h"

// for class member functions being registered from within the class
#define SET_CLICK_EVENT(dataset, name, cls, member_func) \
	dataset->getObject(name)->registerEvent("Click", new aprilui::MemberCallbackEvent(&cls::member_func, (cls*)this));
#define SET_MOUSEDOWN_EVENT(dataset, name, cls, member_func) \
	dataset->getObject(name)->registerEvent("MouseDown", new aprilui::MemberCallbackEvent(&cls::member_func, (cls*)this));
// for class member functions being registered from within the class
#define _SET_CLICK_EVENT(obj, cls, member_func) \
	obj->registerEvent("Click", new aprilui::MemberCallbackEvent(&cls::member_func, (cls*)this));
#define _SET_MOUSEDOWN_EVENT(obj, cls, member_func) \
	obj->registerEvent("MouseDown", new aprilui::MemberCallbackEvent(&cls::member_func, (cls*)this));
// for free C functions and static class functions
#define SET_CLICK_EVENT_FUNCTION(dataset, obj_name, free_function) \
	dataset->getObject(obj_name)->registerEvent("Click", new aprilui::CallbackEvent(free_function));
#define SET_MOUSEDOWN_EVENT_FUNCTION(dataset, obj_name, free_function) \
	dataset->getObject(obj_name)->registerEvent("MouseDown", new aprilui::CallbackEvent(free_function));

#endif
