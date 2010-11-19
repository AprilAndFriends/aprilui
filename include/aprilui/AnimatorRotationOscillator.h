/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_ROTATION_OSCILLATOR_H
#define APRILUI_ROTATION_OSCILLATOR_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace AprilUI
{
	namespace Animators
	{
		class AprilUIExport RotationOscillator : public Animator
		{
			float mBaseline,mAmplitude,mSpeed,mTimer,mInitialAngle;
		public:
			RotationOscillator(chstr name);

			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
		};
	}
}

#endif
