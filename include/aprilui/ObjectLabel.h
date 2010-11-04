/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_LABEL_H
#define APRILUI_LABEL_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "AprilUIExport.h"
#include "ObjectLabelBase.h"
#include "Object.h"

namespace AprilUI
{
	class AprilUIExport Label : public Object, public LabelBase
	{
	protected:
		hstr mTextKey;
		void OnDraw(float offset_x,float offset_y);
	public:
		Label(chstr name,grect rect);
		
		void setTextKey(chstr key);
		chstr getTextXey() { return mTextKey; }
		void setProperty(chstr name,chstr value);
		void notifyEvent(chstr event_name,void* params);
	};
	
}

#endif
