/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "Animator.h"
#include "Dataset.h"
#include "EventArgs.h"
#include "Object.h"

namespace aprilui
{
	EventArgs::EventArgs(chstr eventType, EventReceiver* receiver, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		this->_initDefaults(eventType);
		this->receiver = receiver;
		this->baseObject = dynamic_cast<BaseObject*>(receiver);
		this->object = dynamic_cast<Object*>(receiver);
		this->animator = dynamic_cast<Animator*>(receiver);
		this->dataset = dynamic_cast<Dataset*>(receiver);
		this->keyCode = keyCode;
		this->position = position;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, EventReceiver* receiver, april::Button buttonCode, chstr string, void* userData)
	{
		this->_initDefaults(eventType);
		this->receiver = receiver;
		this->baseObject = dynamic_cast<BaseObject*>(receiver);
		this->object = dynamic_cast<Object*>(receiver);
		this->animator = dynamic_cast<Animator*>(receiver);
		this->dataset = dynamic_cast<Dataset*>(receiver);
		this->buttonCode = buttonCode;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, EventReceiver* receiver, chstr string, void* userData)
	{
		this->_initDefaults(eventType);
		this->receiver = receiver;
		this->baseObject = dynamic_cast<BaseObject*>(receiver);
		this->object = dynamic_cast<Object*>(receiver);
		this->animator = dynamic_cast<Animator*>(receiver);
		this->dataset = dynamic_cast<Dataset*>(receiver);
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, EventReceiver* receiver, void* userData)
	{
		this->_initDefaults(eventType);
		this->receiver = receiver;
		this->baseObject = dynamic_cast<BaseObject*>(receiver);
		this->object = dynamic_cast<Object*>(receiver);
		this->animator = dynamic_cast<Animator*>(receiver);
		this->dataset = dynamic_cast<Dataset*>(receiver);
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, Object* object, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		this->_initDefaults(eventType);
		this->object = object;
		this->baseObject = object;
		this->receiver = object;
		this->keyCode = keyCode;
		this->position = position;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, Object* object, april::Button buttonCode, chstr string, void* userData)
	{
		this->_initDefaults(eventType);
		this->object = object;
		this->baseObject = object;
		this->receiver = object;
		this->buttonCode = buttonCode;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, Object* object, chstr string, void* userData)
	{
		this->_initDefaults(eventType);
		this->object = object;
		this->baseObject = object;
		this->receiver = object;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, Object* object, void* userData)
	{
		this->_initDefaults(eventType);
		this->object = object;
		this->baseObject = object;
		this->receiver = object;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, Animator* animator, chstr string, void* userData)
	{
		this->_initDefaults(eventType);
		this->animator = animator;
		this->baseObject = animator;
		this->receiver = animator;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, Animator* animator, void* userData)
	{
		this->_initDefaults(eventType);
		this->animator = animator;
		this->baseObject = animator;
		this->receiver = animator;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, Dataset* dataset, chstr string, void* userData)
	{
		this->_initDefaults(eventType);
		this->dataset = dataset;
		this->receiver = dataset;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(chstr eventType, Dataset* dataset, void* userData)
	{
		this->_initDefaults(eventType);
		this->dataset = dataset;
		this->receiver = dataset;
		this->userData = userData;
	}

	EventArgs::~EventArgs()
	{
	}

	void EventArgs::_initDefaults(chstr eventType)
	{
		this->eventType = eventType;
		this->receiver = NULL;
		this->baseObject = NULL;
		this->object = NULL;
		this->animator = NULL;
		this->dataset = NULL;
		this->keyCode = april::Key::None;
		this->buttonCode = april::Button::None;
		this->userData = NULL;
	}
	
}
