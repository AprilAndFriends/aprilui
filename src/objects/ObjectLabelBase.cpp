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
		mUseEffectColor = false;
		mUseEffectParameter = false;
		mEffectColor = april::Color::Black;
		mDrawOffset = gvec2f();
		mTextFormatting = true;
		mText = "LabelBase: " + name;
	}
	
	void LabelBase::_drawLabel(grectf rect, float alpha)
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
		if (!mTextFormatting)
		{
			text = "[-]" + text;
		}
		hstr colorCode = "";
		if (mUseEffectColor)
		{
			colorCode += mEffectColor.hex();
		}
		if (mUseEffectParameter)
		{
			colorCode += "," + mEffectParameter;
		}
		if (colorCode != "")
		{
			colorCode = ":" + colorCode;
		}
		if (mFontEffect == atres::TextEffect::Border)
		{
			text = "[b" + colorCode + "]" + text;
		}
		else if (mFontEffect == atres::TextEffect::Shadow)
		{
			text = "[s" + colorCode + "]" + text;
		}
		april::Color color(mTextColor);
		color.a = (unsigned char)(color.a * alpha);
		atres::renderer->drawText(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
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
			setFontEffect(atres::TextEffect::None);
			mUseEffectColor = false;
			harray<hstr> values = value.split(":", 1, true);
			if (values.size() > 0)
			{
				if (values[0] == "none")		setFontEffect(atres::TextEffect::None);
				else if (values[0] == "shadow")	setFontEffect(atres::TextEffect::Shadow);
				else if (values[0] == "border")	setFontEffect(atres::TextEffect::Border);
				else
				{
					logMessage("WARNING: 'effect=' does not support value '" + values[0] + "'.");
					return false;
				}
				if (values.size() > 1)
				{
					values = values[1].split(",", 1);
					if (values[0].isHex() && (values[0].size() == 6 || values[0].size() == 8))
					{
						mUseEffectColor = true;
						mEffectColor = values[0];
					}
					else if (values[0] != "")
					{
						logMessage("WARNING: 'effect=' is using invalid color modifier '" + values[0] + "'.");
						return false;
					}
					if (values.size() > 1)
					{
						mUseEffectParameter = true;
						mEffectParameter = values[1];
					}
				}
			}
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
		hstr newTextKey = value; // because value is a chstr which could reference this->textKey itself
		mText = getDataset()->getText(newTextKey);
		mTextKey = newTextKey;
	}

	void LabelBase::trySetTextKey(chstr value)
	{
		if (value != mTextKey)
		{
			this->setTextKey(value);
		}
	}

}
