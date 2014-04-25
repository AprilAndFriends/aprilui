/// @file
/// @author  Boris Mikic
/// @version 3.14
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

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

namespace hlxml
{
	class Node;
}

namespace aprilui
{
	static const float eTolerance = 0.0001f;

	hstr generateName(chstr prefix);
	hstr gvec2_to_hstr(gvec2 vector);
	hstr grect_to_hstr(grect rect);
	gvec2 hstr_to_gvec2(chstr string);
	grect hstr_to_grect(chstr string);

	void read_rect_node(grect& rect, hlxml::Node* node, bool allowNoSize = false);
	
}
#endif
