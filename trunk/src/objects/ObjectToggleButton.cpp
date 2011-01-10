/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Event.h"
#include "EventArgs.h"
#include "Image.h"
#include "ObjectToggleButton.h"

namespace aprilui
{
	ToggleButton::ToggleButton(chstr name, grect rect) :
		ImageButton(name, rect)
	{
		_setTypeName("ToggleButton");
		mPushed = false;
	}

	void ToggleButton::OnDraw(gvec2 offset, gvec2 center)
	{
		april::Color color;
		color.a = getDerivedAlpha();
		grect rect = mRect + offset;
		if (mPushed && mPushedImage != NULL)
		{
			mPushedImage->draw(rect, color, mAngle);
		}
		else
		{
			mImage->draw(rect, color, mAngle);
		}
	}

	bool ToggleButton::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			mPushed = !mPushed;
			Event* event = (mPushed ? mEvents["Toggle"] : mEvents["Untoggle"]);
			if (event != NULL)
			{
				EventArgs args(this, x, y);
				event->execute(&args);
			}
			return true;
		}
		return false;
	}

	bool ToggleButton::OnMouseUp(float x, float y, int button)
	{
		if (Object::OnMouseUp(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			triggerEvent("Click", x, y, 0);
			return true;
		}
		return false;
	}
	
}
