/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_ALPHA_OSCILLATOR_H
#define APRILUI_ALPHA_OSCILLATOR_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport AlphaOscillator : public Animator
		{
		public:
			AlphaOscillator(chstr name);
			
			bool isAnimated();
			void setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
		protected:
			float mBaseline;
			float mAmplitude;
			float mSpeed;
			float mTimer;
			float mInitialAlpha;
			
		};
	}
}

#endif
