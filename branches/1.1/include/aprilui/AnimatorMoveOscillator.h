/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_MOVE_OSCILLATOR_H
#define APRILUI_MOVE_OSCILLATOR_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport MoveOscillator : public Animator
		{
		public:
			MoveOscillator(chstr name);
			
			bool isAnimated();
			void notifyEvent(chstr eventName, void* params);
			void setProperty(chstr name, chstr value);
			void update(float k);
			
		protected:
			float mTimer;
			gvec2 mBaseline;
			gvec2 mAmplitude;
			gvec2 mSpeed;
			gvec2 mInitialPosition;
			
		};
	}
}

#endif
