/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_MOVER_H
#define APRILUI_MOVER_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include "Animator.h"

namespace AprilUI
{
	namespace Animators
	{
		class AprilUIExport Mover : public Animator
		{
		public:
			Mover(chstr name);
			
			bool isAnimated();
			void setProperty(chstr name, chstr value);
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			void move(float dest_x, float dest_y, float time);
            
		protected:
			gvec2 mInitialS;
			gvec2 mAccel;
			gvec2 mSpeed;
			gvec2 mDest;
			
		};
	}
}

#endif
