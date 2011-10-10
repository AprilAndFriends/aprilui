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

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectLabel.h"

namespace aprilui
{
	Label::Label(chstr name, grect rect) :
		LabelBase(),
		Object("Label", name, rect)
	{
		mText = "Label: " + name;
	}

	void Label::OnDraw()
	{
		Object::OnDraw();
		unsigned char alpha = getDerivedAlpha();
		if (!_isDerivedEnabled())
		{
			alpha /= 2;
		}
		LabelBase::_drawLabel(_getDrawRect(), alpha);
	}

	void Label::notifyEvent(chstr name, void* params)
	{
		if (name == "UpdateText")
		{
			setTextKey(mTextKey);
		}
		Object::notifyEvent(name, params);
	}
	
	void Label::setTextKey(chstr value)
	{
		LabelBase::setTextKey(value);
		setText(mDataset->getText(value));
	}
	
	hstr Label::getProperty(chstr name, bool* property_exists)
	{
		bool exists = false;
		hstr result = LabelBase::getProperty(name, &exists);
		if (!exists)
		{
			return Object::getProperty(name, property_exists);
		}
		if (property_exists != NULL)
		{
			*property_exists = exists;
		}
		return result;
	}

	bool Label::setProperty(chstr name, chstr value)
	{
		if (LabelBase::setProperty(name, value))
		{
			return true;
		}
        return Object::setProperty(name, value);
	}
	
}
