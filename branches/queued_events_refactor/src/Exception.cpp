/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.5
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
		this->msg += errorText;
	}
	_GenericException::~_GenericException() { }

	_ResourceExistsException::_ResourceExistsException(chstr objectName, chstr className,
													   Dataset* dict, const char* file, int line) :
							  _GenericException("", "ResourceExistsException", file, line)
	{
		this->msg += className + " already exists: " + objectName + " in dataset " + dict->getName();
	}
	_ResourceExistsException::~_ResourceExistsException() { }

	_ResourceNotExistsException::_ResourceNotExistsException(chstr objectName, chstr className,
															 Dataset* dict, const char* file, int line) :
								 _GenericException("", "ResourceNotExistsException", file, line)
	{
		this->msg += className + " doesn't exist: " + objectName + " in dataset " + dict->getName();
	}
	_ResourceNotExistsException::~_ResourceNotExistsException() { }

	_InvalidObjectTypeCast::_InvalidObjectTypeCast(chstr errorText, const char* file, int line) : _GenericException(errorText, "InvalidObjectTypeCast", file, line) { }
	
	_InvalidObjectTypeCast::~_InvalidObjectTypeCast() { }
	
	_ObjectHasParentException::_ObjectHasParentException(chstr child, chstr parent,
														 const char* file, int line) :
							   _GenericException("", "ObjectHasParentException", file, line)
	{
		this->msg += "Cannot attach object '" + child + "' to object '" + parent + "', object already attached to another parent";
	}
	_ObjectHasParentException::~_ObjectHasParentException() { }

	_ObjectWithoutParentException::_ObjectWithoutParentException(chstr child,
																 const char* file, int line) :
								   _GenericException("", "ObjectWihoutParentException", file, line)
	{
		this->msg += "Cannot detach object '" + child + "', object has no parent";
	}
	_ObjectWithoutParentException::~_ObjectWithoutParentException() { }

	_ObjectNotChildException::_ObjectNotChildException(chstr child, chstr parent,
													   const char* file, int line) :
							  _GenericException("", "ObjectNotChildException", file, line)
	{
		this->msg += "Cannot detach object '" + child + "' from object '" + parent + "', object is not a child";
	}
	_ObjectNotChildException::~_ObjectNotChildException() { }

	_ObjectFactoryExistsException::_ObjectFactoryExistsException(chstr name,
																 const char* file, int line) :
							  _GenericException("", "ObjectFactoryExistsException", file, line)
	{
		this->msg += "Object factory named '" + name + "' already exists";
	}
	_ObjectFactoryExistsException::~_ObjectFactoryExistsException() { }

	_AnimatorFactoryExistsException::_AnimatorFactoryExistsException(chstr name,
																	 const char* file, int line) :
							  _GenericException("", "AnimatorFactoryExistsException", file, line)
	{
		this->msg += "Animator factory named '" + name + "' already exists";
	}
	_AnimatorFactoryExistsException::~_AnimatorFactoryExistsException() { }

}
