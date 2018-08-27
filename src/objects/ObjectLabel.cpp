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

#include "Dataset.h"
#include "ObjectLabel.h"

namespace aprilui
{
	Label::Label(chstr name, grectf rect) :
		Object("Label", name, rect),
		LabelBase(name)
	{
	}

	Dataset* Label::getDataset()
	{
		return Object::getDataset();
	}

	void Label::OnDraw(gvec2f offset)
	{
		Object::OnDraw(offset);
		float alpha = getDerivedAlpha();
		if (!isDerivedEnabled())
		{
			alpha /= 2;
		}
		LabelBase::_drawLabel(_getDrawRect() + offset, alpha);
	}

	void Label::notifyEvent(chstr name, void* params)
	{
		if (name == "UpdateText")
		{
			setTextKey(mTextKey);
		}
		if (name == "onLocalizationChanged")
		{
			if (mTextKey != "")
			{
				setTextKey(mTextKey);
			}
		}
		Object::notifyEvent(name, params);
	}

	bool Label::setProperty(chstr name, chstr value)
	{
		LabelBase::setProperty(name, value);
		Object::setProperty(name, value);
		if (name == "textkey")
			setTextKey(value);
		return 1;
	}
	
}
