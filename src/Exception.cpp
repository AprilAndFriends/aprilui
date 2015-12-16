/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "Dataset.h"
#include "Exception.h"

namespace aprilui
{
	_GenericException::_GenericException(chstr errorText, chstr type, const char* file, int line) : 
					   hexception("", file, line)
	{
		message += errorText;
	}

	_ResourceExistsException::_ResourceExistsException(chstr object_name, chstr class_name,
													   Dataset* dict, const char* file, int line) :
							  _GenericException("", "ResourceExistsException", file, line)
	{
		message += class_name + " already exists: " + object_name + " in dataset " + dict->getName();
	}

	_ResourceNotExistsException::_ResourceNotExistsException(chstr object_name, chstr class_name,
														  Dataset* dict, const char* file, int line) :
								 _GenericException("", "ResourceNotExistsException", file, line)
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
