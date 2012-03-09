/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Keys.h>
#include <atres/atres.h>
#include <atres/FontResource.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>
#include <april/Window.h>

#include "aprilui.h"
#include "Dataset.h"
#include "ObjectEditBox.h"

#define UNICODE_CHAR_SPACE 0x20

namespace aprilui
{
	EditBox::EditBox(chstr name, grect rect) :
		Label(name, rect)
	{
		mText = "";
		mHorzFormatting = atres::LEFT;
		mTextFormatting = false;
		mPushed = false;
		mMaxLength = 0;
		mPasswordChar = '\0';
		mCursorIndex = 0;
		mOffsetIndex = 0;
		mCtrlMode = false;
		mFilter = "";
		mBlinkTimer = 0.0f;
        mBackground = true;
		/// TODO - remove
		mSpaceHack = false;
	}

	EditBox::~EditBox()
	{
	}

	Object* EditBox::createInstance(chstr name, grect rect)
	{
		return new EditBox(name, rect);
	}

	void EditBox::update(float time)
	{
		Label::update(time);
		mBlinkTimer += time * 2;
		mBlinkTimer = (mBlinkTimer - (int)mBlinkTimer);
	}

	void EditBox::OnDraw()
	{
		//////////////
		// TODO - remove this hack, fix it in ATRES
		if (mSpaceHack)
		{
			if (mUnicodeChars.size() > 0 && mUnicodeChars[0] == UNICODE_CHAR_SPACE)
			{
				while (mUnicodeChars.size() > 0 && mUnicodeChars[0] == UNICODE_CHAR_SPACE)
				{
					mUnicodeChars.remove_at(0);
				}
				mText = unicode_to_utf8(mUnicodeChars);
			}
		}
		//////////////
		grect rect = _getDrawRect();
        april::Color color = APRIL_COLOR_BLACK;
        if (mBackground)
        {
            if (!mPushed)
            {
                color.a = 191;
            }
            color.a = getDerivedAlpha();
            april::rendersys->drawColoredQuad(grect(rect.x, rect.y, rect.w, rect.h), color);
			color = april::Color(mTextColor, color.a);
            april::rendersys->drawQuad(grect(rect.x, rect.y, rect.w, rect.h), color);
        }
		hstr text = mText;
		harray<unsigned int> unicodeChars = mUnicodeChars;
		if (mPasswordChar != '\0' && mText != "")
		{
			mText = hstr(mPasswordChar, mUnicodeChars.size());
			mUnicodeChars = _convertToUnicodeChars(mText);
		}
		while (mOffsetIndex > 0 && mOffsetIndex >= mCursorIndex)
		{
			mOffsetIndex = hmax(0, mCursorIndex - 5);
		}
		//////////////
		// TODO - remove this hack, fix it in ATRES
		while (mOffsetIndex < mUnicodeChars.size() - 1 && mUnicodeChars[mOffsetIndex] == UNICODE_CHAR_SPACE)
		{
			mOffsetIndex++;
		}
		//////////////
		rect.w -= 12;
		int count;
		while (true)
		{
			mText = unicode_to_utf8(mUnicodeChars(mOffsetIndex, mUnicodeChars.size() - mOffsetIndex));
			count = atres::renderer->getTextCountUnformatted(mFontName, mText, rect.w);
			count = _convertToUnicodeChars(mText(0, count)).size();
			if (mOffsetIndex > mCursorIndex)
			{
				mOffsetIndex = mCursorIndex;
			}
			else if (mOffsetIndex < mCursorIndex - count)
			{
				mOffsetIndex = mCursorIndex - count;
			}
			else
			{
				break;
			}
		}
		Label::OnDraw();
		if (mDataset != NULL && mDataset->getFocusedObject() == this && mBlinkTimer < 0.5f)
		{
			mText = unicode_to_utf8(mUnicodeChars(mOffsetIndex, mCursorIndex - mOffsetIndex));
			rect.x += atres::renderer->getTextWidthUnformatted(mFontName, mText);
			float h = atres::renderer->getFontLineHeight(mFontName);
			if (mHorzFormatting == atres::CENTER || mHorzFormatting == atres::CENTER_WRAPPED)
			{
				float w = atres::renderer->getTextWidthUnformatted(mFontName, text);
				rect.x += (mRect.w - w) / 2;
			}
			else if (mHorzFormatting == atres::RIGHT || mHorzFormatting == atres::RIGHT_WRAPPED)
			{
				float w = atres::renderer->getTextWidthUnformatted(mFontName, text);
				rect.x += mRect.w - w;
			}
			rect.y += (rect.h - h) / 2 + 2;
			rect.w = 2;
			rect.h = h - 4;
			april::rendersys->drawColoredQuad(rect, _getDrawColor() * mTextColor);
		}
		mText = text;
		mUnicodeChars = unicodeChars;
	}
    
    hstr EditBox::getProperty(chstr name, bool* property_exists)
    {
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
        if (name == "max_length")		return getMaxLength();
		if (name == "password_char")	return getPasswordChar();
		if (name == "filter")			return getFilter();
        if (name == "background")		return mBackground;
		if (name == "space_hack")		return mSpaceHack;
        return Label::getProperty(name, property_exists);
    }
	
	bool EditBox::setProperty(chstr name, chstr value)
	{
		if      (name == "max_length")		setMaxLength(value);
		else if (name == "password_char")	setPasswordChar(value.c_str()[0]);
		else if (name == "filter")			setFilter(value);
        else if (name == "background")		mBackground = (bool)value;
		else if (name == "space_hack")		mSpaceHack = (bool)value;
        else return Label::setProperty(name, value);
        return true;
	}
	
	void EditBox::setCursorIndex(int cursorIndex)
	{
		mCursorIndex = hclamp(cursorIndex, 0, mUnicodeChars.size());
		mBlinkTimer = 0.0f;
	}
	
	void EditBox::setCursorIndexAt(float x, float y)
	{
		hstr text;
		if (mPasswordChar == '\0' || mText == "")
		{
			text = unicode_to_utf8(mUnicodeChars(mOffsetIndex, mUnicodeChars.size() - mOffsetIndex));
		}
		else
		{
			text = hstr(mPasswordChar, mUnicodeChars.size() - mOffsetIndex);
		}
		int count = atres::renderer->getTextCountUnformatted(mFontName, text, x - mRect.x);
		setCursorIndex(mOffsetIndex + _convertToUnicodeChars(text(0, count)).size());
	}
	
	void EditBox::setMaxLength(int maxLength)
	{
		mMaxLength = maxLength;
		if (mMaxLength > 0 && mUnicodeChars.size() > mMaxLength)
		{
			mUnicodeChars = mUnicodeChars(0, mMaxLength);
			mText = unicode_to_utf8(mUnicodeChars);
			setCursorIndex(mCursorIndex);
		}
	}

	void EditBox::setFilter(chstr filter)
	{
		mFilter = filter;
		mFilterChars = _convertToUnicodeChars(mFilter);
		setText(mText);
	}
	
	void EditBox::setText(chstr text)
	{
		mUnicodeChars = _convertToUnicodeChars(text);
		if (mUnicodeChars.size() > 0 && mFilterChars.size() > 0)
		{
			mUnicodeChars &= mFilterChars; // intersect, remove from first all that are not in second
		}
		if (mMaxLength > 0 && mUnicodeChars.size() > mMaxLength)
		{
			mUnicodeChars = mUnicodeChars(0, mMaxLength);
		}
		Label::setText(unicode_to_utf8(mUnicodeChars));
		setCursorIndex(mCursorIndex);
	}

	bool EditBox::onMouseDown(float x, float y, int button)
	{
		if (Object::onMouseDown(x, y, button))
		{
			return true;
		}
		if (isCursorInside())
		{
			mPushed = true;
			return true;
		}
		return false;
	}

	bool EditBox::onMouseUp(float x, float y, int button)
	{
		if (Object::onMouseUp(x, y, button))
		{
			return true;
		}
		if (mPushed && isCursorInside())
		{
			setCursorIndexAt(x, y);
			if (mDataset != NULL)
			{
				mDataset->setFocusedObject(this);
				mBlinkTimer = 0.0f;
				april::rendersys->getWindow()->beginKeyboardHandling();
			}
			mPushed = false;
			_triggerEvent("Click", x, y, button);
			return true;
		}
		mPushed = false;
		return false;
	}

	void EditBox::onKeyDown(unsigned int keycode)
	{
		switch (keycode)
		{
#ifndef _ANDROID // these keys aren't really available on Android
		case april::AK_LEFT:
			mCtrlMode ? _cursorMoveLeftWord() : _cursorMoveLeft();
			break;
		case april::AK_RIGHT:
			mCtrlMode ? _cursorMoveRightWord() : _cursorMoveRight();
			break;
#endif
		case april::AK_BACK:
			mCtrlMode ? _deleteLeftWord() : _deleteLeft();
			break;
		case april::AK_DELETE:
			mCtrlMode ? _deleteRightWord() : _deleteRight();
			break;
#ifndef _ANDROID // these keys aren't really available on Android
		case april::AK_HOME:
			setCursorIndex(0);
			break;
		case april::AK_END:
			setCursorIndex(mUnicodeChars.size());
			break;
		case april::AK_CONTROL:
			mCtrlMode = true;
			break;
		case april::AK_RETURN:
				_triggerEvent("Submit", 0, 0, april::AK_RETURN);
			break;
#else
		case april::AK_RETURN:
			mDataset->setFocusedObject(NULL);
			mBlinkTimer = 0.0f;
			april::rendersys->getWindow()->terminateKeyboardHandling();
			break;
#endif
		}
	}
	
	void EditBox::onKeyUp(unsigned int keycode)
	{
		switch (keycode)
		{
		case april::AK_CONTROL:
		case april::AK_MENU:
			mCtrlMode = false;
			break;
		}
	}

	void EditBox::onChar(unsigned int charcode)
	{
		if (atres::renderer->getFontResource(mFontName)->hasChar(charcode) && (mFilterChars.size() == 0 || mFilterChars.contains(charcode)))
		{
			_insertChar(charcode);
		}
	}

	void EditBox::cancelMouseDown()
	{
		mPushed = false;
		Object::cancelMouseDown();
	}

	harray<unsigned int> EditBox::_convertToUnicodeChars(chstr string)
	{
		int length;
		unsigned int* text = utf8_to_unicode(string, &length);
		harray<unsigned int> chars = harray<unsigned int>(text, length);
		delete [] text;
		return chars;
	}
	
	void EditBox::_cursorMoveLeft()
	{
		setCursorIndex(mCursorIndex - 1);
	}
	
	void EditBox::_cursorMoveRight()
	{
		setCursorIndex(mCursorIndex + 1);
	}
	
	void EditBox::_cursorMoveLeftWord()
	{
		while (mCursorIndex > 0 && mUnicodeChars[mCursorIndex - 1] == UNICODE_CHAR_SPACE)
		{
			mCursorIndex--;
		}
		while (mCursorIndex > 0 && mUnicodeChars[mCursorIndex - 1] != UNICODE_CHAR_SPACE)
		{
			mCursorIndex--;
		}
		mBlinkTimer = 0.0f;
	}
	
	void EditBox::_cursorMoveRightWord()
	{
		while (mCursorIndex < mUnicodeChars.size() && mUnicodeChars[mCursorIndex] != UNICODE_CHAR_SPACE)
		{
			mCursorIndex++;
		}
		while (mCursorIndex < mUnicodeChars.size() && mUnicodeChars[mCursorIndex] == UNICODE_CHAR_SPACE)
		{
			mCursorIndex++;
		}
		mBlinkTimer = 0.0f;
	}
	
	void EditBox::_deleteLeft(int count)
	{
		if (mCursorIndex > 0)
		{
			count = hmin(count, mCursorIndex);
			harray<unsigned int> left = (mCursorIndex > count ? mUnicodeChars(0, mCursorIndex - count) : harray<unsigned int>());
			harray<unsigned int> right = (mCursorIndex < mUnicodeChars.size() ? mUnicodeChars(mCursorIndex, mUnicodeChars.size() - mCursorIndex) : harray<unsigned int>());
			mCursorIndex -= count;
			mUnicodeChars = left + right;
			mText = unicode_to_utf8(mUnicodeChars);
			mBlinkTimer = 0.0f;
		}
	}
	
	void EditBox::_deleteRight(int count)
	{
		count = hmin(count, mUnicodeChars.size() - mCursorIndex);
		mCursorIndex += count;
		_deleteLeft(count);
	}
	
	void EditBox::_deleteLeftWord()
	{
		int index = mCursorIndex;
		while (index > 0 && mUnicodeChars[index - 1] == UNICODE_CHAR_SPACE)
		{
			index--;
		}
		while (index > 0 && mUnicodeChars[index - 1] != UNICODE_CHAR_SPACE)
		{
			index--;
		}
		if (mCursorIndex > index)
		{
			_deleteLeft(mCursorIndex - index);
		}
	}
	
	void EditBox::_deleteRightWord()
	{
		int index = mCursorIndex;
		while (index < mUnicodeChars.size() && mUnicodeChars[index] != UNICODE_CHAR_SPACE)
		{
			index++;
		}
		while (index < mUnicodeChars.size() && mUnicodeChars[index] == UNICODE_CHAR_SPACE)
		{
			index++;
		}
		if (index > mCursorIndex)
		{
			_deleteRight(index - mCursorIndex);
		}
	}
	
	void EditBox::_insertChar(unsigned int charcode)
	{
		if (mMaxLength > 0 && mUnicodeChars.size() >= mMaxLength)
		{
			return;
		}
		harray<unsigned int> left = (mCursorIndex > 0 ? mUnicodeChars(0, mCursorIndex) : harray<unsigned int>());
		harray<unsigned int> right = (mCursorIndex < mUnicodeChars.size() ?
			mUnicodeChars(mCursorIndex, mUnicodeChars.size() - mCursorIndex) : harray<unsigned int>());
		mCursorIndex++;
		mUnicodeChars = (left + charcode) + right;
		mText = unicode_to_utf8(mUnicodeChars);
		mBlinkTimer = 0.0f;
	}
	

}
