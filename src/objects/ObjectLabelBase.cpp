/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <atres/atres.h>
#include <atres/Utility.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hexception.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	LabelBase::LabelBase(chstr name) :
		   mTextColor(255, 255, 255, 255)
	{
		mHorzFormatting = atres::Horizontal::CenterWrapped;
		mVertFormatting = atres::Vertical::Center;
		mFontEffect = atres::TextEffect::None;
		mDrawOffset = gvec2();
		mTextFormatting = true;
		mText = "LabelBase: " + name;
	}
	
	void LabelBase::_drawLabel(grect rect, float alpha)
	{
#ifdef _DEBUG
		if (aprilui::isDebugMode())
		{
			april::rendersys->drawFilledRect(rect, april::Color(0, 0, 0, (int) (alpha/2)));
		}
#endif
		if (mText.size() == 0)
		{
			return;
		}
		hstr text = mText;
		if (mFontEffect == atres::TextEffect::Border)
		{
			text = "[b]" + text;
		}
		else if (mFontEffect == atres::TextEffect::Shadow)
		{
			text = "[b]" + text;
		}
		april::Color color(mTextColor);
		color.a = (unsigned char)(color.a * alpha);
		if (mTextFormatting)
		{
			atres::renderer->drawText(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
		}
		else
		{
			atres::renderer->drawTextUnformatted(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
		}
	}

	bool LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font") setFont(value);
		else if (name == "text") setText(value);
		else if (name == "wrap_text")
		{
			logMessage("\"wrap_text=\" is deprecated. Use \"horz_formatting=\" instead.");
		}
		else if (name == "horz_formatting")
		{
			if (value == "left")                setHorzFormatting(atres::Horizontal::Left);
			else if (value == "right")          setHorzFormatting(atres::Horizontal::Right);
			else if (value == "center")         setHorzFormatting(atres::Horizontal::Center);
			else if (value == "left_wrapped")   setHorzFormatting(atres::Horizontal::LeftWrapped);
			else if (value == "right_wrapped")  setHorzFormatting(atres::Horizontal::RightWrapped);
			else if (value == "center_wrapped") setHorzFormatting(atres::Horizontal::CenterWrapped);
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")         setVertFormatting(atres::Vertical::Top);
			else if (value == "center") setVertFormatting(atres::Vertical::Center);
			else if (value == "bottom") setVertFormatting(atres::Vertical::Bottom);
		}
		else if (name == "color") setTextColor(value);
		else if (name == "effect")
		{
			if (value == "none")           setFontEffect(atres::TextEffect::None);
			else if (value == "shadow")    setFontEffect(atres::TextEffect::Shadow);
			else if (value == "border")    setFontEffect(atres::TextEffect::Border);
		}
		else if (name == "offset_x") mDrawOffset.x = (float)value;
		else if (name == "offset_y") mDrawOffset.y = (float)value;
		return 1;
	}

	void LabelBase::notifyEvent(chstr name, void* params)
	{
		if (name == "onLocalizationChanged")
		{
			if (mTextKey != "")
			{
				setTextKey(mTextKey);
			}
		}
	}
	
	void LabelBase::setText(chstr value)
	{
		mText = value;
		mTextKey = "";
	}

	void LabelBase::setTextKey(chstr value)
	{
		if (value == "")
			mTextKey = "";
		mText = getDataset()->getText(value);
		mTextKey = value;
	}

	void LabelBase::trySetTextKey(chstr value)
	{
		if (value != mTextKey)
		{
			this->setTextKey(value);
		}
	}

}
