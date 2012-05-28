/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EARTHQUAKE_H
#define APRILUI_EARTHQUAKE_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport Earthquake : public Animator
		{
		public:
			Earthquake(chstr name);
			
			bool isAnimated();
			bool setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
		protected:
			gvec2 mInitial;
			float mIntensity;
			float mDuration;
			float mConstDuration;
			float mFreq;
			float mTimer;
			float mFreqTimer;
			
		};
	}
}

#endif
