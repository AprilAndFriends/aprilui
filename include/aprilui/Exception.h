/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EXCEPTION_H
#define APRILUI_EXCEPTION_H

#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;
	
	class apriluiExport _GenericException : public hltypes::exception
	{
	public:
		hstr mType;
		
		_GenericException(chstr errorText, chstr type = "", const char* file = "", int line = 0);
		hstr getType(){ return mType; }
		hstr getErrorText() { return msg; }
	};

	#define GenericException(msg) _GenericException(msg, "GenericException", __FILE__, __LINE__)

	#define exception_cls(name) class apriluiExport name : public _GenericException \
	{ \
	public: \
		name(chstr errorText, chstr type = "", const char* file = "", int line = 0) : \
		  _GenericException(errorText, type, file, line) { } \
	}

	exception_cls(_ObjectExistsException);
	#define ObjectExistsException(name, filename) _ObjectExistsException("Object already exists: " + name + " in " + filename, "ObjectExistsException", __FILE__, __LINE__)

	exception_cls(_ObjectNotExistsException);
	#define ObjectNotExistsException(name, filename) _ObjectNotExistsException("Object doesn't exist: " + name + " in " + filename, "ObjectNotExistsException", __FILE__, __LINE__)

	exception_cls(_FontExistsException);
	#define FontExistsException(fontname) _FontExistsException("Unable to add new font, it already exists: " + fontname, "FontExistsException", __FILE__, __LINE__)

	exception_cls(_FontNotExistsException);
	#define FontNotExistsException(fontname) _FontNotExistsException("Font doesn't exist: " + fontname, "FontNotExistsException", __FILE__, __LINE__)

	//---------------------------------------------------------------------------------------------------------

	class apriluiExport _ResourceExistsException : public _GenericException
	{
	public:
		_ResourceExistsException(chstr object_name, chstr class_name, Dataset* dict, const char* file = "", int line = 0);
	};
	
	class apriluiExport _ResourceNotExistsException : public _GenericException
	{
	public:
		_ResourceNotExistsException(chstr object_name, chstr class_name, Dataset* dict, const char* file = "", int line = 0);
	};
	
	#define ResourceExistsException(name, cls, data) _ResourceExistsException(name, cls, data, __FILE__, __LINE__)
	#define ResourceNotExistsException(name, cls, data) _ResourceNotExistsException(name, cls, data, __FILE__, __LINE__)

	//---------------------------------------------------------------------------------------------------------

	class apriluiExport _ObjectHasParentException : public _GenericException
	{
	public:
		_ObjectHasParentException(chstr child, chstr parent, const char* file = "", int line = 0);
	};

	#define ObjectHasParentException(child, parent) _ObjectHasParentException(child, parent, __FILE__, __LINE__)

	//---------------------------------------------------------------------------------------------------------

	class apriluiExport _ObjectWithoutParentException : public _GenericException
	{
	public:
		_ObjectWithoutParentException(chstr child, const char* file = "", int line = 0);
	};

	#define ObjectWithoutParentException(child) _ObjectWithoutParentException(child, __FILE__, __LINE__)

	//---------------------------------------------------------------------------------------------------------

	class apriluiExport _ObjectNotChildException : public _GenericException
	{
	public:
		_ObjectNotChildException(chstr child, chstr parent, const char* file = "", int line = 0);
	};

	#define ObjectNotChildException(child, parent) _ObjectNotChildException(child, parent, __FILE__, __LINE__)

}

#endif
