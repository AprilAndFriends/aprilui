/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Keys.h>
#include <atres/atres.h>
#include <atres/FontResource.h>
#include <hltypes/hlog.h>
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
		this->mText = "";
		this->mEmptyText = "";
		this->mEmptyTextKey = "";
		this->mHorzFormatting = atres::LEFT;
		this->mTextFormatting = false;
		this->mPushed = false;
		this->mMaxLength = 0;
		this->mPasswordChar = '\0';
		this->mCursorIndex = 0;
		this->mOffsetIndex = 0;
		this->mCtrlMode = false;
		this->mFilter = "";
		this->mBlinkTimer = 0.0f;
		this->mBackgroundColor = april::Color::Black;
		/// TODO - remove
		this->mSpaceHack = false;
	}

	EditBox::~EditBox()
	{
	}

	Object* EditBox::createInstance(chstr name, grect rect)
	{
		return new EditBox(name, rect);
	}

	void EditBox::setCursorIndex(int value)
	{
		this->mCursorIndex = hclamp(value, 0, this->mUnicodeChars.size());
		this->mBlinkTimer = 0.0f;
	}
	
	void EditBox::setCursorIndexAt(float x, float y)
	{
		hstr text;
		if (this->mPasswordChar == '\0' || this->mText == "")
		{
			text = unicode_to_utf8(this->mUnicodeChars(this->mOffsetIndex, this->mUnicodeChars.size() - this->mOffsetIndex));
		}
		else
		{
			text = hstr(this->mPasswordChar, this->mUnicodeChars.size() - this->mOffsetIndex);
		}
		int count = atres::renderer->getTextCountUnformatted(this->mFontName, text, x);
		this->setCursorIndex(this->mOffsetIndex + this->_convertToUnicodeChars(text(0, count)).size());
	}
	
	void EditBox::setMaxLength(int value)
	{
		this->mMaxLength = value;
		if (this->mMaxLength > 0 && this->mUnicodeChars.size() > this->mMaxLength)
		{
			this->mUnicodeChars = this->mUnicodeChars(0, this->mMaxLength);
			this->mText = unicode_to_utf8(this->mUnicodeChars);
			this->setCursorIndex(this->mCursorIndex);
		}
	}

	void EditBox::setFilter(chstr value)
	{
		this->mFilter = value;
		this->mFilterChars = this->_convertToUnicodeChars(this->mFilter);
		this->setText(this->mText);
	}
	
	void EditBox::setText(chstr value)
	{
		this->mUnicodeChars = this->_convertToUnicodeChars(value);
		if (this->mUnicodeChars.size() > 0 && this->mFilterChars.size() > 0)
		{
			this->mUnicodeChars &= this->mFilterChars; // intersect, remove from first all that are not in second
		}
		if (this->mMaxLength > 0 && this->mUnicodeChars.size() > this->mMaxLength)
		{
			this->mUnicodeChars = this->mUnicodeChars(0, this->mMaxLength);
		}
		Label::setText(unicode_to_utf8(this->mUnicodeChars));
		this->setCursorIndex(this->mCursorIndex);
	}

	void EditBox::setEmptyText(chstr value)
	{
		this->mEmptyText = value;
		this->mEmptyTextKey = "";
	}

	void EditBox::setEmptyTextKey(chstr value)
	{
		hstr emptyTextKey = value; // because value is a chstr which could reference mTextKey
		this->setEmptyText(this->getDataset()->getText(emptyTextKey));
		this->mEmptyTextKey = emptyTextKey;
	}

	bool EditBox::isFocused()
	{
		return (this->mDataset != NULL && this->mDataset->getFocusedObject() == this);
	}

	void EditBox::setFocused(bool value)
	{
		if (this->mDataset != NULL)
		{
			if (value)
			{
				this->mDataset->setFocusedObject(this);
				this->mBlinkTimer = 0.0f;
				april::window->beginKeyboardHandling();
			}
			else if (this->mDataset->getFocusedObject() == this)
			{
				april::window->terminateKeyboardHandling();
				this->mDataset->setFocusedObject(NULL);
			}
		}
	}

	void EditBox::update(float time)
	{
		Label::update(time);
		this->mBlinkTimer += time * 2;
		this->mBlinkTimer = (this->mBlinkTimer - (int)this->mBlinkTimer);
	}

	void EditBox::OnDraw()
	{
		//////////////
		// TODO - remove this hack, fix it in ATRES
		if (this->mSpaceHack)
		{
			if (this->mUnicodeChars.size() > 0 && this->mUnicodeChars[0] == UNICODE_CHAR_SPACE)
			{
				while (this->mUnicodeChars.size() > 0 && this->mUnicodeChars[0] == UNICODE_CHAR_SPACE)
				{
					this->mUnicodeChars.remove_at(0);
				}
				this->mText = unicode_to_utf8(this->mUnicodeChars);
			}
			if (this->mEmptyUnicodeChars.size() > 0 && this->mEmptyUnicodeChars[0] == UNICODE_CHAR_SPACE)
			{
				while (this->mEmptyUnicodeChars.size() > 0 && this->mEmptyUnicodeChars[0] == UNICODE_CHAR_SPACE)
				{
					this->mEmptyUnicodeChars.remove_at(0);
				}
				this->mEmptyText = unicode_to_utf8(this->mEmptyUnicodeChars);
			}
		}
		//////////////
		grect rect = this->_getDrawRect();
		hstr text = this->mText;
		harray<unsigned int> unicodeChars = this->mUnicodeChars;
		if (this->mPasswordChar != '\0' && this->mText != "")
		{
			this->mText = hstr(this->mPasswordChar, this->mUnicodeChars.size());
			this->mUnicodeChars = this->_convertToUnicodeChars(this->mText);
		}
		while (this->mOffsetIndex > 0 && this->mOffsetIndex >= this->mCursorIndex)
		{
			this->mOffsetIndex = hmax(0, this->mCursorIndex - 5);
		}
		//////////////
		// TODO - remove this hack, fix it in ATRES
		while (this->mOffsetIndex < this->mUnicodeChars.size() - 1 && this->mUnicodeChars[this->mOffsetIndex] == UNICODE_CHAR_SPACE)
		{
			this->mOffsetIndex++;
		}
		//////////////
		rect.w -= 12;
		int count;
		while (true)
		{
			this->mText = unicode_to_utf8(this->mUnicodeChars(this->mOffsetIndex, this->mUnicodeChars.size() - this->mOffsetIndex));
			count = atres::renderer->getTextCountUnformatted(this->mFontName, this->mText, rect.w);
			count = this->_convertToUnicodeChars(this->mText(0, count)).size();
			if (this->mOffsetIndex > this->mCursorIndex)
			{
				this->mOffsetIndex = this->mCursorIndex;
			}
			else if (this->mOffsetIndex < this->mCursorIndex - count)
			{
				this->mOffsetIndex = this->mCursorIndex - count;
			}
			else
			{
				break;
			}
		}
		hstr renderText = this->mText;
		if (renderText == "" && this->mDataset != NULL && this->mDataset->getFocusedObject() != this)
		{
			this->mText = this->mEmptyText;
		}
		unsigned char alpha = this->mBackgroundColor.a;
		if (this->mPushed)
		{
			this->mBackgroundColor.a = (unsigned char)(this->mBackgroundColor.a * 0.75f);
		}
		Label::OnDraw();
		this->mBackgroundColor.a = alpha;
		this->mText = renderText;
		if (this->mDataset != NULL && this->mDataset->getFocusedObject() == this && this->mBlinkTimer < 0.5f)
		{
			this->mText = unicode_to_utf8(this->mUnicodeChars(this->mOffsetIndex, this->mCursorIndex - this->mOffsetIndex));
			rect.x += atres::renderer->getTextWidthUnformatted(this->mFontName, this->mText);
			float h = atres::renderer->getFontLineHeight(this->mFontName);
			if (this->mHorzFormatting == atres::CENTER || this->mHorzFormatting == atres::CENTER_WRAPPED)
			{
				float w = atres::renderer->getTextWidthUnformatted(this->mFontName, text);
				rect.x += (this->mRect.w - w) / 2;
			}
			else if (this->mHorzFormatting == atres::RIGHT || this->mHorzFormatting == atres::RIGHT_WRAPPED)
			{
				float w = atres::renderer->getTextWidthUnformatted(this->mFontName, text);
				rect.x += this->mRect.w - w;
			}
			rect.y += (rect.h - h) / 2 + 2;
			rect.w = 1;
			rect.h = h - 4;
			april::rendersys->drawRect(rect, this->_getDrawColor() * this->mTextColor);
		}
		this->mText = text;
		this->mUnicodeChars = unicodeChars;
	}
	
	void EditBox::notifyEvent(chstr name, void* params)
	{
		if (name == "onLocalizationChanged")
		{
			if (this->mEmptyTextKey != "")
			{
				this->setEmptyTextKey(this->mEmptyTextKey);
			}
		}
		Label::notifyEvent(name, params);
	}
	
	bool EditBox::onMouseDown(int button)
	{
		if (Object::onMouseDown(button))
		{
			return true;
		}
		if (this->isCursorInside())
		{
			this->mPushed = true;
			return true;
		}
		return false;
	}

	bool EditBox::onMouseUp(int button)
	{
		if (Object::onMouseUp(button))
		{
			return true;
		}
		if (this->mPushed && this->isCursorInside())
		{
			gvec2 position = (aprilui::getCursorPosition() - this->getDerivedPosition()) / this->getDerivedScale();
			this->setCursorIndexAt(position.x, position.y);
			this->setFocused(true);
			this->mPushed = false;
			this->triggerEvent("Click", button);
			return true;
		}
		this->mPushed = false;
		return false;
	}

	bool EditBox::onKeyDown(unsigned int keyCode)
	{
		if (Object::onKeyDown(keyCode))
		{
			return true;
		}
		if (this->mDataset == NULL || this->mDataset->getFocusedObject() == this)
		{
			switch (keyCode)
			{
#ifndef _ANDROID // these keys aren't really available on Android
			case april::AK_LEFT:
				this->mCtrlMode ? this->_cursorMoveLeftWord() : this->_cursorMoveLeft();
				break;
			case april::AK_RIGHT:
				this->mCtrlMode ? this->_cursorMoveRightWord() : this->_cursorMoveRight();
				break;
#endif
			case april::AK_BACK:
				this->mCtrlMode ? this->_deleteLeftWord() : this->_deleteLeft();
				break;
			case april::AK_DELETE:
				this->mCtrlMode ? this->_deleteRightWord() : this->_deleteRight();
				break;
#ifndef _ANDROID // these keys aren't really available on Android
			case april::AK_HOME:
				this->setCursorIndex(0);
				break;
			case april::AK_END:
				this->setCursorIndex(this->mUnicodeChars.size());
				break;
			case april::AK_CONTROL:
				this->mCtrlMode = true;
				break;
#endif
			case april::AK_RETURN:
				this->triggerEvent("Submit", april::AK_RETURN);
				break;
			}
		}
		return false;
	}
	
	bool EditBox::onKeyUp(unsigned int keyCode)
	{
		if (Object::onKeyUp(keyCode))
		{
			return true;
		}
		switch (keyCode)
		{
		case april::AK_CONTROL:
		case april::AK_MENU:
			this->mCtrlMode = false;
			break;
		}
		return false;
	}

	bool EditBox::onChar(unsigned int charCode)
	{
		if (Object::onChar(charCode))
		{
			return true;
		}
		if (atres::renderer->getFontResource(this->mFontName)->hasChar(charCode) && (this->mFilterChars.size() == 0 || this->mFilterChars.contains(charCode)))
		{
			this->_insertChar(charCode);
		}
		return false;
	}

	void EditBox::cancelMouseDown()
	{
		this->mPushed = false;
		Object::cancelMouseDown();
	}

	hstr EditBox::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "max_length")		return this->getMaxLength();
		if (name == "password_char")	return this->getPasswordChar();
		if (name == "filter")			return this->getFilter();
		if (name == "empty_text")		return this->getEmptyText();
		if (name == "empty_text_key")	return this->getEmptyTextKey();
		if (name == "space_hack")		return this->mSpaceHack;
		return Label::getProperty(name, property_exists);
	}
	
	bool EditBox::setProperty(chstr name, chstr value)
	{
		if		(name == "max_length")		this->setMaxLength(value);
		else if	(name == "password_char")	this->setPasswordChar(value.c_str()[0]);
		else if	(name == "filter")			this->setFilter(value);
		else if	(name == "empty_text")		this->setEmptyText(value);
		else if	(name == "empty_text_key")	this->setEmptyTextKey(value);
		else if	(name == "space_hack")		this->mSpaceHack = (bool)value;
		else return Label::setProperty(name, value);
		return true;
	}
	
	harray<unsigned int> EditBox::_convertToUnicodeChars(chstr string)
	{
		std::basic_string<unsigned int> text = utf8_to_unicode(string);
		return harray<unsigned int>(text.c_str(), text.size());
	}
	
	void EditBox::_cursorMoveLeft()
	{
		this->setCursorIndex(this->mCursorIndex - 1);
	}
	
	void EditBox::_cursorMoveRight()
	{
		this->setCursorIndex(this->mCursorIndex + 1);
	}
	
	void EditBox::_cursorMoveLeftWord()
	{
		while (this->mCursorIndex > 0 && this->mUnicodeChars[mCursorIndex - 1] == UNICODE_CHAR_SPACE)
		{
			this->mCursorIndex--;
		}
		while (this->mCursorIndex > 0 && this->mUnicodeChars[mCursorIndex - 1] != UNICODE_CHAR_SPACE)
		{
			this->mCursorIndex--;
		}
		this->mBlinkTimer = 0.0f;
	}
	
	void EditBox::_cursorMoveRightWord()
	{
		while (this->mCursorIndex < this->mUnicodeChars.size() && this->mUnicodeChars[mCursorIndex] != UNICODE_CHAR_SPACE)
		{
			this->mCursorIndex++;
		}
		while (this->mCursorIndex < this->mUnicodeChars.size() && this->mUnicodeChars[mCursorIndex] == UNICODE_CHAR_SPACE)
		{
			this->mCursorIndex++;
		}
		this->mBlinkTimer = 0.0f;
	}
	
	void EditBox::_deleteLeft(int count)
	{
		if (this->mCursorIndex > 0)
		{
			count = hmin(count, this->mCursorIndex);
			harray<unsigned int> left;
			if (this->mCursorIndex > count)
			{
				left = this->mUnicodeChars(0, this->mCursorIndex - count);
			}
			harray<unsigned int> right;
			if (this->mCursorIndex < this->mUnicodeChars.size())
			{
				right = this->mUnicodeChars(this->mCursorIndex, this->mUnicodeChars.size() - this->mCursorIndex);
			}
			this->mCursorIndex -= count;
			this->mUnicodeChars = left + right;
			this->mText = unicode_to_utf8(this->mUnicodeChars);
			this->mBlinkTimer = 0.0f;
		}
	}
	
	void EditBox::_deleteRight(int count)
	{
		count = hmin(count, this->mUnicodeChars.size() - this->mCursorIndex);
		this->mCursorIndex += count;
		this->_deleteLeft(count);
	}
	
	void EditBox::_deleteLeftWord()
	{
		int index = this->mCursorIndex;
		while (index > 0 && this->mUnicodeChars[index - 1] == UNICODE_CHAR_SPACE)
		{
			index--;
		}
		while (index > 0 && this->mUnicodeChars[index - 1] != UNICODE_CHAR_SPACE)
		{
			index--;
		}
		if (this->mCursorIndex > index)
		{
			this->_deleteLeft(this->mCursorIndex - index);
		}
	}
	
	void EditBox::_deleteRightWord()
	{
		int index = this->mCursorIndex;
		while (index < this->mUnicodeChars.size() && this->mUnicodeChars[index] != UNICODE_CHAR_SPACE)
		{
			index++;
		}
		while (index < this->mUnicodeChars.size() && this->mUnicodeChars[index] == UNICODE_CHAR_SPACE)
		{
			index++;
		}
		if (index > this->mCursorIndex)
		{
			this->_deleteRight(index - this->mCursorIndex);
		}
	}
	
	void EditBox::_insertChar(unsigned int charCode)
	{
		if (this->mMaxLength > 0 && this->mUnicodeChars.size() >= this->mMaxLength)
		{
			return;
		}
		harray<unsigned int> left;
		if (this->mCursorIndex > 0)
		{
			left = this->mUnicodeChars(0, this->mCursorIndex);
		}
		harray<unsigned int> right;
		if (this->mCursorIndex < this->mUnicodeChars.size())
		{
			right = this->mUnicodeChars(this->mCursorIndex, this->mUnicodeChars.size() - this->mCursorIndex);
		}
		this->mCursorIndex++;
		this->mUnicodeChars = (left + charCode) + right;
		this->mText = unicode_to_utf8(this->mUnicodeChars);
		this->mBlinkTimer = 0.0f;
	}
	

}
