/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef APRILUI_ROTATION_IMAGE_BOX_H
#define APRILUI_ROTATION_IMAGE_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport RotationImageBox : public ImageBox
	{
	public:
		RotationImageBox(chstr name, grect rect);

		float getAngle() { return mAngle; }
		virtual void setAngle(float value) { mAngle = value; }
		bool angleEquals(float angle);
		bool setProperty(chstr name, chstr value);
		
	protected:
		float mAngle;
		
		void OnDraw(gvec2 offset = gvec2());
		
	};
	
}

#endif
