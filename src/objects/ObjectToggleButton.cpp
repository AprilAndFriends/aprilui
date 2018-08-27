/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Event.h"
#include "EventArgs.h"
#include "Image.h"
#include "ObjectToggleButton.h"

namespace aprilui
{
	ToggleButton::ToggleButton(chstr name, grectf rect) :
		ImageButton(name, rect)
	{
		_setTypeName("ToggleButton");
		mPushed = false;
	}

	void ToggleButton::OnDraw(gvec2f offset)
	{
		float alpha = getDerivedAlpha();
		april::Color color;
		color.a = (unsigned char)(alpha * 255);
		grectf rect = _getDrawRect() + offset;
		if (mPushed && mPushedImage != NULL)
		{
			mPushedImage->draw(rect, color);
		}
		else
		{
			mImage->draw(rect, color);
		}
	}

	bool ToggleButton::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isPointInside(x, y))
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
		if (isPointInside(x, y))
		{
			triggerEvent("Click", x, y, 0);
			return true;
		}
		return false;
	}
	
}
