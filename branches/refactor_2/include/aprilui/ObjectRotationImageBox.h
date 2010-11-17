/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_ROTATION_IMAGE_BOX_H
#define APRILUI_ROTATION_IMAGE_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectImageBox.h"

namespace AprilUI
{
	class AprilUIExport RotationImageBox : public ImageBox
	{
	public:
		RotationImageBox(chstr name, grect rect);

		float getAngle() { return mAngle; }
		virtual void setAngle(float value) { mAngle = value; }
		bool angleEquals(float angle);
		void setProperty(chstr name, chstr value);
		
	protected:
		float mAngle;
		
		void OnDraw(gvec2 offset = gvec2());
		
	};
	
}

#endif
