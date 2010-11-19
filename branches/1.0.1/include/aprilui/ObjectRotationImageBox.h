/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_ROTATION_IMAGE_BOX_H
#define APRILUI_ROTATION_IMAGE_BOX_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectImageBox.h"

namespace AprilUI
{
	class AprilUIExport RotationImageBox : public ImageBox
	{
	protected:
		float mAngle;
		void OnDraw(float offset_x,float offset_y);
	public:
		RotationImageBox(chstr name,float x,float y,float w,float h);

		virtual void setAngle(float angle) { mAngle=angle; }
		float getAngle() { return mAngle; }
		bool angleEquals(float angle);
		void setProperty(chstr name,chstr value);
	};
	
}

#endif
