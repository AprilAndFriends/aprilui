/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_EXCEPTION_H
#define APRILUI_EXCEPTION_H

#include <hltypes/hstring.h>
#include <hltypes/hexception.h>
#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;
	
	class apriluiExport _GenericException : public hexception
	{
	public:
		hstr mType;
		
		_GenericException(chstr errorText, chstr type = "", const char* file = "", int line = 0);
		hstr getType(){ return mType; }
		hstr getErrorText() { return message; }
	};

	#define GenericException(msg) _GenericException(msg, "GenericException", __FILE__, __LINE__)

	#define exception_cls(name) class apriluiExport name : public _GenericException \
	{ \
	public: \
		name(chstr errorText, chstr type = "", const char* file = "", int line = 0) : \
		  _GenericException(errorText, type, file, line) { } \
	}

	exception_cls(_ObjectExistsException);
	#define ObjectExistsException(msg) _ObjectExistsException("Object already exists: " + msg, "ObjectExistsException", __FILE__, __LINE__)

	exception_cls(_FileNotFoundException);
	#define FileNotFoundException(msg) _FileNotFoundException("File not found: " + msg, "FileNotFoundException", __FILE__, __LINE__)

	exception_cls(_ObjectNotExistsException);
	#define ObjectNotExistsException(msg) _ObjectNotExistsException("Object doesn't exist: " + msg,"ObjectNotExistsException", __FILE__, __LINE__)

	exception_cls(_FontExistsException);
	#define FontExistsException(msg) _FontExistsException("Unable to add new font, it already exists: " + msg, "FontExistsException", __FILE__, __LINE__)

	exception_cls(_FontNotExistsException);
	#define FontNotExistsException(msg) _FontNotExistsException("Font doesn't exist: " + msg, "FontNotExistsException", __FILE__, __LINE__)

	//---------------------------------------------------------------------------------------------------------

	class apriluiExport _ApriluiResourceExistsException : public _GenericException
	{
	public:
		_ApriluiResourceExistsException(chstr object_name, chstr class_name, Dataset* dict, const char* file = "", int line = 0);
	};
	
	class apriluiExport _ApriluiResourceNotExistsException : public _GenericException
	{
	public:
		_ApriluiResourceNotExistsException(chstr object_name, chstr class_name, Dataset* dict, const char* file = "", int line = 0);
	};
	
	#define ApriluiResourceExistsException(name, cls, data) _ApriluiResourceExistsException(name, cls, data, __FILE__, __LINE__)
	#define ApriluiResourceNotExistsException(name, cls, data) _ApriluiResourceNotExistsException(name, cls, data, __FILE__, __LINE__)

	//---------------------------------------------------------------------------------------------------------

	class apriluiExport _ObjectHasParentException : public _GenericException
	{
	public:
		_ObjectHasParentException(chstr child, chstr parent, const char* file = "", int line = 0);
	};

	#define ObjectHasParentException(child, parent) _ObjectHasParentException(child, parent, __FILE__, __LINE__)

	//---------------------------------------------------------------------------------------------------------

	class apriluiExport _ObjectNotChildException : public _GenericException
	{
	public:
		_ObjectNotChildException(chstr child, chstr parent, const char* file = "", int line = 0);
	};

	#define ObjectNotChildException(child, parent) _ObjectNotChildException(child, parent, __FILE__, __LINE__)

}

#endif
