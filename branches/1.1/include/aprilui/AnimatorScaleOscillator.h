/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_SCALE_OSCILLATOR_H
#define APRILUI_SCALE_OSCILLATOR_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport ScaleOscillator : public Animator
		{
		public:
			ScaleOscillator(chstr name);
			
			bool isAnimated();
			void notifyEvent(chstr eventName, void* params);
			bool setProperty(chstr name, chstr value);
			void update(float k);
			
		protected:
			float mBaseline;
			float mAmplitude;
			float mSpeed;
			float mTimer;
			gvec2 mInitialSize;
			
		};
	}
}

#endif
