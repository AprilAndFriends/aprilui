/// @file
/// @version 4.1
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
	bool textureFilesDebugExceptionsEnabled = true;
	bool childManipulationDebugExceptionsEnabled = true;
	bool creationFactoriesDebugExceptionsEnabled = true;
	bool objectExistenceDebugExceptionsEnabled = true;
	bool systemConsistencyDebugExceptionsEnabled = true;

	void readRectNode(grect& rect, hlxml::Node* node)
	{
		rect.set(0.0f, 0.0f, 0.0f, 0.0f);
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
				rect.setPosition(node->pfloat("x", 0.0f), node->pfloat("y", 0.0f));
			}
			if (node->pexists("size"))
			{
				rect.setSize(april::hstrToGvec2(node->pstr("size")));
			}
			else
			{
				rect.setSize(node->pfloat("w", 0.0f), node->pfloat("h", 0.0f));
			}
		}
	}

	april::Color makeModifiedDrawColor(const april::Color& color, const april::Color& drawColor)
	{
		return april::Color(color, (unsigned char)(color.a * drawColor.a_f()));
	}

}
