/// @file
/// @version 1.34
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
					   hexception("", file, line)
	{
		message += errorText;
	}

	_ApriluiResourceExistsException::_ApriluiResourceExistsException(chstr object_name, chstr class_name,
													   Dataset* dict, const char* file, int line) :
							  _GenericException("", "ApriluiResourceExistsException", file, line)
	{
		message += class_name + " already exists: " + object_name + " in dataset " + dict->getName();
	}

	_ApriluiResourceNotExistsException::_ApriluiResourceNotExistsException(chstr object_name, chstr class_name,
														  Dataset* dict, const char* file, int line) :
								 _GenericException("", "ApriluiResourceNotExistsException", file, line)
	{
		message += class_name + " doesn't exist: " + object_name + " in dataset " + dict->getName();
	}

	_ObjectHasParentException::_ObjectHasParentException(chstr child, chstr parent,
														 const char* file, int line) :
							   _GenericException("", "ObjectHasParentException", file, line)
	{
		message += "Cannot attach object '" + child + "' to object '" + parent + "', object already attached to another parent";
	}

	_ObjectNotChildException::_ObjectNotChildException(chstr child, chstr parent,
														 const char* file, int line) :
							  _GenericException("", "ObjectNotChildException", file, line)
	{
		message += "Cannot detach object '" + child + "' from object '" + parent + "', object is not a child";
	}

}
