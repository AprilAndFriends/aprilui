/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "Dataset.h"
#include "Exception.h"

namespace aprilui
{
	_ObjectExistsException::_ObjectExistsException(chstr type, chstr name, chstr filename, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage(type + " already exists: " + name + " in " + filename, file, line);
	}

	_ObjectNotExistsException::_ObjectNotExistsException(chstr type, chstr name, chstr filename, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage(type + " doesn't exist: " + name + " in " + filename, file, line);
	}

	_FontExistsException::_FontExistsException(chstr fontname, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage("Unable to add new font, it already exists: " + fontname, file, line);
	}

	_FontNotExistsException::_FontNotExistsException(chstr fontname, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage("Font doesn't exist: " + fontname, file, line);
	}

	_InvalidObjectTypeCast::_InvalidObjectTypeCast(chstr type, chstr objectName, chstr datasetName, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage(hsprintf("%s '%s' found in dataset '%s' but dynamic cast failed.", type.cStr(), objectName.cStr(), datasetName.cStr()), file, line);
	}
	
	_ObjectHasParentException::_ObjectHasParentException(chstr child, chstr parent, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage("Cannot attach object '" + child + "' to object '" + parent + "', object already attached to another parent.", file, line);
	}

	_ObjectNotChildException::_ObjectNotChildException(chstr child, chstr parent, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage("Cannot detach object '" + child + "' from object '" + parent + "', object is not a child.", file, line);
	}

	_ObjectFactoryExistsException::_ObjectFactoryExistsException(chstr type, chstr name, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage(type + " factory named '" + name + "' already exists.", file, line);
	}

	_ObjectFactoryNotExistsException::_ObjectFactoryNotExistsException(chstr type, chstr name, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage(type + " factory named '" + name + "' doesn't exists.", file, line);
	}

}
