/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_ROTATABLE_IMAGE_BOX_H
#define APRILUI_ROTATABLE_IMAGE_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectRotationImageBox.h"

namespace aprilui
{
	class apriluiExport RotatableImageBox : public RotationImageBox
	{
	public:
		RotatableImageBox(chstr name, grectf rect);
		
		float getRotationSpeed() { return mRotationSpeed; }
		void setRotationSpeed(float value) { mRotationSpeed = value; }
		void setAngle(float value) { mAngle = mDestAngle = value; }
		float getDestAngle() { return mDestAngle; }
		void setDestAngle(float value) { mDestAngle = value; }
		void rotate(float angle) { mDestAngle += angle; }
		bool isRotating();

		void update(float k);
		
	protected:
		float mDestAngle;
		float mRotationSpeed;
		
	};
}

#endif
