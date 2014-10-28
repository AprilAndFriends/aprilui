/// @file
/// @version 3.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hlxml/Node.h>

#include "aprilui.h"
#include "apriluiUtil.h"

namespace aprilui
{	
	void readRectNode(grect& rect, hlxml::Node* node, bool allowNoSize)
	{
		rect.set(0.0f, 0.0f, 0.0f, 0.0f);
		if (allowNoSize)
		{
			rect.setSize(-1.0f, -1.0f);
		}
		if (node->pexists("rect"))
		{
			rect = april::hstrToGrect(node->pstr("rect"));
		}
		else
		{
			if (node->pexists("position"))
			{
				rect.setPosition(april::hstrToGvec2(node->pstr("position")));
			}
			else
			{
				rect.setPosition(node->pfloat("x"), node->pfloat("y"));
			}
			if (node->pexists("size"))
			{
				rect.setSize(april::hstrToGvec2(node->pstr("size")));
			}
			else if (!allowNoSize)
			{
				rect.setSize(node->pfloat("w"), node->pfloat("h"));
			}
			else
			{
				rect.setSize(node->pfloat("w", -1.0f), node->pfloat("h", -1.0f));
			}
		}
	}

}
