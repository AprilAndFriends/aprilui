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

#include "ObjectScrollBarButtonBackward.h"

namespace aprilui
{
	ScrollBarButtonBackward::ScrollBarButtonBackward(chstr name, grect rect) : ImageButton(name, rect)
	{
	}

	ScrollBarButtonBackward::~ScrollBarButtonBackward()
	{
	}

	Object* ScrollBarButtonBackward::createInstance(chstr name, grect rect)
	{
		return new ScrollBarButtonBackward(name, rect);
	}

}
