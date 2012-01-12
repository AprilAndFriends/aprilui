/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_MOVER_H
#define APRILUI_MOVER_H

#include <hltypes/hstring.h>

#include "ObjectCallbackObject.h"
#include <gtypes/Vector2.h>
#include "Animator.h"

namespace AprilUI
{
	namespace Animators
	{
		class AprilUIExport Mover : public Animator
		{
			gtypes::Vector2 mInitialS;
			gtypes::Vector2 mAccel;
			gtypes::Vector2 mSpeed;
			gtypes::Vector2 mDest;
		public:
			Mover(chstr name);
			
			void notifyEvent(chstr event_name,void* params);
			void setProperty(chstr name,chstr value);
			void update(float k);
			void move(float dest_x,float dest_y,float time);
		};
	}
}

#endif