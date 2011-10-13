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
#include "ObjectLabelBase.h"
#include "ObjectTextImageButton.h"

namespace aprilui
{
	TextImageButton::TextImageButton(chstr name, grect rect) :
		LabelBase(),
		ImageButton(name, rect)
	{
		_setTypeName("TextImageButton");
		mText = "TextImageButton: " + name;
		mUseDisabledColor = true;
	}

	void TextImageButton::OnDraw()
	{
		ImageButton::OnDraw();
		unsigned char alpha = getDerivedAlpha();
		if (mUseDisabledColor && !_isDerivedEnabled() || mImage == NULL && mNormalImage == NULL && mPushedImage == NULL && mPushed)
		{
			alpha /= 2;
		}
		LabelBase::_drawLabel(_getDrawRect(), alpha);
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
			return Object::getProperty(name, property_exists);
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
		if (name == "text_key")	setTextKey(value);
		else if (name == "textkey")
		{
			aprilui::log("WARNING: \"textkey=\" is deprecated. Use \"text_key=\" instead."); // DEPRECATED
			setTextKey(value);
		}
		else if (name == "use_disabled_color")	setUseDisabledColor(value);
        else if (LabelBase::setProperty(name, value)) {}
        else return ImageButton::setProperty(name, value);
        return true;
	}
	
}
