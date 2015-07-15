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
	EventArgs::EventArgs(EventReceiver* receiver, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		this->_initDefaults();
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

	EventArgs::EventArgs(EventReceiver* receiver, april::Button buttonCode, chstr string, void* userData)
	{
		this->_initDefaults();
		this->receiver = receiver;
		this->baseObject = dynamic_cast<BaseObject*>(receiver);
		this->object = dynamic_cast<Object*>(receiver);
		this->animator = dynamic_cast<Animator*>(receiver);
		this->dataset = dynamic_cast<Dataset*>(receiver);
		this->buttonCode = buttonCode;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(EventReceiver* receiver, chstr string, void* userData)
	{
		this->_initDefaults();
		this->receiver = receiver;
		this->baseObject = dynamic_cast<BaseObject*>(receiver);
		this->object = dynamic_cast<Object*>(receiver);
		this->animator = dynamic_cast<Animator*>(receiver);
		this->dataset = dynamic_cast<Dataset*>(receiver);
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(EventReceiver* receiver, void* userData)
	{
		this->_initDefaults();
		this->receiver = receiver;
		this->baseObject = dynamic_cast<BaseObject*>(receiver);
		this->object = dynamic_cast<Object*>(receiver);
		this->animator = dynamic_cast<Animator*>(receiver);
		this->dataset = dynamic_cast<Dataset*>(receiver);
		this->userData = userData;
	}

	EventArgs::EventArgs(Object* object, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		this->_initDefaults();
		this->object = object;
		this->baseObject = object;
		this->receiver = object;
		this->keyCode = keyCode;
		this->position = position;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(Object* object, april::Button buttonCode, chstr string, void* userData)
	{
		this->_initDefaults();
		this->object = object;
		this->baseObject = object;
		this->receiver = object;
		this->buttonCode = buttonCode;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(Object* object, chstr string, void* userData)
	{
		this->_initDefaults();
		this->object = object;
		this->baseObject = object;
		this->receiver = object;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(Object* object, void* userData)
	{
		this->_initDefaults();
		this->object = object;
		this->baseObject = object;
		this->receiver = object;
		this->userData = userData;
	}

	EventArgs::EventArgs(Animator* animator, chstr string, void* userData)
	{
		this->_initDefaults();
		this->animator = animator;
		this->baseObject = animator;
		this->receiver = animator;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(Animator* animator, void* userData)
	{
		this->_initDefaults();
		this->animator = animator;
		this->baseObject = animator;
		this->receiver = animator;
		this->userData = userData;
	}

	EventArgs::EventArgs(Dataset* dataset, chstr string, void* userData)
	{
		this->_initDefaults();
		this->dataset = dataset;
		this->receiver = dataset;
		this->string = string;
		this->userData = userData;
	}

	EventArgs::EventArgs(Dataset* dataset, void* userData)
	{
		this->_initDefaults();
		this->dataset = dataset;
		this->receiver = dataset;
		this->userData = userData;
	}

	EventArgs::~EventArgs()
	{
	}

	void EventArgs::_initDefaults()
	{
		this->receiver = NULL;
		this->baseObject = NULL;
		this->object = NULL;
		this->animator = NULL;
		this->dataset = NULL;
		this->keyCode = april::AK_NONE;
		this->buttonCode = april::AB_NONE;
		this->userData = NULL;
	}
	
}
