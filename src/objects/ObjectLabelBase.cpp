/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.75
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <atres/atres.h>
#include <gtypes/Rectangle.h>
#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	LabelBase::LabelBase()
	{
		mHorzFormatting = atres::CENTER_WRAPPED;
		mVertFormatting = atres::CENTER;
		mFontEffect = atres::NONE;
		mTextFormatting = true;
		mText = "";
		mTextKey = "";
	}

	LabelBase::~LabelBase()
	{
	}

	void LabelBase::_drawLabel(grect rect, april::Color color)
	{
		color *= mTextColor;
		if (aprilui::isDebugEnabled())
		{
			april::rendersys->drawFilledRect(rect, april::Color(APRIL_COLOR_BLACK, color.a / 2));
			april::rendersys->drawRect(rect, april::Color(APRIL_COLOR_WHITE, color.a / 2));
		}
		if (mText.size() == 0)
		{
			return;
		}
		hstr text = mText;
		switch (mFontEffect)
		{
		case atres::BORDER:
			text = "[b]" + text;
			break;
		case atres::SHADOW:
			text = "[s]" + text;
			break;
		default:
			break;
		}
		if (mTextFormatting)
		{
			atres::renderer->drawText(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
		}
		else
		{
			atres::renderer->drawTextUnformatted(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
		}
	}

	hstr LabelBase::_parseTextKey(chstr key)
	{
		if (!key.starts_with("{"))
		{
			if (!_hasTextKey(key))
			{
				aprilui::log(hsprintf("WARNING! Text key '%s' does not exist", key.c_str()));
			}
			return _getTextEntry(key);
		}
		int index = key.find_first_of('}');
		if (index < 0)
		{
			aprilui::log(hsprintf("WARNING! Error while trying to parse formatted key '%s'.", key.c_str()));
			return key;
		}
		harray<hstr> args;
		hstr format = key(1, index - 1);
		hstr argString = key(index + 1, key.size() - index - 1).trim(' ');
		if (!_processArgs(argString, args))
		{
			aprilui::log(hsprintf("- while processing args: '%s' with args '%s'.", format.c_str(), argString.c_str()));
			return key;
		}
		hstr preprocessedFormat;
		harray<hstr> preprocessedArgs;
		if (!_preprocessFormat(format, args, preprocessedFormat, preprocessedArgs))
		{
			aprilui::log(hsprintf("- while preprocessing format: '%s' with args '%s'.", format.c_str(), argString.c_str()));
			return key;
		}
		hstr result;
		if (!_processFormat(preprocessedFormat, preprocessedArgs, result))
		{
			aprilui::log(hsprintf("- while processing format: '%s' with args '%s'.", format.c_str(), argString.c_str()));
			return key;
		}
		return result;
	}


	bool LabelBase::_processArgs(chstr argString, harray<hstr>& args)
	{
		args.clear();
		// splittings args
		hstr string = argString;
		harray<hstr> keys;
		int openIndex;
		int closeIndex;
		while (string.size() > 0)
		{
			openIndex = string.find_first_of('{');
			closeIndex = string.find_first_of('}');
			if (openIndex < 0 && closeIndex < 0)
			{
				args += _getTextEntries(string.split(" ", -1, true));
				break;
			}
			if (openIndex < 0 || closeIndex < 0)
			{
				aprilui::log("WARNING! '{' without '}' or '}' without '{'");
				return false;
			}
			if (closeIndex < openIndex)
			{
				aprilui::log("WARNING! '}' before '{'");
				return false;
			}
			// getting all args before the {
			args += _getTextEntries(string(0, openIndex).split(" ", -1, true));
			// getting args inside of {}
			args += string(openIndex + 1, closeIndex - openIndex - 1);
			// rest of the args
			string = string(closeIndex + 1, string.size() - closeIndex - 1);
		}
		return true;
	}

	bool LabelBase::_preprocessFormat(chstr format, harray<hstr> args, hstr& preprocessedFormat, harray<hstr>& preprocessedArgs)
	{
		preprocessedFormat = "";
		preprocessedArgs.clear();
		// preprocessing of format string and args
		hstr string = format;
		int index;
		hstr arg;
		harray<int> indexes;
		while (string.size() > 0)
		{
			index = string.find_first_of('%');
			if (index < 0)
			{
				preprocessedFormat += string;
				break;
			}
			if (index >= string.size() - 1)
			{
				aprilui::log("WARNING! Last character is '%'");
				return false;
			}
			if (string[index + 1] == '%') // escaped "%", continue processing
			{
				preprocessedFormat += string(0, index + 2);
				string = string(index + 2, string.size() - index - 2);
				continue;
			}
			if (string[index + 1] == 's') // %s, not processing that now
			{
				if (args.size() == 0)
				{
					aprilui::log("WARNING! Not enough args");
					return false;
				}
				preprocessedFormat += string(0, index + 2);
				string = string(index + 2, string.size() - index - 2);
				preprocessedArgs += args.pop_first();
				continue;
			}
			if (string[index + 1] == 'f')
			{
				if (args.size() == 0)
				{
					aprilui::log("WARNING! Not enough args");
					return false;
				}
				preprocessedFormat += string(0, index) + args.pop_first();
				string = string(index + 2, string.size() - index - 2);
			}
		}
		preprocessedArgs += args; // remaining args
		return true;
	}

	bool LabelBase::_processFormat(chstr format, harray<hstr> args, hstr& result)
	{
		result = "";
		// preprocessing of format string and args
		hstr string = format;
		harray<int> indexes;
		if (!_getFormatIndexes(format, indexes))
		{
			return false;
		}
		if (args.size() < indexes.size())
		{
			aprilui::log("WARNING! Not enough args");
			return false;
		}
		if (indexes.size() > args.size())
		{
			aprilui::log("WARNING! Too many args");
			return false;
		}
		foreach (int, it, indexes)
		{
			result += string(0, (*it));
			result += args.pop_first();
			string = string((*it) + 2, string.size() - (*it) - 2);
		}
		result = result.replace("%%", "%");
		return true;
	}

	bool LabelBase::_getFormatIndexes(chstr format, harray<int>& indexes)
	{
		indexes.clear();
		// finding formatting indexes
		hstr string = format;
		int index;
		int currentIndex = 0;
		while (string.size() > 0)
		{
			index = string.find_first_of('%');
			if (index < 0)
			{
				break;
			}
			if (index >= string.size() - 1)
			{
				aprilui::log("WARNING! Last character is '%'");
				return false;
			}
			if (string[index + 1] == '%') // escaped "%", use just one "%".
			{
				string = string(index + 2, string.size() - index - 2);
				currentIndex += index + 2;
				continue;
			}
			if (string[index + 1] != 's')
			{
				aprilui::log(hsprintf("WARNING! Unsupported formatting '%%%c'", string[index + 1]));
				return false;
			}
			indexes += currentIndex + index;
			string = string(index + 2, string.size() - index - 2);
			currentIndex = 0;
		}
		return true;
	}

	harray<hstr> LabelBase::_getTextEntries(harray<hstr> keys)
	{
		harray<hstr> result;
		foreach (hstr, it, keys)
		{
			if (!_hasTextKey(*it))
			{
				aprilui::log(hsprintf("WARNING! Text key '%s' does not exist", (*it).c_str()));
			}
			result += _getTextEntry(*it);
		}
		return result;
	}

	hstr LabelBase::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "font")		return getFont();
		if (name == "text")		return getText();
		if (name == "text_key")	return getTextKey();
		if (name == "horz_formatting")
		{
			if (mHorzFormatting == atres::LEFT)				return "left";
			if (mHorzFormatting == atres::RIGHT)			return "right";
			if (mHorzFormatting == atres::CENTER)			return "center";
			if (mHorzFormatting == atres::LEFT_WRAPPED)		return "left_wrapped";
			if (mHorzFormatting == atres::RIGHT_WRAPPED)	return "right_wrapped";
			if (mHorzFormatting == atres::CENTER_WRAPPED)	return "center_wrapped";
			if (mHorzFormatting == atres::JUSTIFIED)		return "justified";
		}
		if (name == "vert_formatting")
		{
			if (mVertFormatting == atres::TOP)		return "top";
			if (mVertFormatting == atres::CENTER)	return "center";
			if (mVertFormatting == atres::BOTTOM)	return "bottom";
		}
		if (name == "text_color")	return getTextColor().hex();
		if (name == "effect")
		{
			if (mFontEffect == atres::SHADOW)	return "shadow";
			if (mFontEffect == atres::BORDER)	return "border";
			return "none";
		}
		if (name == "offset_x")	return mDrawOffset.x;
		if (name == "offset_y")	return mDrawOffset.y;
		if (property_exists != NULL)
		{
			*property_exists = false;
		}
		return "";
	}
	
	bool LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font")				setFont(value);
		else if (name == "text_key")	setTextKey(value);
		else if (name == "textkey")
		{
			aprilui::log("WARNING: \"textkey=\" is deprecated. Use \"text_key=\" instead."); // DEPRECATED
			setTextKey(value);
		}
		else if (name == "text")		setText(value);
		else if (name == "horz_formatting")
		{
			if (value == "left")				setHorzFormatting(atres::LEFT);
			else if (value == "right")			setHorzFormatting(atres::RIGHT);
			else if (value == "center")			setHorzFormatting(atres::CENTER);
			else if (value == "left_wrapped")	setHorzFormatting(atres::LEFT_WRAPPED);
			else if (value == "right_wrapped")	setHorzFormatting(atres::RIGHT_WRAPPED);
			else if (value == "center_wrapped")	setHorzFormatting(atres::CENTER_WRAPPED);
			else if (value == "justified")		setHorzFormatting(atres::JUSTIFIED);
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")			setVertFormatting(atres::TOP);
			else if (value == "center")	setVertFormatting(atres::CENTER);
			else if (value == "bottom")	setVertFormatting(atres::BOTTOM);
		}
		else if (name == "text_color")	setTextColor(value);
		else if (name == "color")
		{
			throw hl_exception("LabelBase instance using \"color=\" which is conflicted with TextImageButton's color and cannot be used! Maybe you meant \"text_color=\"?");
		}
		else if (name == "effect")
		{
			if (value == "none")		setFontEffect(atres::NONE);
			else if (value == "shadow")	setFontEffect(atres::SHADOW);
			else if (value == "border")	setFontEffect(atres::BORDER);
		}
		else if (name == "offset_x")	mDrawOffset.x = (float)value;
		else if (name == "offset_y")	mDrawOffset.y = (float)value;
		else return false;
		return true;
	}
	
	void LabelBase::setText(chstr value)
	{
		mText = value;
		mTextKey = "";
	}

}
