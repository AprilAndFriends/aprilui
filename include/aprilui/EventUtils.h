/// @file
/// @version 3.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines macros that can be used for simpler registration of callbacks.

#ifndef APRILUI_EVENT_UTILS_H
#define APRILUI_EVENT_UTILS_H

#include "aprilui.h"
#include "CallbackEvent.h"
#include "MemberCallbackEvent.h"

#pragma message("WARNING! Using EventUtils.h is deprecated!")

// for class member functions being registered from within the class
#define SET_Click_EVENT(dataset, name, cls, member_func) \
	hlog::warn(aprilui::logTag, "Using SET_Click_EVENT() is deprecated."); \
	dataset->getObject(name)->registerEvent(aprilui::Event::Click, new aprilui::MemberCallbackEvent<cls>(&cls::member_func, (cls*)this));
#define SET_MOUSEDOWN_EVENT(dataset, name, cls, member_func) \
	hlog::warn(aprilui::logTag, "Using SET_MOUSEDOWN_EVENT() is deprecated."); \
	dataset->getObject(name)->registerEvent(aprilui::Event::MouseDown, new aprilui::MemberCallbackEvent<cls>(&cls::member_func, (cls*)this));
// for class member functions being registered from within the class
#define _SET_Click_EVENT(obj, cls, member_func) \
	hlog::warn(aprilui::logTag, "Using _SET_Click_EVENT() is deprecated."); \
	obj->registerEvent(aprilui::Event::Click, new aprilui::MemberCallbackEvent<cls>(&cls::member_func, (cls*)this));
#define _SET_MOUSEDOWN_EVENT(obj, cls, member_func) \
	hlog::warn(aprilui::logTag, "Using _SET_MOUSEDOWN_EVENT() is deprecated."); \
	obj->registerEvent(aprilui::Event::MouseDown, new aprilui::MemberCallbackEvent<cls>(&cls::member_func, (cls*)this));
// for free C functions and static class functions
#define SET_Click_EVENT_FUNCTION(dataset, obj_name, free_function) \
	hlog::warn(aprilui::logTag, "Using SET_Click_EVENT_FUNCTION() is deprecated."); \
	dataset->getObject(obj_name)->registerEvent(aprilui::Event::Click, new aprilui::CallbackEvent(free_function));
#define SET_MOUSEDOWN_EVENT_FUNCTION(dataset, obj_name, free_function) \
	hlog::warn(aprilui::logTag, "Using SET_MOUSEDOWN_EVENT_FUNCTION() is deprecated."); \
	dataset->getObject(obj_name)->registerEvent(aprilui::Event::MouseDown, new aprilui::CallbackEvent(free_function));
// for free C functions and static class functions
#define _SET_Click_EVENT_FUNCTION(obj, free_function) \
	hlog::warn(aprilui::logTag, "Using _SET_Click_EVENT_FUNCTION() is deprecated."); \
	obj->registerEvent(aprilui::Event::Click, new aprilui::CallbackEvent(free_function));
#define _SET_MOUSEDOWN_EVENT_FUNCTION(obj, free_function) \
	hlog::warn(aprilui::logTag, "Using _SET_MOUSEDOWN_EVENT_FUNCTION() is deprecated."); \
	obj->registerEvent(aprilui::Event::MouseDown, new aprilui::CallbackEvent(free_function));

#endif
