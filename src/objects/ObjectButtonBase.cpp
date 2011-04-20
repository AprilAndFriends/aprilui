/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <april/Keys.h>
#include "aprilui.h"
#include "ObjectButtonBase.h"

namespace aprilui
{
	ButtonBase::ButtonBase()
	{
		mPushed = false;
		mHover = false;
	}
	
	void ButtonBase::update(float k)
	{
		mHover = isCursorInside();
	}

	bool ButtonBase::OnMouseDown(float x, float y, int button)
	{
        if (button != april::AK_LBUTTON) return 0;
		mHover = isCursorInside();
		if (mHover)
		{
			mPushed = true;
			return true;
		}
		return false;
	}

	bool ButtonBase::OnMouseUp(float x, float y, int button)
	{
        if (button != april::AK_LBUTTON) return 0;
		mHover = isCursorInside();
		if (mPushed && mHover)
		{
			mPushed = false;
			return true;
		}
		mPushed = false;
		return false;
	}

	void ButtonBase::OnMouseMove(float x, float y)
	{
		mHover = isCursorInside();
	}
	
}
