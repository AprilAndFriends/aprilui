/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 3.04
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
	EditBox::EditBox(chstr name, grect rect) : Label(name, rect)
	{
		this->text = "";
		this->emptyText = "";
		this->emptyTextKey = "";
		this->horzFormatting = atres::LEFT;
		this->textFormatting = false;
		this->pushed = false;
		this->maxLength = 0;
		this->passwordChar = '\0';
		this->multiLine = false;
		this->cursorIndex = 0;
		this->backgroundColor = april::Color::Black;
		this->filter = "";
		this->_ctrlMode = false;
		this->_blinkTimer = 0.0f;
		/// TODO - remove
		this->spaceHack = false;
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
		this->cursorIndex = hclamp(value, 0, this->text.utf8_size());
		this->_blinkTimer = 0.0f;
	}
	
	void EditBox::setCursorIndexAt(float x, float y)
	{
		if (this->text == "")
		{
			this->setCursorIndex(0);
			return;
		}
		float fh = atres::renderer->getFontResource(this->font)->getLineHeight();
		// full text
		harray<atres::FormatTag> tags = atres::renderer->prepareTags(this->font);
		harray<atres::RenderLine> lines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT), this->text, tags, this->horzFormatting, this->vertFormatting);
		float w2 = this->rect.w * 0.5f;
		float h2 = this->rect.h * 0.5f;
		float xhf = 0.0f; // x height factor
		gvec2 base;
		if (this->horzFormatting == atres::CENTER || this->horzFormatting == atres::CENTER_WRAPPED)
		{
			base.x = w2;
		}
		else if (this->horzFormatting == atres::RIGHT || this->horzFormatting == atres::RIGHT_WRAPPED)
		{
			base.x = w2 * 2;
		}
		if (this->vertFormatting == atres::CENTER)
		{
			xhf = 0.5f;
		}
		else if (this->vertFormatting == atres::BOTTOM)
		{
			xhf = 1.0f;
		}
		base.y = (h2 * 2 - fh) * xhf;
		int offsetIndex = this->text.size();
		x -= this->textOffset.x;
		y += xhf * (CHECK_RECT_HEIGHT - this->rect.h) - this->textOffset.y;
		if (lines.size() > 0)
		{
			if (y >= lines.first().rect.y)
			{
				offsetIndex = this->text.size();
				atres::RenderLine* line = NULL;
				float descender = 0.0f;
				for_iter (i, 0, lines.size())
				{
					if (i == lines.size() - 1)
					{
						descender = atres::renderer->getFontResource(this->font)->getDescender();
					}
					if (is_in_range(y, lines[i].rect.y, lines[i].rect.y + lines[i].rect.h + descender))
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
								++offsetIndex;
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
		this->setCursorIndex(this->text(0, offsetIndex).utf8_size());
	}
	
	void EditBox::setMaxLength(int value)
	{
		this->maxLength = value;
		if (this->maxLength > 0 && this->text.utf8_size() > this->maxLength)
		{
			this->text = this->text.utf8_substr(0, this->maxLength);
			this->setCursorIndex(this->cursorIndex);
		}
	}

	void EditBox::setFilter(chstr value)
	{
		this->filter = value;
		this->setText(this->text);
	}
	
	void EditBox::setText(chstr value)
	{
		hstr newValue = value;
		if (newValue.size() > 0 && this->filter.size() > 0)
		{
			std::basic_string<unsigned int> ustr = newValue.u_str();
			harray<unsigned int> uText(ustr.c_str(), ustr.size());
			ustr = this->filter.u_str();
			harray<unsigned int> uFilter(ustr.c_str(), ustr.size());
			uFilter |= '\n'; // this is for multiline
			uText &= uFilter; // intersect, remove from first all that are not in second
			newValue = hstr::from_unicode(uText);
		}
		if (this->maxLength > 0 && newValue.utf8_size() > this->maxLength)
		{
			newValue = newValue.utf8_substr(0, this->maxLength);
		}
		Label::setText(newValue);
		this->setCursorIndex(this->cursorIndex);
	}

	void EditBox::setEmptyText(chstr value)
	{
		this->emptyText = value;
		this->emptyTextKey = "";
	}

	void EditBox::setEmptyTextKey(chstr value)
	{
		hstr emptyTextKey = value; // because value is a chstr which could reference textKey
		this->setEmptyText(this->getDataset()->getText(emptyTextKey));
		this->emptyTextKey = emptyTextKey;
	}

	void EditBox::setFocused(bool value)
	{
		Label::setFocused(value);
		if (this->dataset != NULL && value)
		{
			this->_blinkTimer = 0.0f;
		}
	}

	void EditBox::update(float time)
	{
		Label::update(time);
		if (!this->pushed)
		{
			this->_blinkTimer += time * 2;
			this->_blinkTimer = (this->_blinkTimer - (int)this->_blinkTimer);
		}
		else
		{
			gvec2 position = this->transformToLocalSpace(aprilui::getCursorPosition());;
			this->setCursorIndexAt(position.x, position.y);
		}
	}

	void EditBox::OnDraw()
	{
		//////////////
		// TODO - remove this hack, fix it in ATRES
		if (this->spaceHack)
		{
			this->text = this->text.ltrim();
			this->emptyText = this->emptyText.ltrim();
		}
		//////////////
		grect rect = this->_getDrawRect();
		hstr text = this->text;
		rect.w -= 12;
		if (this->passwordChar != '\0' && this->text != "")
		{
			this->text = hstr(this->passwordChar, this->text.utf8_size());
		}
		if (this->text == "" && this->dataset != NULL && this->dataset->getFocusedObject() != this)
		{
			this->text = this->emptyText;
		}
		unsigned char alpha = this->backgroundColor.a;
		if (this->pushed)
		{
			this->backgroundColor.a = (unsigned char)(this->backgroundColor.a * 0.75f);
		}
		float fh = atres::renderer->getFontResource(this->font)->getLineHeight();
		float descender = atres::renderer->getFontResource(this->font)->getDescender();
		if (this->dataset != NULL && this->dataset->getFocusedObject() == this)
		{
			hstr leftText = this->text.utf8_substr(0, this->cursorIndex);
			rect.setPosition(this->_makeCaretPosition(leftText, this->text));
			if (this->horzFormatting != atres::LEFT_WRAPPED && this->horzFormatting != atres::CENTER_WRAPPED &&
				this->horzFormatting != atres::RIGHT_WRAPPED && this->horzFormatting != atres::JUSTIFIED)
			{
				if (atres::renderer->getTextWidth(this->font, this->text) > rect.w)
				{
					while (rect.x < fh && (this->horzFormatting != atres::LEFT || this->textOffset.x < 0.0f))
					{
						rect.x += fh;
						this->textOffset.x += fh;
					}
					float boundary = (this->horzFormatting == atres::CENTER ? fh : 0.0f);
					boundary = (this->horzFormatting == atres::RIGHT ? 0.0f : fh);
					while (rect.x + fh > this->rect.w && (this->horzFormatting != atres::RIGHT || this->textOffset.x > 0.0f))
					{
						rect.x -= fh;
						this->textOffset.x -= fh;
					}
				}
			}
			while (rect.y < fh * 0.5f || rect.y < fh && this->textOffset.y < 0.0f)
			{
				rect.y += fh;
				this->textOffset.y += fh;
			}
			float lh = fh + descender;
			while (rect.y + lh * 0.5f > this->rect.h)
			{
				rect.y -= fh;
				this->textOffset.y -= fh;
			}
			this->caretPosition = rect.getPosition();
		}
		Label::OnDraw();
		this->backgroundColor.a = alpha;
		// caret render
		if (this->dataset != NULL && this->dataset->getFocusedObject() == this && this->_blinkTimer < 0.5f)
		{
			rect.y += 2 - fh * 0.5f;
			rect.w = 1;
			rect.h = hmin(fh + descender - 4, this->rect.h - rect.y);
			if (rect.y < 0.0f)
			{
				rect.h += rect.y;
				rect.y = 0.0f;
			}
			if (rect.h > 0.0f)
			{
				april::rendersys->drawRect(rect - this->center, this->_getDrawColor() * this->textColor);
			}
		}
		this->text = text;
	}

	gvec2 EditBox::_makeCaretPosition(chstr text, chstr originalText)
	{
		gvec2 position;
		float fh = atres::renderer->getFontResource(this->font)->getLineHeight();
		float descender = atres::renderer->getFontResource(this->font)->getDescender();
		// full text
		harray<atres::FormatTag> tags = atres::renderer->prepareTags(this->font);
		harray<atres::RenderLine> fullLines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT),
			originalText, tags, this->horzFormatting, this->vertFormatting);
		int lineCount = fullLines.size();
		if (lineCount == 0 || fullLines.last().terminated)
		{
			++lineCount;
		}
		float w2 = this->rect.w * 0.5f;
		float h2 = this->rect.h * 0.5f;
		float xhf = 0.0f; // multiplier height factor
		gvec2 base;
		if (this->horzFormatting == atres::CENTER || this->horzFormatting == atres::CENTER_WRAPPED)
		{
			base.x = w2;
		}
		else if (this->horzFormatting == atres::RIGHT || this->horzFormatting == atres::RIGHT_WRAPPED)
		{
			base.x = w2 * 2;
		}
		if (this->vertFormatting == atres::CENTER)
		{
			xhf = 0.5f;
		}
		else if (this->vertFormatting == atres::BOTTOM)
		{
			xhf = 1.0f;
		}
		base.y = (h2 * 2 - fh - descender) * xhf;
		// caret position
		if (text != "")
		{
			tags = atres::renderer->prepareTags(this->font);
			harray<atres::RenderLine> lines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT),
				text, tags, this->horzFormatting, this->vertFormatting, gvec2(), true);
			if (lines.size() > 0)
			{
				atres::RenderLine line = lines.last();
				atres::RenderLine fullLine = fullLines[lines.size() - 1];
				position.y += fullLine.rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT);
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
			position.y += fullLines[0].rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT);
		}
		else
		{
			position += base;
		}
		if (this->horzFormatting == atres::RIGHT || this->horzFormatting == atres::RIGHT_WRAPPED)
		{
			position.x -= 1;
		}
		position += this->textOffset;
		position.y += fh * 0.5f;
		return position;
	}
	
	void EditBox::notifyEvent(chstr name, void* params)
	{
		if (name == "onLocalizationChanged")
		{
			if (this->emptyTextKey != "")
			{
				this->setEmptyTextKey(this->emptyTextKey);
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
			gvec2 position = this->transformToLocalSpace(aprilui::getCursorPosition());;
			this->setCursorIndexAt(position.x, position.y);
			this->setFocused(true);
			this->pushed = true;
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
		if (this->pushed && this->isCursorInside())
		{
			this->pushed = false;
			this->triggerEvent("Click", keyCode);
			return true;
		}
		this->pushed = false;
		return false;
	}

	bool EditBox::onKeyDown(april::Key keyCode)
	{
		if (Object::onKeyDown(keyCode))
		{
			return true;
		}
		if (this->dataset == NULL || this->dataset->getFocusedObject() == this)
		{
			switch (keyCode)
			{
#ifndef _ANDROID // these keys aren't really available on Android
			case april::AK_LEFT:
				this->_ctrlMode ? this->_cursorMoveLeftWord() : this->_cursorMoveLeft();
				break;
			case april::AK_RIGHT:
				this->_ctrlMode ? this->_cursorMoveRightWord() : this->_cursorMoveRight();
				break;
			case april::AK_UP:
				this->multiLine ? this->_cursorMoveUp() : this->setCursorIndex(0);
				break;
			case april::AK_DOWN:
				this->multiLine ? this->_cursorMoveDown() : this->setCursorIndex(this->text.utf8_size());
				break;
#endif
			case april::AK_BACK:
				this->_ctrlMode ? this->_deleteLeftWord() : this->_deleteLeft();
				break;
			case april::AK_DELETE:
				this->_ctrlMode ? this->_deleteRightWord() : this->_deleteRight();
				break;
#ifndef _ANDROID // these keys aren't really available on Android
			case april::AK_HOME:
				this->setCursorIndex(0);
				break;
			case april::AK_END:
				this->setCursorIndex(this->text.utf8_size());
				break;
			case april::AK_CONTROL:
				this->_ctrlMode = true;
				break;
#endif
			case april::AK_RETURN:
				if (this->multiLine)
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
			this->_ctrlMode = false;
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
		if (this->dataset == NULL || this->dataset->getFocusedObject() == this)
		{
			if (atres::renderer->getFontResource(this->font)->hasChar(charCode) && (this->filter.size() == 0 || this->filter.u_str().find_first_of(charCode) != std::string::npos))
			{
				this->_insertChar(charCode);
			}
		}
		return false;
	}

	void EditBox::mouseCancel()
	{
		this->pushed = false;
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
		if (name == "space_hack")		return this->spaceHack;
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
		else if	(name == "space_hack")		this->spaceHack = (bool)value;
		else return Label::setProperty(name, value);
		return true;
	}
	
	void EditBox::_cursorMoveLeft()
	{
		this->setCursorIndex(this->cursorIndex - 1);
	}
	
	void EditBox::_cursorMoveRight()
	{
		this->setCursorIndex(this->cursorIndex + 1);
	}
	
	void EditBox::_cursorMoveUp()
	{
		if (this->cursorIndex > 0)
		{
			gvec2 position = this->_makeCaretPosition(this->text.utf8_substr(0, this->cursorIndex), this->text);
			this->setCursorIndexAt(position.x, position.y - atres::renderer->getFontResource(this->font)->getLineHeight());
		}
	}
	
	void EditBox::_cursorMoveDown()
	{
		if (this->cursorIndex < this->text.utf8_size())
		{
			gvec2 position = this->_makeCaretPosition(this->text.utf8_substr(0, this->cursorIndex), this->text);
			this->setCursorIndexAt(position.x, position.y + atres::renderer->getFontResource(this->font)->getLineHeight());
		}
	}
	
	void EditBox::_cursorMoveLeftWord()
	{
		unsigned char c = 0;
		bool first = true;
		bool newLine = false;
		while (this->cursorIndex > 0)
		{
			c = this->text.first_unicode_char(cursorIndex - 1);
			if (c != UNICODE_CHAR_SPACE && c != UNICODE_CHAR_NEWLINE)
			{
				break;
			}
			if (c == UNICODE_CHAR_NEWLINE)
			{
				newLine = true;
				if (first)
				{
					--this->cursorIndex;
				}
				break;
			}
			first = false;
			--this->cursorIndex;
		}
		if (!newLine)
		{
			while (this->cursorIndex > 0)
			{
				c = this->text.first_unicode_char(cursorIndex - 1);
				if (c == UNICODE_CHAR_SPACE || c == UNICODE_CHAR_NEWLINE)
				{
					break;
				}
				--this->cursorIndex;
			}
		}
		this->_blinkTimer = 0.0f;
	}
	
	void EditBox::_cursorMoveRightWord()
	{
		int size = this->text.utf8_size();
		unsigned char c = 0;
		bool first = true;
		bool newLine = false;
		while (this->cursorIndex < size)
		{
			c = this->text.first_unicode_char(cursorIndex);
			if (c == UNICODE_CHAR_SPACE)
			{
				break;
			}
			if (c == UNICODE_CHAR_NEWLINE)
			{
				newLine = true;
				if (first)
				{
					++this->cursorIndex;
				}
				break;
			}
			first = false;
			++this->cursorIndex;
		}
		if (!newLine)
		{
			while (this->cursorIndex < size)
			{
				c = this->text.first_unicode_char(cursorIndex);
				if (c != UNICODE_CHAR_SPACE && c != UNICODE_CHAR_NEWLINE)
				{
					break;
				}
				++this->cursorIndex;
			}
		}
		this->_blinkTimer = 0.0f;
	}
	
	void EditBox::_deleteLeft(int count)
	{
		if (this->cursorIndex > 0)
		{
			count = hmin(count, this->cursorIndex);
			hstr left;
			if (this->cursorIndex > count)
			{
				left = this->text.utf8_substr(0, this->cursorIndex - count);
			}
			hstr right;
			int size = this->text.utf8_size();
			if (this->cursorIndex < size)
			{
				right = this->text.utf8_substr(this->cursorIndex, size - this->cursorIndex);
			}
			this->cursorIndex -= count;
			this->text = left + right;
			this->_blinkTimer = 0.0f;
		}
	}
	
	void EditBox::_deleteRight(int count)
	{
		count = hmin(count, this->text.utf8_size() - this->cursorIndex);
		this->cursorIndex += count;
		this->_deleteLeft(count);
	}
	
	void EditBox::_deleteLeftWord()
	{
		int index = this->cursorIndex;
		while (index > 0 && this->text.first_unicode_char(index - 1) == UNICODE_CHAR_SPACE)
		{
			--index;
		}
		while (index > 0 && this->text.first_unicode_char(index - 1) != UNICODE_CHAR_SPACE)
		{
			--index;
		}
		if (this->cursorIndex > index)
		{
			this->_deleteLeft(this->cursorIndex - index);
		}
	}
	
	void EditBox::_deleteRightWord()
	{
		int index = this->cursorIndex;
		int size = this->text.utf8_size();
		while (index < size && this->text.first_unicode_char(index) != UNICODE_CHAR_SPACE)
		{
			++index;
		}
		while (index < size && this->text.first_unicode_char(index) == UNICODE_CHAR_SPACE)
		{
			++index;
		}
		if (index > this->cursorIndex)
		{
			this->_deleteRight(index - this->cursorIndex);
		}
	}
	
	void EditBox::_insertChar(unsigned int charCode)
	{
		int size = this->text.utf8_size();
		if (this->maxLength > 0 && size >= this->maxLength)
		{
			return;
		}
		hstr left;
		if (this->cursorIndex > 0)
		{
			left = this->text.utf8_substr(0, this->cursorIndex);
		}
		hstr right;
		if (this->cursorIndex < size)
		{
			right = this->text.utf8_substr(this->cursorIndex, size - this->cursorIndex);
		}
		++this->cursorIndex;
		this->text = (left + hstr::from_unicode(charCode)) + right;
		this->_blinkTimer = 0.0f;
	}
	

}
