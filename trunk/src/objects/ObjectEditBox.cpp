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
#include <atres/Atres.h>
#include <atres/Font.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>
#include <april/Window.h>

#include "AprilUI.h"
#include "Dataset.h"
#include "ObjectEditBox.h"

namespace AprilUI
{
	EditBox::EditBox(chstr name, grect rect) :
		Label(name, rect)
	{
		mText = "";
		mTypeName = "EditBox";
		mHorzFormatting = Atres::LEFT;
		mTextFormatting = false;
		mPushed = false;
		mMaxLength = 0;
		mPasswordChar = '\0';
		mCursorIndex = 0;
		mOffsetIndex = 0;
		mCtrlMode = false;
		mFilter = "";
		mBlinkTimer = 0.0f;
	}
	
	void EditBox::update(float time)
	{
		Label::update(time);
		mBlinkTimer += time * 2;
		mBlinkTimer = (mBlinkTimer - (int)mBlinkTimer);
	}

	void EditBox::OnDraw(gvec2 offset)
	{
		grect rect = mRect + offset;
#ifdef _DEBUG
		if (!AprilUI::isDebugMode())
#endif
		April::rendersys->drawColoredQuad(rect.x, rect.y, rect.w, rect.h, 0, 0, 0, 0.7f + 0.3f * mPushed);
		hstr text = mText;
		if (mPasswordChar && mText != "")
		{
			mText = hstr(mPasswordChar, mText.size());
		}
		while (mOffsetIndex > 0 && mOffsetIndex >= mCursorIndex)
		{
			mOffsetIndex = hmax(0, mCursorIndex - 5);
		}
		rect.w -= 12;
		int count;
		while (true)
		{
			count = Atres::getTextCountUnformatted(mFontName, mText(mOffsetIndex, mText.size() - mOffsetIndex), rect.w);
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
		mText = mText(mOffsetIndex, mText.size() - mOffsetIndex);
		offset.x += 2;
		Label::OnDraw(offset);
		if (mDataset != NULL && this == mDataset->getFocusedObject() && mBlinkTimer < 0.5f)
		{
			rect = mRect + offset;
			rect.x += Atres::getTextWidthUnformatted(mFontName, mText(0, mCursorIndex - mOffsetIndex));
			float h = Atres::getFontHeight(mFontName);
			rect.y += (rect.h - h) / 2 + 2;
			rect.w = 2;
			rect.h = h - 4;
			April::rendersys->drawColoredQuad(rect.x, rect.y, rect.w, rect.h,
				mTextColor.r_float(), mTextColor.g_float(), mTextColor.b_float(), mTextColor.a_float());
		}
		mText = text;
	}
	
	void EditBox::setProperty(chstr name, chstr value)
	{
		Label::setProperty(name, value);
		if      (name == "max_length")    setMaxLength(value);
		else if (name == "password_char") setPasswordChar(value.c_str()[0]);
		else if (name == "filter")        setFilter(value);
	}
	
	void EditBox::setCursorIndex(int cursorIndex)
	{
		mCursorIndex = hclamp(cursorIndex, 0, mText.size());
		mBlinkTimer = 0.0f;
	}
	
	void EditBox::setCursorIndexAt(float x,float y)
	{
		hstr text = mText;
		if (mPasswordChar != '\0' && text != "")
		{
			text = hstr(mPasswordChar, text.size());
		}
		int count = Atres::getTextCountUnformatted(mFontName, text(mOffsetIndex, text.size() - mOffsetIndex), x - mRect.x);
		setCursorIndex(mOffsetIndex + count);
	}
	
	void EditBox::setMaxLength(int maxLength)
	{
		mMaxLength = maxLength;
		if (mMaxLength > 0 && mText.size() > mMaxLength)
		{
			mText = mText(0, mMaxLength);
			setCursorIndex(mCursorIndex);
		}
	}
	
	void EditBox::setFilter(chstr filter)
	{
		mFilter = filter;
		setText(mText);
	}
	
	void EditBox::setText(chstr text)
	{
		Label::setText(text);
		harray<char> chars = harray<char>(mText.c_str(), mText.size());
		if (mFilter != "")
		{
			foreach (char, it, chars)
			{
				if (!mFilter.contains(*it))
				{
					mText.replace((*it), "");
				}
			}
		}
		if (mMaxLength > 0 && mText.size() > mMaxLength)
		{
			mText = mText(0, mMaxLength);
		}
		setCursorIndex(mCursorIndex);
	}

	bool EditBox::OnMouseDown(float x, float y, int button)
	{
		if (Object::OnMouseDown(x, y, button))
		{
			return true;
		}
		if (isPointInside(x, y))
		{
			mPushed = true;
			return true;
		}
		return false;
	}

	bool EditBox::OnMouseUp(float x, float y, int button)
	{
		if (Object::OnMouseUp(x, y, button))
		{
			return true;
		}
		if (mPushed && isPointInside(x, y))
		{
			setCursorIndexAt(x, y);
			if (mDataset)
			{
				mDataset->setFocusedObject(this);
				mBlinkTimer = 0.0f;
			}
			April::rendersys->getWindow()->beginKeyboardHandling();
			mPushed = false;
			triggerEvent("Click", x, y, 0);
			return true;
		}
		mPushed = false;
		return false;
	}
	
	void EditBox::OnKeyDown(unsigned int keycode)
	{
		switch (keycode)
		{
		case April::AK_LEFT:
			mCtrlMode ? _cursorMoveLeftWord() : setCursorIndex(mCursorIndex - 1);
			break;
		case April::AK_RIGHT:
			mCtrlMode ? _cursorMoveRightWord() : setCursorIndex(mCursorIndex + 1);
			break;
		case April::AK_BACK:
			mCtrlMode ? _deleteLeftWord() : _deleteLeft();
			break;
		case April::AK_DELETE:
			mCtrlMode ? _deleteRightWord() : _deleteRight();
			break;
		case April::AK_HOME:
			setCursorIndex(0);
			break;
		case April::AK_END:
			setCursorIndex(mText.size());
			break;
		case April::AK_CONTROL:
			mCtrlMode = true;
			break;
		}
	}
	
	void EditBox::OnKeyUp(unsigned int keycode)
	{
		switch (keycode)
		{
		case April::AK_CONTROL:
		case April::AK_MENU:
			mCtrlMode = false;
			break;
		}
	}

	void EditBox::OnChar(unsigned int charcode)
	{
		char c = (char)charcode;
		if (Atres::getFont(mFontName)->hasChar(charcode) && (mFilter == "" || mFilter.contains(c)))
		{
			_insertText(c);
		}
	}
	
	void EditBox::_cursorMoveLeftWord()
	{
		while (mCursorIndex > 0 && mText[mCursorIndex - 1] == ' ')
		{
			mCursorIndex--;
		}
		while (mCursorIndex > 0 && mText[mCursorIndex - 1] != ' ')
		{
			mCursorIndex--;
		}
		mBlinkTimer = 0.0f;
	}
	
	void EditBox::_cursorMoveRightWord()
	{
		while (mCursorIndex < mText.size() && mText[mCursorIndex] != ' ')
		{
			mCursorIndex++;
		}
		while (mCursorIndex < mText.size() && mText[mCursorIndex] == ' ')
		{
			mCursorIndex++;
		}
		mBlinkTimer = 0.0f;
	}
	
	void EditBox::_deleteLeft(int count)
	{
		if (mCursorIndex != 0)
		{
			count = hmin(count, mCursorIndex);
			hstr left = (mCursorIndex > count ? mText(0, mCursorIndex - count) : "");
			hstr right = (mCursorIndex < mText.size() ? mText(mCursorIndex, mText.size() - mCursorIndex) : "");
			mCursorIndex -= count;
			mText = left + right;
			mBlinkTimer = 0.0f;
		}
	}
	
	void EditBox::_deleteRight(int count)
	{
		count = hmin(count, mText.size() - mCursorIndex);
		mCursorIndex += count;
		_deleteLeft(count);
	}
	
	void EditBox::_deleteLeftWord()
	{
		int index = mCursorIndex;
		while (index > 0 && mText[index - 1] == ' ')
		{
			index--;
		}
		while (index > 0 && mText[index - 1] != ' ')
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
		while (index < mText.size() && mText[index] != ' ')
		{
			index++;
		}
		while (index < mText.size() && mText[index] == ' ')
		{
			index++;
		}
		if (index > mCursorIndex)
		{
			_deleteRight(index - mCursorIndex);
		}
	}
	
	void EditBox::_insertText(chstr text)
	{
		hstr newText = text;
		if (mMaxLength > 0)
		{
			if (mText.size() >= mMaxLength)
			{
				return;
			}
			if (mText.size() + newText.size() > mMaxLength)
			{
				newText = newText(0, mMaxLength - mText.size());
			}
		}
		hstr left = (mCursorIndex > 0 ? mText(0,mCursorIndex) : "");
		hstr right = (mCursorIndex < mText.size() ? mText(mCursorIndex, mText.size() - mCursorIndex) : "");
		mCursorIndex += newText.size();
		mText = left + newText + right;
		mBlinkTimer = 0.0f;
	}
	

}
