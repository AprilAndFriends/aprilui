/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_SCALER_H
#define APRILUI_SCALER_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport Scaler : public Animator
		{
		public:
			Scaler(chstr name);
			
			bool isAnimated();
			bool setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			void scale(float dest_w, float dest_h, float time);
            
		protected:
			gvec2f mInitialSize;
			gvec2f mInitialS;
			gvec2f mAccel;
			gvec2f mSpeed;
			gvec2f mDest,mStart;
			gvec2f mT;
		};
	}
}

#endif
