/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_MOVER_Y_H
#define APRILUI_MOVER_Y_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace AprilUI
{
	namespace Animators
	{
		class AprilUIExport MoverY : public Animator
		{
		public:
			MoverY(chstr name);
			
			void notifyEvent(chstr name, void* params);
			
			void update(float k);
			
		};
	}
}

#endif
