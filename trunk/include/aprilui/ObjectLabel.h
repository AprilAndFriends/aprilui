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

#include "apriluiExport.h"
#include "ObjectLabelBase.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport Label : public Object, public LabelBase
	{
	public:
		Label(chstr name, grect rect);
		
		float getAngle();
		chstr getTextXey() { return mTextKey; }
		void setTextKey(chstr key);
		
		void notifyEvent(chstr name, void* params);
		void setProperty(chstr name, chstr value);
		
	protected:
		hstr mTextKey;
		void OnDraw(gvec2 offset = gvec2());
		
	};
	
}

#endif
