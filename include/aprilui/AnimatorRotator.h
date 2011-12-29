/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can rotate objects.

#ifndef APRILUI_ROTATOR_H
#define APRILUI_ROTATOR_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport Rotator : public Animator
		{
		public:
			Rotator(chstr name);
			~Rotator();
			static Animator* createInstance(chstr name);
			
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
		};
	}
}

#endif
