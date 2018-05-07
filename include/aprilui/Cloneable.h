/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a cloneable interface.

#ifndef APRILUI_CLONEABLE_H
#define APRILUI_CLONEABLE_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"

#define APRILUI_CLONEABLE(type) \
public: \
	virtual type* clone() const { return new type(*this); } \
protected: \
	type(const type& other);

#define APRILUI_CLONEABLE_ABSTRACT(type) \
public: \
	virtual type* clone() const = 0; \
protected: \
	type(const type& other);

namespace aprilui
{
	class apriluiExport Cloneable
	{
		APRILUI_CLONEABLE_ABSTRACT(Cloneable);
	public:
		Cloneable();
		virtual ~Cloneable();

	};
	
}

#endif
