/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_MOVER_H
#define APRILUI_MOVER_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport Mover : public Animator
		{
		public:
			Mover(chstr name);
			
			bool isAnimated();
			bool setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			void move(float dest_x, float dest_y, float time);
            
		protected:
			gvec2 mInitialS;
			gvec2 mAccel;
			gvec2 mSpeed;
			gvec2 mDest, mStart, mT;
			
		};
	}
}

#endif
