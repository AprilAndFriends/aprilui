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
/// Defines utility functions used internally.

#ifndef APRILUI_UTIL_H
#define APRILUI_UTIL_H

#include <april/april.h>
#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hlxml/Node.h>

#define __THROW_EXCEPTION(exception, enabled, returnCode) \
	if (enabled) \
	{ \
		throw exception; \
	} \
	exception; \
	returnCode;

namespace aprilui
{
	extern bool textureFilesDebugExceptionsEnabled;
	extern bool childManipulationDebugExceptionsEnabled;
	extern bool creationFactoriesDebugExceptionsEnabled;
	extern bool objectExistenceDebugExceptionsEnabled;
	extern bool systemConsistencyDebugExceptionsEnabled;

	inline void _readRectNode(grect& rect, hlxml::Node* node, const gvec2* parentSize = NULL)
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
				rect.x = node->pfloat("x", 0.0f);
				rect.y = node->pfloat("y", 0.0f);
			}
			if (node->pexists("size"))
			{
				if (node->pstr("size") == "parent" && parentSize != NULL)
				{
					rect.setSize(*parentSize);
				}
				else
				{
					rect.setSize(april::hstrToGvec2(node->pstr("size")));
				}
			}
			else
			{
				rect.w = node->pfloat("w", 0.0f);
				rect.h = node->pfloat("h", 0.0f);
			}
		}
	}

	inline april::Color _makeModifiedDrawColor(const april::Color& color, const april::Color& drawColor)
	{
		return april::Color(color, (unsigned char)(color.a * drawColor.a_f()));
	}

	inline april::Color _makeColor(chstr colorValue)
	{
		april::Color result;
		if (april::findSymbolicColor(colorValue, result))
		{
			return result;
		}
		result.set(colorValue);
		return result;
	}

}
#endif
