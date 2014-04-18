/// @file
/// @author  Boris Mikic
/// @version 3.14
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "ObjectScrollBarButtonBackground.h"

namespace aprilui
{
	ScrollBarButtonBackground::ScrollBarButtonBackground(chstr name, grect rect) : ImageButton(name, rect)
	{
	}

	ScrollBarButtonBackground::~ScrollBarButtonBackground()
	{
	}

	Object* ScrollBarButtonBackground::createInstance(chstr name, grect rect)
	{
		return new ScrollBarButtonBackground(name, rect);
	}

}
