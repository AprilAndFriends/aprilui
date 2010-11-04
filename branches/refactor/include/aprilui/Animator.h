/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_ANIMATOR_H
#define APRILUI_ANIMATOR_H

#include "AprilUIExport.h"
#include "Object.h"

namespace AprilUI
{

	class AprilUIExport Animator : public Object
	{
	public:
		Animator(chstr type_name,chstr name,float x,float y,float w,float h);
	};
}

#endif
