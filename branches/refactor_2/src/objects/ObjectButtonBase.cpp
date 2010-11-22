/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "AprilUI.h"
#include "ObjectButtonBase.h"

namespace AprilUI
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
		if (mHover)
		{
			mPushed = true;
			return true;
		}
		return false;
	}

	bool ButtonBase::OnMouseUp(float x, float y, int button)
	{
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
