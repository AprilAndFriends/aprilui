/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef ROTATABLE_IMAGE_BOX_H
#define ROTATABLE_IMAGE_BOX_H

#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "RotationImageBox.h"

namespace AprilUI
{
	class AprilUIExport RotatableImageBox : public RotationImageBox
	{
	protected:
		float mDestAngle;
		float mRotationSpeed;
	public:
		RotatableImageBox(chstr name,float x,float y,float w,float h);
		
		void setRotationSpeed(float speed) { mRotationSpeed=speed; }
		float getRotationSpeed() { return mRotationSpeed; }
		void setAngle(float angle) { mAngle=mDestAngle=angle; }
		void setDestAngle(float angle) { mDestAngle=angle; }
		float getDestAngle() { return mDestAngle; }
		void rotate(float angle) { mDestAngle+=angle; }
		bool isRotating();

		void update(float k);
	};
	
}

#endif
