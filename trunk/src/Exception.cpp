/// @file
/// @version 3.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "Dataset.h"
#include "Exception.h"

namespace aprilui
{
	_GenericException::_GenericException(chstr errorText, chstr type, const char* file, int line) : 
					   hltypes::exception(errorText, file, line)
	{
	}
	_GenericException::~_GenericException() { }

	_ResourceExistsException::_ResourceExistsException(chstr objectName, chstr className,
													   Dataset* dict, const char* file, int line) :
							  _GenericException(className + " already exists: " + objectName + " in dataset " + dict->getName(),
								  "ResourceExistsException", file, line)
	{
	}
	_ResourceExistsException::~_ResourceExistsException() { }

	_ResourceNotExistsException::_ResourceNotExistsException(chstr objectName, chstr className,
															 Dataset* dict, const char* file, int line) :
								 _GenericException(className + " doesn't exist: '" + objectName + "' in dataset '" + dict->getName() + "'",
									 "ResourceNotExistsException", file, line)
	{
	}
	_ResourceNotExistsException::~_ResourceNotExistsException() { }

	_InvalidObjectTypeCast::_InvalidObjectTypeCast(chstr errorText, const char* file, int line) : _GenericException(errorText, "InvalidObjectTypeCast", file, line) { }
	
	_InvalidObjectTypeCast::~_InvalidObjectTypeCast() { }
	
	_ObjectHasParentException::_ObjectHasParentException(chstr child, chstr parent,
														 const char* file, int line) :
							   _GenericException("Cannot attach object '" + child + "' to object '" + parent + "', object already attached to another parent",
								   "ObjectHasParentException", file, line)
	{
	}
	_ObjectHasParentException::~_ObjectHasParentException() { }

	_ObjectWithoutParentException::_ObjectWithoutParentException(chstr child,
																 const char* file, int line) :
								   _GenericException("Cannot detach object '" + child + "', object has no parent",
									   "ObjectWihoutParentException", file, line)
	{
	}
	_ObjectWithoutParentException::~_ObjectWithoutParentException() { }

	_ObjectNotChildException::_ObjectNotChildException(chstr child, chstr parent,
													   const char* file, int line) :
							  _GenericException("Cannot detach object '" + child + "' from object '" + parent + "', object is not a child", "ObjectNotChildException", file, line)
	{
	}
	_ObjectNotChildException::~_ObjectNotChildException() { }

	_ObjectFactoryExistsException::_ObjectFactoryExistsException(chstr name,
																 const char* file, int line) :
	_GenericException("Object factory named '" + name + "' already exists",
					  "ObjectFactoryExistsException", file, line)
	{
	}
	_ObjectFactoryExistsException::~_ObjectFactoryExistsException() { }

	_ObjectFactoryNotExistsException::_ObjectFactoryNotExistsException(chstr name,
																 const char* file, int line) :
	_GenericException("Object factory named '" + name + "' doesn't exists",
					  "ObjectFactoryNotExistsException", file, line)
	{
	}
	_ObjectFactoryNotExistsException::~_ObjectFactoryNotExistsException() { }

	_AnimatorFactoryExistsException::_AnimatorFactoryExistsException(chstr name,
																	 const char* file, int line) :
	_GenericException("Animator factory named '" + name + "' already exists",
					  "AnimatorFactoryExistsException", file, line)
	{
	}
	_AnimatorFactoryNotExistsException::~_AnimatorFactoryNotExistsException() { }

	_AnimatorFactoryNotExistsException::_AnimatorFactoryNotExistsException(chstr name,
																	 const char* file, int line) :
	_GenericException("Animator factory named '" + name + "' doesn't exist",
					  "AnimatorFactoryNotExistsException", file, line)
	{
	}
	_AnimatorFactoryExistsException::~_AnimatorFactoryExistsException() { }

}
