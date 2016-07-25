/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>

#include "Animator.h"

namespace aprilui
{
	Animator::Animator(chstr type, chstr name, grect rect) :
		Object(type, name, rect)
	{
		mDelay = 0.0f;
		mInitialDelay = 0.0f;
	}
	
}
