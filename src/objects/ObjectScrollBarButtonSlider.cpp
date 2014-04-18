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

#include "ObjectScrollBarButtonSlider.h"

namespace aprilui
{
	ScrollBarButtonSlider::ScrollBarButtonSlider(chstr name, grect rect) : ImageButton(name, rect)
	{
	}

	ScrollBarButtonSlider::~ScrollBarButtonSlider()
	{
	}

	Object* ScrollBarButtonSlider::createInstance(chstr name, grect rect)
	{
		return new ScrollBarButtonSlider(name, rect);
	}

}
