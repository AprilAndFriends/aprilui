/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectLabelBase.h"
#include "ObjectTextImageButton.h"

namespace aprilui
{
	TextImageButton::TextImageButton(chstr name, grect rect) :
		LabelBase(),
		ImageButton(name, rect)
	{
		mText = "TextImageButton: " + name;
		mUseDisabledColor = true;
	}

	TextImageButton::~TextImageButton()
	{
	}

	Object* TextImageButton::createInstance(chstr name, grect rect)
	{
		return new TextImageButton(name, rect);
	}

	hstr TextImageButton::getName()
	{
		return ImageButton::getName();
	}

	void TextImageButton::OnDraw()
	{
		ImageButton::OnDraw();
		april::Color color = _getDrawColor();
		if (mUseDisabledColor && !_isDerivedEnabled() || mImage == NULL && mNormalImage == NULL && mPushedImage == NULL && mPushed)
		{
			color.a /= 2;
		}
		LabelBase::_drawLabel(_getDrawRect(), color);
	}
    
    void TextImageButton::setTextKey(chstr value)
	{
		LabelBase::setTextKey(value);
		setText(mDataset->getText(value));
	}

	hstr TextImageButton::getProperty(chstr name, bool* property_exists)
	{
        bool exists = false;
        hstr result = LabelBase::getProperty(name, &exists);
        if (!exists)
		{
			return ImageButton::getProperty(name, property_exists);
		}
        if (property_exists != NULL)
		{
			*property_exists = exists;
		}
		if (name == "use_disabled_color")	return mUseDisabledColor;
        return result;
    }
    
	bool TextImageButton::setProperty(chstr name,chstr value)
	{
		if (name == "use_disabled_color")	setUseDisabledColor(value);
        else if (LabelBase::setProperty(name, value)) {}
        else return ImageButton::setProperty(name, value);
        return true;
	}
	
}