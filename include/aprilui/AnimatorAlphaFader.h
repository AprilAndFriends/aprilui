/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_ALPHA_FADER_H
#define APRILUI_ALPHA_FADER_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport AlphaFader : public Animator
		{
		public:
			AlphaFader(chstr name);
			
			bool isAnimated();
			bool setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			void fade(float dest, float time);
			void reset();
			void setResetHack(bool value) { mResetHack=value; }
			
			void update(float k);
			
		protected:
			float mInitialAlpha;
			float mInitialSpeed;
			float mSpeed;
			float mInitialAccel;
			float mAccel;
			float mInitialDestAlpha;
			float mDestAlpha;
			bool mResetHack;
			
		};
	}
}

#endif
