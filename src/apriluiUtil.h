/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines utility functions used internally.

#ifndef APRILUI_UTIL_H
#define APRILUI_UTIL_H

#include <april/Color.h>
#include <gtypes/Rectangle.h>

namespace hlxml
{
	class Node;
}

namespace aprilui
{
	void readRectNode(grect& rect, hlxml::Node* node);
	april::Color makeModifiedDrawColor(april::Color color, april::Color drawColor);

}
#endif
