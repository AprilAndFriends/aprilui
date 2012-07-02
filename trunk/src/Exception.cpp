/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "Dataset.h"
#include "Exception.h"

namespace aprilui
{
	_GenericException::_GenericException(chstr errorText, chstr type, const char* file, int line) : 
					   hltypes::exception("", file, line)
	{
		msg += errorText;
	}
	_GenericException::~_GenericException() { }

	_ResourceExistsException::_ResourceExistsException(chstr object_name, chstr class_name,
													   Dataset* dict, const char* file, int line) :
							  _GenericException("", "ResourceExistsException", file, line)
	{
		msg += class_name + " already exists: " + object_name + " in dataset " + dict->getName();
	}
	_ResourceExistsException::~_ResourceExistsException() { }

	_ResourceNotExistsException::_ResourceNotExistsException(chstr object_name, chstr class_name,
															 Dataset* dict, const char* file, int line) :
								 _GenericException("", "ResourceNotExistsException", file, line)
	{
		msg += class_name + " doesn't exist: " + object_name + " in dataset " + dict->getName();
	}
	_ResourceNotExistsException::~_ResourceNotExistsException() { }

	_InvalidObjectTypeCast::_InvalidObjectTypeCast(chstr err_text, const char* file, int line) : _GenericException(err_text, "InvalidObjectTypeCast", file, line) { }
	
	_InvalidObjectTypeCast::~_InvalidObjectTypeCast() { }
	
	_ObjectHasParentException::_ObjectHasParentException(chstr child, chstr parent,
														 const char* file, int line) :
							   _GenericException("", "ObjectHasParentException", file, line)
	{
		msg += "Cannot attach object '" + child + "' to object '" + parent + "', object already attached to another parent";
	}
	_ObjectHasParentException::~_ObjectHasParentException() { }

	_ObjectWithoutParentException::_ObjectWithoutParentException(chstr child,
																 const char* file, int line) :
								   _GenericException("", "ObjectWihoutParentException", file, line)
	{
		msg += "Cannot detach object '" + child + "', object has no parent";
	}
	_ObjectWithoutParentException::~_ObjectWithoutParentException() { }

	_ObjectNotChildException::_ObjectNotChildException(chstr child, chstr parent,
													   const char* file, int line) :
							  _GenericException("", "ObjectNotChildException", file, line)
	{
		msg += "Cannot detach object '" + child + "' from object '" + parent + "', object is not a child";
	}
	_ObjectNotChildException::~_ObjectNotChildException() { }

	_ObjectFactoryExistsException::_ObjectFactoryExistsException(chstr name,
																 const char* file, int line) :
							  _GenericException("", "ObjectFactoryExistsException", file, line)
	{
		msg += "Object factory named '" + name + "' already exists";
	}
	_ObjectFactoryExistsException::~_ObjectFactoryExistsException() { }

	_AnimatorFactoryExistsException::_AnimatorFactoryExistsException(chstr name,
																	 const char* file, int line) :
							  _GenericException("", "AnimatorFactoryExistsException", file, line)
	{
		msg += "Animator factory named '" + name + "' already exists";
	}
	_AnimatorFactoryExistsException::~_AnimatorFactoryExistsException() { }

}
