/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectGridViewCell.h"

namespace aprilui
{
	GridViewCell::GridViewCell(chstr name) : Container(name)
	{
		this->_gridView = NULL;
	}

	GridViewCell::GridViewCell(const GridViewCell& other) : Container(other)
	{
		this->_gridView = NULL;
	}

	GridViewCell::~GridViewCell()
	{
	}

	Object* GridViewCell::createInstance(chstr name)
	{
		return new GridViewCell(name);
	}

}
