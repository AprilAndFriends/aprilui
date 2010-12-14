/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_TEXT_BUTTON_H
#define APRILUI_TEXT_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class apriluiExport TextButton : public Label
	{
	public:
		TextButton(chstr name, grect rect);
		
		April::Color getHoverTextColor() { return mHoverTextColor; }
		void setHoverTextColor(April::Color value) { mHoverTextColor = value; }
		void setHoverTextColor(chstr value) { mHoverTextColor.setColor(value); }
		April::Color getPushedTextColor() { return mPushedTextColor; }
		void setPushedTextColor(April::Color value) { mPushedTextColor = value; }
		void setPushedTextColor(chstr value) { mPushedTextColor.setColor(value); }
		April::Color getDisabledTextColor() { return mDisabledTextColor; }
		void setDisabledTextColor(April::Color value) { mDisabledTextColor = value; }
		void setDisabledTextColor(chstr value) { mDisabledTextColor.setColor(value); }

		void setTextKey(chstr key);
		void setProperty(chstr name, chstr value);
		
		bool OnMouseDown(float x, float y, int button);
		bool OnMouseUp(float x, float y, int button);
		
	protected:
		bool mPushed;
		bool mBackground;
		April::Color mHoverTextColor;
		April::Color mPushedTextColor;
		April::Color mDisabledTextColor;
		
		void OnDraw(gvec2 offset = gvec2());
		
	};
}

#endif
