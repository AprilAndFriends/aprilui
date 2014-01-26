/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.8
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
#define UNICODE_CHAR_NEWLINE 0x0A
#define CHECK_RECT_HEIGHT 100000.0f

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
		this->mMultiLine = false;
		this->mCursorIndex = 0;
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
		this->mCursorIndex = hclamp(value, 0, this->mText.utf8_size());
		this->mBlinkTimer = 0.0f;
	}
	
	void EditBox::setCursorIndexAt(float x, float y)
	{
		if (this->mText == "")
		{
			this->setCursorIndex(0);
			return;
		}
		float fh = atres::renderer->getFontLineHeight(this->mFontName);
		// full text
		harray<atres::FormatTag> tags = atres::renderer->prepareTags(this->mFontName);
		harray<atres::RenderLine> lines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->mRect.w, CHECK_RECT_HEIGHT), this->mText, tags, this->mHorzFormatting, this->mVertFormatting);
		float w2 = this->mRect.w * 0.5f;
		float h2 = this->mRect.h * 0.5f;
		float xhf = 0.0f; // x height factor
		gvec2 base;
		if (this->mHorzFormatting == atres::CENTER || this->mHorzFormatting == atres::CENTER_WRAPPED)
		{
			base.x = w2;
		}
		else if (this->mHorzFormatting == atres::RIGHT || this->mHorzFormatting == atres::RIGHT_WRAPPED)
		{
			base.x = w2 * 2;
		}
		if (this->mVertFormatting == atres::CENTER)
		{
			xhf = 0.5f;
		}
		else if (this->mVertFormatting == atres::BOTTOM)
		{
			xhf = 1.0f;
		}
		base.y = (h2 * 2 - fh) * xhf;
		int offsetIndex = this->mText.size();
		x -= this->mTextOffset.x;
		y += xhf * (CHECK_RECT_HEIGHT - this->mRect.h) - this->mTextOffset.y;
		if (lines.size() > 0)
		{
			if (y >= lines.first().rect.y)
			{
				offsetIndex = this->mText.size();
				atres::RenderLine* line = NULL;
				for_iter (i, 0, lines.size())
				{
					if (is_in_range(y, lines[i].rect.y, lines[i].rect.y + lines[i].rect.h))
					{
						if (x <= lines[i].rect.x + lines[i].rect.w)
						{
							line = &lines[i];
						}
						else
						{
							offsetIndex = lines[i].start + lines[i].count;
						}
						break;
					}
				}
				if (line != NULL)
				{
					offsetIndex = line->start;
					if (line->words.size() > 0)
					{
						atres::RenderWord* word = NULL;
						foreach (atres::RenderWord, it, line->words)
						{
							if (is_in_range(x, (*it).rect.x, (*it).rect.x + (*it).rect.w))
							{
								word = &(*it);
								break;
							}
						}
						if (word != NULL)
						{
							float ow = word->rect.x;
							offsetIndex = word->start;
							float cw = 0.0f;
							foreach (float, it, word->charWidths)
							{
								cw = (*it) * 0.5f;
								if (is_in_range(x, ow, ow + cw))
								{
									break;
								}
								offsetIndex++;
								ow += cw;
								if (is_in_range(x, ow, ow + cw))
								{
									break;
								}
								ow += cw;
							}
						}
					}
				}
			}
			else
			{
				offsetIndex = 0;
			}
		}
		this->setCursorIndex(this->mText(0, offsetIndex).utf8_size());
	}
	
	void EditBox::setMaxLength(int value)
	{
		this->mMaxLength = value;
		if (this->mMaxLength > 0 && this->mText.utf8_size() > this->mMaxLength)
		{
			this->mText = this->mText.utf8_substr(0, this->mMaxLength);
			this->setCursorIndex(this->mCursorIndex);
		}
	}

	void EditBox::setFilter(chstr value)
	{
		this->mFilter = value;
		this->setText(this->mText);
	}
	
	void EditBox::setText(chstr value)
	{
		hstr newValue = value;
		if (newValue.size() > 0 && this->mFilter.size() > 0)
		{
			std::basic_string<unsigned int> ustr = newValue.u_str();
			harray<unsigned int> uText(ustr.c_str(), ustr.size());
			ustr = this->mFilter.u_str();
			harray<unsigned int> uFilter(ustr.c_str(), ustr.size());
			uFilter |= '\n'; // this is for multiline
			uText &= uFilter; // intersect, remove from first all that are not in second
			newValue = hstr::from_unicode(uText);
		}
		if (this->mMaxLength > 0 && newValue.utf8_size() > this->mMaxLength)
		{
			newValue = newValue.utf8_substr(0, this->mMaxLength);
		}
		Label::setText(newValue);
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

	void EditBox::setFocused(bool value)
	{
		if (this->mDataset != NULL)
		{
			if (value)
			{
				this->mDataset->focus(this);
				this->mBlinkTimer = 0.0f;
			}
			else if (this->mDataset->getFocusedObject() == this)
			{
				this->mDataset->removeFocus();
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
			this->mText = this->mText.ltrim();
			this->mEmptyText = this->mEmptyText.ltrim();
		}
		//////////////
		grect rect = this->_getDrawRect();
		hstr text = this->mText;
		rect.w -= 12;
		if (this->mPasswordChar != '\0' && this->mText != "")
		{
			this->mText = hstr(this->mPasswordChar, this->mText.utf8_size());
		}
		if (this->mText == "" && this->mDataset != NULL && this->mDataset->getFocusedObject() != this)
		{
			this->mText = this->mEmptyText;
		}
		unsigned char alpha = this->mBackgroundColor.a;
		if (this->mPushed)
		{
			this->mBackgroundColor.a = (unsigned char)(this->mBackgroundColor.a * 0.75f);
		}
		float fh = atres::renderer->getFontLineHeight(this->mFontName);
		if (this->mDataset != NULL && this->mDataset->getFocusedObject() == this)
		{
			rect.setPosition(this->_makeCaretPosition(this->mText.utf8_substr(0, this->mCursorIndex), text));
			if (this->mHorzFormatting != atres::LEFT_WRAPPED && this->mHorzFormatting != atres::CENTER_WRAPPED &&
				this->mHorzFormatting != atres::RIGHT_WRAPPED && this->mHorzFormatting != atres::JUSTIFIED)
			{
				while (rect.x < 0.0f || rect.x < fh && this->mTextOffset.x < 0.0f && this->mCursorIndex > 0)
				{
					rect.x += fh;
					this->mTextOffset.x += fh;
				}
				while (rect.x + fh >= this->mRect.w)
				{
					rect.x -= fh;
					this->mTextOffset.x -= fh;
				}
			}
			while (rect.y < fh * 0.5f || rect.y < fh && this->mTextOffset.y < 0.0f)
			{
				rect.y += fh;
				this->mTextOffset.y += fh;
			}
			while (rect.y + fh * 0.5f >= this->mRect.h)
			{
				rect.y -= fh;
				this->mTextOffset.y -= fh;
			}
		}
		Label::OnDraw();
		this->mBackgroundColor.a = alpha;
		// caret render
		if (this->mDataset != NULL && this->mDataset->getFocusedObject() == this && this->mBlinkTimer < 0.5f)
		{
			rect.y -= fh * 0.5f - 2;
			rect.w = 1;
			rect.h = hmin(fh - 4, this->mRect.h - rect.y);
			if (rect.y < 0.0f)
			{
				rect.h += rect.y;
				rect.y = 0.0f;
			}
			if (rect.h > 0.0f)
			{
				april::rendersys->drawRect(rect - this->mRect.getSize() * 0.5f, this->_getDrawColor() * this->mTextColor);
			}
		}
		this->mText = text;
	}

	gvec2 EditBox::_makeCaretPosition(chstr text, chstr originalText)
	{
		gvec2 position;
		float fh = atres::renderer->getFontLineHeight(this->mFontName);
		// full text
		harray<atres::FormatTag> tags = atres::renderer->prepareTags(this->mFontName);
		harray<atres::RenderLine> fullLines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->mRect.w, CHECK_RECT_HEIGHT),
			originalText, tags, this->mHorzFormatting, this->mVertFormatting);
		int lineCount = fullLines.size();
		if (lineCount == 0 || fullLines.last().terminated)
		{
			lineCount++;
		}
		float w2 = this->mRect.w * 0.5f;
		float h2 = this->mRect.h * 0.5f;
		float xhf = 0.0f; // x height factor
		gvec2 base;
		if (this->mHorzFormatting == atres::CENTER || this->mHorzFormatting == atres::CENTER_WRAPPED)
		{
			base.x = w2;
		}
		else if (this->mHorzFormatting == atres::RIGHT || this->mHorzFormatting == atres::RIGHT_WRAPPED)
		{
			base.x = w2 * 2;
		}
		if (this->mVertFormatting == atres::CENTER)
		{
			xhf = 0.5f;
		}
		else if (this->mVertFormatting == atres::BOTTOM)
		{
			xhf = 1.0f;
		}
		base.y = (h2 * 2 - fh) * xhf;
		// caret position
		if (text != "")
		{
			tags = atres::renderer->prepareTags(this->mFontName);
			harray<atres::RenderLine> lines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->mRect.w, CHECK_RECT_HEIGHT),
				text, tags, this->mHorzFormatting, this->mVertFormatting, gvec2(), true);
			if (lines.size() > 0)
			{
				atres::RenderLine line = lines.last();
				atres::RenderLine fullLine = fullLines[lines.size() - 1];
				position.y += fullLine.rect.y + xhf * (this->mRect.h - CHECK_RECT_HEIGHT);
				if (line.terminated)
				{
					position.y += fh;
					if (fullLines.size() > lines.size())
					{
						fullLine = fullLines[lines.size()];
						position.x += fullLine.rect.x;
					}
					else
					{
						position.x += base.x;
					}
				}
				else
				{
					position.x += fullLine.rect.x + line.rect.w;
				}
			}
		}
		else if (fullLines.size() > 0)
		{
			position.x += fullLines[0].rect.x;
			position.y += fullLines[0].rect.y;
		}
		else
		{
			position += base;
		}
		if (this->mHorzFormatting == atres::RIGHT || this->mHorzFormatting == atres::RIGHT_WRAPPED)
		{
			position.x -= 1;
		}
		position += this->mTextOffset;
		position.y += fh * 0.5f;
		return position;
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
		else if (name == "onGainFocus")
		{
			april::window->beginKeyboardHandling();
		}
		Label::notifyEvent(name, params);
	}
	
	bool EditBox::onMouseDown(april::Key keyCode)
	{
		if (Object::onMouseDown(keyCode))
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

	bool EditBox::onMouseUp(april::Key keyCode)
	{
		if (Object::onMouseUp(keyCode))
		{
			return true;
		}
		if (this->mPushed && this->isCursorInside())
		{
			gvec2 position = this->transformToLocalSpace(aprilui::getCursorPosition());;
			this->setCursorIndexAt(position.x, position.y);
			this->setFocused(true);
			this->mPushed = false;
			this->triggerEvent("Click", keyCode);
			return true;
		}
		this->mPushed = false;
		return false;
	}

	bool EditBox::onKeyDown(april::Key keyCode)
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
			case april::AK_UP:
				this->mMultiLine ? this->_cursorMoveUp() : this->setCursorIndex(0);
				break;
			case april::AK_DOWN:
				this->mMultiLine ? this->_cursorMoveDown() : this->setCursorIndex(this->mText.utf8_size());
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
				this->setCursorIndex(this->mText.utf8_size());
				break;
			case april::AK_CONTROL:
				this->mCtrlMode = true;
				break;
#endif
			case april::AK_RETURN:
				if (this->mMultiLine)
				{
					this->_insertChar('\n');
				}
				this->triggerEvent("Submit", april::AK_RETURN);
				break;
			default:
				break;
			}
		}
		return false;
	}
	
	bool EditBox::onKeyUp(april::Key keyCode)
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
		default:
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
		if (this->mDataset == NULL || this->mDataset->getFocusedObject() == this)
		{
			if (atres::renderer->getFontResource(this->mFontName)->hasChar(charCode) && (this->mFilter.size() == 0 || this->mFilter.u_str().find_first_of(charCode) != std::string::npos))
			{
				this->_insertChar(charCode);
			}
		}
		return false;
	}

	void EditBox::mouseCancel()
	{
		this->mPushed = false;
		Object::mouseCancel();
	}

	hstr EditBox::getProperty(chstr name, bool* propertyExists)
	{
		if (propertyExists != NULL)
		{
			*propertyExists = true;
		}
		if (name == "max_length")		return this->getMaxLength();
		if (name == "password_char")	return this->getPasswordChar();
		if (name == "filter")			return this->getFilter();
		if (name == "empty_text")		return this->getEmptyText();
		if (name == "empty_text_key")	return this->getEmptyTextKey();
		if (name == "cursor_index")		return this->getCursorIndex();
		if (name == "multi_line")		return this->isMultiLine();
		if (name == "space_hack")		return this->mSpaceHack;
		return Label::getProperty(name, propertyExists);
	}
	
	bool EditBox::setProperty(chstr name, chstr value)
	{
		if		(name == "max_length")		this->setMaxLength(value);
		else if	(name == "password_char")	this->setPasswordChar(value.c_str()[0]);
		else if	(name == "filter")			this->setFilter(value);
		else if	(name == "empty_text")		this->setEmptyText(value);
		else if	(name == "empty_text_key")	this->setEmptyTextKey(value);
		else if (name == "cursor_index")	this->setCursorIndex(value);
		else if	(name == "multi_line")		this->setMultiLine(value);
		else if	(name == "space_hack")		this->mSpaceHack = (bool)value;
		else return Label::setProperty(name, value);
		return true;
	}
	
	void EditBox::_cursorMoveLeft()
	{
		this->setCursorIndex(this->mCursorIndex - 1);
	}
	
	void EditBox::_cursorMoveRight()
	{
		this->setCursorIndex(this->mCursorIndex + 1);
	}
	
	void EditBox::_cursorMoveUp()
	{
		if (this->mCursorIndex > 0)
		{
			gvec2 position = this->_makeCaretPosition(this->mText.utf8_substr(0, this->mCursorIndex), this->mText);
			this->setCursorIndexAt(position.x, position.y - atres::renderer->getFontLineHeight(this->mFontName));
		}
	}
	
	void EditBox::_cursorMoveDown()
	{
		if (this->mCursorIndex < this->mText.utf8_size())
		{
			gvec2 position = this->_makeCaretPosition(this->mText.utf8_substr(0, this->mCursorIndex), this->mText);
			this->setCursorIndexAt(position.x, position.y + atres::renderer->getFontLineHeight(this->mFontName));
		}
	}
	
	void EditBox::_cursorMoveLeftWord()
	{
		unsigned char c = 0;
		bool first = true;
		bool newLine = false;
		while (this->mCursorIndex > 0)
		{
			c = this->mText.first_unicode_char(mCursorIndex - 1);
			if (c != UNICODE_CHAR_SPACE && c != UNICODE_CHAR_NEWLINE)
			{
				break;
			}
			if (c == UNICODE_CHAR_NEWLINE)
			{
				newLine = true;
				if (first)
				{
					this->mCursorIndex--;
				}
				break;
			}
			first = false;
			this->mCursorIndex--;
		}
		if (!newLine)
		{
			while (this->mCursorIndex > 0)
			{
				c = this->mText.first_unicode_char(mCursorIndex - 1);
				if (c == UNICODE_CHAR_SPACE || c == UNICODE_CHAR_NEWLINE)
				{
					break;
				}
				this->mCursorIndex--;
			}
		}
		this->mBlinkTimer = 0.0f;
	}
	
	void EditBox::_cursorMoveRightWord()
	{
		int size = this->mText.utf8_size();
		unsigned char c = 0;
		bool first = true;
		bool newLine = false;
		while (this->mCursorIndex < size)
		{
			c = this->mText.first_unicode_char(mCursorIndex);
			if (c == UNICODE_CHAR_SPACE)
			{
				break;
			}
			if (c == UNICODE_CHAR_NEWLINE)
			{
				newLine = true;
				if (first)
				{
					this->mCursorIndex++;
				}
				break;
			}
			first = false;
			this->mCursorIndex++;
		}
		if (!newLine)
		{
			while (this->mCursorIndex < size)
			{
				c = this->mText.first_unicode_char(mCursorIndex);
				if (c != UNICODE_CHAR_SPACE && c != UNICODE_CHAR_NEWLINE)
				{
					break;
				}
				this->mCursorIndex++;
			}
		}
		this->mBlinkTimer = 0.0f;
	}
	
	void EditBox::_deleteLeft(int count)
	{
		if (this->mCursorIndex > 0)
		{
			count = hmin(count, this->mCursorIndex);
			hstr left;
			if (this->mCursorIndex > count)
			{
				left = this->mText.utf8_substr(0, this->mCursorIndex - count);
			}
			hstr right;
			int size = this->mText.utf8_size();
			if (this->mCursorIndex < size)
			{
				right = this->mText.utf8_substr(this->mCursorIndex, size - this->mCursorIndex);
			}
			this->mCursorIndex -= count;
			this->mText = left + right;
			this->mBlinkTimer = 0.0f;
		}
	}
	
	void EditBox::_deleteRight(int count)
	{
		count = hmin(count, this->mText.utf8_size() - this->mCursorIndex);
		this->mCursorIndex += count;
		this->_deleteLeft(count);
	}
	
	void EditBox::_deleteLeftWord()
	{
		int index = this->mCursorIndex;
		while (index > 0 && this->mText.first_unicode_char(index - 1) == UNICODE_CHAR_SPACE)
		{
			index--;
		}
		while (index > 0 && this->mText.first_unicode_char(index - 1) != UNICODE_CHAR_SPACE)
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
		int size = this->mText.utf8_size();
		while (index < size && this->mText.first_unicode_char(index) != UNICODE_CHAR_SPACE)
		{
			index++;
		}
		while (index < size && this->mText.first_unicode_char(index) == UNICODE_CHAR_SPACE)
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
		int size = this->mText.utf8_size();
		if (this->mMaxLength > 0 && size >= this->mMaxLength)
		{
			return;
		}
		hstr left;
		if (this->mCursorIndex > 0)
		{
			left = this->mText.utf8_substr(0, this->mCursorIndex);
		}
		hstr right;
		if (this->mCursorIndex < size)
		{
			right = this->mText.utf8_substr(this->mCursorIndex, size - this->mCursorIndex);
		}
		this->mCursorIndex++;
		this->mText = (left + hstr::from_unicode(charCode)) + right;
		this->mBlinkTimer = 0.0f;
	}
	

}
