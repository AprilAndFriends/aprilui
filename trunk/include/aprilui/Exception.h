/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines all exceptions.

#ifndef APRILUI_EXCEPTION_H
#define APRILUI_EXCEPTION_H

#include <hltypes/exception.h>
#include <hltypes/hstring.h>
#include <hltypes/hltypesUtil.h>

#include "apriluiExport.h"

namespace aprilui
{
	class Dataset;

#define _EXCEPTION_CLASS_1(classe, arg0) \
	class apriluiExport _ ## classe : public hexception \
	{ \
	public: \
		_ ## classe(chstr arg0, const char* file, int line); \
		inline ~_ ## classe() { } \
		inline hstr getType() { return #classe; } \
	};

#define _EXCEPTION_CLASS_2(classe, arg0, arg1) \
	class apriluiExport _ ## classe : public hexception \
	{ \
	public: \
		_ ## classe(chstr arg0, chstr arg1, const char* file, int line); \
		inline ~_ ## classe() { } \
		inline hstr getType() { return #classe; } \
	};

#define _EXCEPTION_CLASS_3(classe, arg0, arg1, arg2) \
	class apriluiExport _ ## classe : public hexception \
	{ \
	public: \
		_ ## classe(chstr arg0, chstr arg1, chstr arg2, const char* file, int line); \
		inline ~_ ## classe() { } \
		inline hstr getType() { return #classe; } \
	};

	_EXCEPTION_CLASS_3(ObjectExistsException, type, name, filename);
	#define ObjectExistsException(type, name, filename) _ObjectExistsException(type, name, filename, __FILE__, __LINE__)

	_EXCEPTION_CLASS_3(ObjectNotExistsException, type, name, filename);
	#define ObjectNotExistsException(type, name, filename) _ObjectNotExistsException(type, name, filename, __FILE__, __LINE__)

	_EXCEPTION_CLASS_1(FontExistsException, fontname);
	#define FontExistsException(fontname) _FontExistsException(fontname, __FILE__, __LINE__)

	_EXCEPTION_CLASS_1(FontNotExistsException, fontname);
	#define FontNotExistsException(fontname) _FontNotExistsException(fontname, __FILE__, __LINE__)

	_EXCEPTION_CLASS_3(InvalidObjectTypeCast, type, objectName, datasetName);
	#define InvalidObjectTypeCast(type, objectName, datasetName) _InvalidObjectTypeCast(type, objectName, datasetName, __FILE__, __LINE__)

	_EXCEPTION_CLASS_2(ObjectHasParentException, child, parent);
	#define ObjectHasParentException(child, parent) _ObjectHasParentException(child, parent, __FILE__, __LINE__)

	_EXCEPTION_CLASS_1(ObjectWithoutParentException, child);
	#define ObjectWithoutParentException(child) _ObjectWithoutParentException(child, __FILE__, __LINE__)

	_EXCEPTION_CLASS_2(ObjectNotChildException, child, parent);
	#define ObjectNotChildException(child, parent) _ObjectNotChildException(child, parent, __FILE__, __LINE__)

	_EXCEPTION_CLASS_2(ObjectFactoryExistsException, type, name);
	#define ObjectFactoryExistsException(type, name) _ObjectFactoryExistsException(type, name, __FILE__, __LINE__)

	_EXCEPTION_CLASS_2(ObjectFactoryNotExistsException, type, name);
	#define ObjectFactoryNotExistsException(type, name) _ObjectFactoryNotExistsException(type, name, __FILE__, __LINE__)

}

#endif
