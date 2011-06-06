/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EVENT_UTILS_H
#define APRILUI_EVENT_UTILS_H

#include "CallbackEvent.h"
#include "MemberCallbackEvent.h"

// for class member functions being registered from within the class
#define SET_CLICK_EVENT(dataset,obj_name,cls,member_func) \
		dataset->getObject(obj_name)->registerEvent("Click",new aprilui::MemberCallbackEvent(&cls::member_func,(cls*) this));
#define SET_MOUSEDOWN_EVENT(dataset,obj_name,cls,member_func) \
        dataset->getObject(obj_name)->registerEvent("MouseDown",new aprilui::MemberCallbackEvent(&cls::member_func,(cls*) this));
// for class member functions being registered from within the class
#define _SET_CLICK_EVENT(obj,cls,member_func) \
		obj->registerEvent("Click",new aprilui::MemberCallbackEvent(&cls::member_func,(cls*) this));
#define _SET_MOUSEDOWN_EVENT(obj,cls,member_func) \
        obj->registerEvent("MouseDown",new aprilui::MemberCallbackEvent(&cls::member_func,(cls*) this));
// for free C functions and static class functions
#define SET_CLICK_EVENT_FUNCTION(dataset,obj_name,free_function) \
		dataset->getObject(obj_name)->registerEvent("Click",new aprilui::CallbackEvent(free_function));

#endif
