/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_ANIMATOR_H
#define APRILUI_ANIMATOR_H

#include <gtypes/Rectangle.h>

#include "Object.h"

#include "apriluiExport.h"

namespace aprilui
{
	class apriluiExport Animator : public Object
	{
	public:
		Animator(chstr type, chstr name, grectf rect);
		
		virtual bool isAnimated() = 0;
		
		void setDelay(float value) { mDelay = value; }
		
	protected:
		float mInitialDelay;
		float mDelay;
		
	};
}

#endif
