/// @file
/// @version 3.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Keys.h>
#include <atres/atres.h>
#include <atres/Font.h>
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
	harray<PropertyDescription> EditBox::_propertyDescriptions;

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
		this->selectable = false;
		this->filter = "";
		this->caretIndex = 0;
		this->selectionCount = 0;
		this->backgroundColor = april::Color::Black;
		this->selectionColor = april::Color::Grey;
		this->_ctrlMode = false;
		this->_shiftMode = false;
		this->_blinkTimer = 0.0f;
		this->_sizeProblemReported = false;
		this->renderOffsetX = 0;
		this->renderOffsetY = 0;
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

	harray<PropertyDescription> EditBox::getPropertyDescriptions()
	{
		if (EditBox::_propertyDescriptions.size() == 0)
		{
			EditBox::_propertyDescriptions += PropertyDescription("max_length", PropertyDescription::INT);
			EditBox::_propertyDescriptions += PropertyDescription("password_char", PropertyDescription::CHAR);
			EditBox::_propertyDescriptions += PropertyDescription("filter", PropertyDescription::STRING);
			EditBox::_propertyDescriptions += PropertyDescription("empty_text", PropertyDescription::STRING);
			EditBox::_propertyDescriptions += PropertyDescription("empty_text_key", PropertyDescription::STRING);
			EditBox::_propertyDescriptions += PropertyDescription("caret_index", PropertyDescription::INT);
			EditBox::_propertyDescriptions += PropertyDescription("multi_line", PropertyDescription::BOOL);
			EditBox::_propertyDescriptions += PropertyDescription("selectable", PropertyDescription::BOOL);
			EditBox::_propertyDescriptions += PropertyDescription("selection_color", PropertyDescription::HEXCOLOR);
			EditBox::_propertyDescriptions += PropertyDescription("space_hack", PropertyDescription::BOOL);
		}
		return (Label::getPropertyDescriptions() + EditBox::_propertyDescriptions);
	}

	void EditBox::setCaretIndex(int value)
	{
		this->caretIndex = hclamp(value, 0, this->text.utf8_size());
		this->_blinkTimer = 0.0f;
	}

	void EditBox::setCaretIndexAt(gvec2 position)
	{
		this->setCaretIndex(this->text(0, this->_makeCaretIndexAt(position)).utf8_size());
	}

	void EditBox::_getBaseOffset(gvec2& offset, float& xhf)
	{
		offset.set(0.0f, 0.0f);
		xhf = 0.0f; // x height factor
		float fh = atres::renderer->getFont(this->font)->getLineHeight();
		float w2 = this->rect.w * 0.5f;
		float h2 = this->rect.h * 0.5f;
		if (this->horzFormatting == atres::CENTER || this->horzFormatting == atres::CENTER_WRAPPED)
		{
			offset.x = w2;
		}
		else if (this->horzFormatting == atres::RIGHT || this->horzFormatting == atres::RIGHT_WRAPPED)
		{
			offset.x = w2 * 2;
		}
		if (this->vertFormatting == atres::CENTER)
		{
			xhf = 0.5f;
		}
		else if (this->vertFormatting == atres::BOTTOM)
		{
			xhf = 1.0f;
		}
		offset.y = (h2 * 2 - fh) * xhf;
	}
	
	int EditBox::_makeCaretIndexAt(gvec2 position)
	{
		if (this->text == "")
		{
			return 0;
		}
		float fh = atres::renderer->getFont(this->font)->getLineHeight();
		// full text
		harray<atres::FormatTag> tags = atres::renderer->prepareTags(this->font);
		harray<atres::RenderLine> lines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT), this->text, tags, this->horzFormatting, this->vertFormatting);
		gvec2 base;
		float xhf = 0.0f; // x height factor
		this->_getBaseOffset(base, xhf);
		int offsetIndex = this->text.size();
		position.x -= this->renderOffsetX * fh;
		position.y += xhf * (CHECK_RECT_HEIGHT - this->rect.h) - this->renderOffsetY * fh;
		if (lines.size() > 0)
		{
			if (position.y >= lines.first().rect.y)
			{
				offsetIndex = this->text.size();
				atres::RenderLine* line = NULL;
				float descender = 0.0f;
				for_iter(i, 0, lines.size())
				{
					if (i == lines.size() - 1)
					{
						descender = atres::renderer->getFont(this->font)->getDescender();
					}
					if (is_between_ie(position.y, lines[i].rect.y, lines[i].rect.y + lines[i].rect.h + descender))
					{
						if (position.x <= lines[i].rect.x + lines[i].rect.w)
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
						foreach(atres::RenderWord, it, line->words)
						{
							if (is_between_ie(position.x, (*it).rect.x, (*it).rect.x + (*it).rect.w))
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
							foreach(float, it, word->charWidths)
							{
								cw = (*it) * 0.5f;
								if (is_between_ie(position.x, ow, ow + cw))
								{
									break;
								}
								++offsetIndex;
								ow += cw;
								if (is_between_ie(position.x, ow, ow + cw))
								{
									break;
								}
								ow += cw;
							}
						}
						else if (position.x > line->rect.x)
						{
							offsetIndex = line->start + line->count;
						}
					}
				}
			}
			else
			{
				offsetIndex = 0;
			}
		}
		return offsetIndex;
	}

	void EditBox::setSelectionCount(int value)
	{
		this->selectionCount = (this->selectable ? hclamp(value, -this->caretIndex, this->text.utf8_size() - this->caretIndex) : 0);
	}

	void EditBox::setMaxLength(int value)
	{
		this->maxLength = value;
		if (this->maxLength > 0 && this->text.utf8_size() > this->maxLength)
		{
			this->text = this->text.utf8_substr(0, this->maxLength);
			this->setCaretIndex(this->caretIndex);
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
		this->setCaretIndex(this->caretIndex);
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

	hstr EditBox::getSelectedText()
	{
		return (this->selectionCount != 0 ? this->text.utf8_substr(hmin(this->caretIndex, this->caretIndex + this->selectionCount), habs(this->selectionCount)) : hstr(""));
	}

	void EditBox::update(float time)
	{
		Label::update(time);
		if (!this->pushed)
		{
			this->_blinkTimer += time * 2;
			this->_blinkTimer = (this->_blinkTimer - (int)this->_blinkTimer);
		}
		else if (this->selectable)
		{
			this->selectionCount = this->_makeCaretIndexAt(this->transformToLocalSpace(aprilui::getCursorPosition())) - this->caretIndex;
		}
		else
		{
			this->selectionCount = 0;
			this->setCaretIndexAt(this->transformToLocalSpace(aprilui::getCursorPosition()));
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
		april::Color drawColor = this->_getDrawColor();
		grect drawRect = this->_getDrawRect();
		grect caretRect = drawRect;
		hstr text = this->text;
		caretRect.w -= 12;
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
		float fh = atres::renderer->getFont(this->font)->getLineHeight();
		float descender = atres::renderer->getFont(this->font)->getDescender();
		float lh = fh + descender;
		int jumps = 0;
		if (this->dataset != NULL && this->dataset->getFocusedObject() == this)
		{
			hstr leftText = this->text.utf8_substr(0, this->caretIndex);
			caretRect.setPosition(this->_makeCaretPosition(leftText, this->text));
			if (this->horzFormatting != atres::LEFT_WRAPPED && this->horzFormatting != atres::CENTER_WRAPPED &&
				this->horzFormatting != atres::RIGHT_WRAPPED && this->horzFormatting != atres::JUSTIFIED)
			{
				if (atres::renderer->getTextWidth(this->font, this->text) > caretRect.w)
				{
					// left side
					if (caretRect.x < fh && (this->horzFormatting != atres::LEFT || this->renderOffsetX < 0))
					{
						jumps = hceil((fh - caretRect.x) / fh);
						if (this->horzFormatting == atres::LEFT)
						{
							jumps = hmin(jumps, -this->renderOffsetX);
						}
						if (jumps != 0)
						{
							caretRect.x += jumps * fh;
							this->renderOffsetX += jumps;
						}
					}
					// right side
					if (caretRect.x + fh > this->rect.w && (this->horzFormatting != atres::RIGHT || this->renderOffsetX > 0))
					{
						jumps = -hceil((caretRect.x + fh - this->rect.w) / fh);
						if (this->horzFormatting == atres::RIGHT)
						{
							jumps = hmax(jumps, -this->renderOffsetX);
						}
						if (jumps != 0)
						{
							caretRect.x += jumps * fh;
							this->renderOffsetX += jumps;
						}
					}
				}
				else
				{
					this->renderOffsetX = 0;
				}
			}
			if (this->multiLine)
			{
				// top side
				if (caretRect.y < fh * 0.5f && (this->horzFormatting != atres::TOP || this->renderOffsetY < 0))
				{
					jumps = hceil((fh - caretRect.y) / fh);
					if (this->vertFormatting == atres::TOP)
					{
						jumps = hmin(jumps, -this->renderOffsetY);
					}
					if (jumps != 0)
					{
						caretRect.y += jumps * fh;
						this->renderOffsetY += jumps;
					}
				}
				// bottom side
				if (caretRect.y + lh * 0.5f > this->rect.h && (this->horzFormatting != atres::BOTTOM || this->renderOffsetY > 0))
				{
					jumps = -hceil((caretRect.y + lh * 0.5f - this->rect.h) / fh);
					if (this->vertFormatting == atres::BOTTOM)
					{
						jumps = hmax(jumps, -this->renderOffsetY);
					}
					if (jumps != 0)
					{
						caretRect.y += jumps * fh;
						this->renderOffsetY += jumps;
					}
				}
			}
			else
			{
				this->renderOffsetY = 0;
			}
			this->textOffset.set(this->renderOffsetX * fh, this->renderOffsetY * fh);
			this->caretPosition = caretRect.getPosition();
		}
		if (this->multiLine && !this->_sizeProblemReported && this->rect.h < lh)
		{
			hlog::warnf(aprilui::logTag, "EditBox '%s' height (%d) is smaller than the minimum needed line height (%d) for the given font '%s' when using multi-line!", this->name.c_str(), (int)this->rect.h, (int)lh, this->font.c_str());
			this->_sizeProblemReported = true;
		}
		float caretHeight = hmin(fh, this->rect.h - caretRect.y);
		// not using Label::OnDraw() directly
		Object::OnDraw();
		float disabledAlphaFactor = this->_getDisabledAlphaFactor();
		drawColor.a = (unsigned char)(drawColor.a * disabledAlphaFactor);
		this->backgroundColor.a = (unsigned char)(this->backgroundColor.a * disabledAlphaFactor);
		LabelBase::_drawLabelBackground(drawRect, drawColor, this->backgroundColor);
		if (this->selectionCount != 0)
		{
			april::Color selectionColor = this->selectionColor;
			selectionColor.a = (unsigned char)(selectionColor.a * disabledAlphaFactor);
			harray<grect> selectionRects = this->_makeSelectedRects();
			foreach (grect, it, selectionRects)
			{
				(*it) += drawRect.getPosition();
				(*it).clip(drawRect);
				april::rendersys->drawFilledRect((*it), selectionColor);
			}
		}
		LabelBase::_drawLabel(drawRect, drawColor);
		this->backgroundColor.a = alpha;
		// caret render
		if (this->dataset != NULL && this->dataset->getFocusedObject() == this && this->_blinkTimer < 0.5f)
		{
			caretRect.y -= fh * 0.5f;
			caretRect.w = 1;
			caretRect.h = caretHeight;
			if (caretRect.y < 0.0f)
			{
				caretRect.h += caretRect.y;
				caretRect.y = 0.0f;
			}
			if (caretRect.h > 0.0f)
			{
				april::rendersys->drawRect(caretRect - this->center, drawColor * this->textColor);
			}
		}
		this->text = text;
	}

	gvec2 EditBox::_makeCaretPosition(chstr text, chstr originalText)
	{
		gvec2 position;
		float fh = atres::renderer->getFont(this->font)->getLineHeight();
		float descender = atres::renderer->getFont(this->font)->getDescender();
		// full text
		harray<atres::FormatTag> tags = atres::renderer->prepareTags(this->font);
		harray<atres::RenderLine> fullLines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT),
			originalText, tags, this->horzFormatting, this->vertFormatting);
		gvec2 base;
		float xhf = 0.0f; // x height factor
		this->_getBaseOffset(base, xhf);
		base.y -= descender * xhf;
		// caret position
		harray<atres::RenderLine> lines;
		if (text != "")
		{
			tags = atres::renderer->prepareTags(this->font);
			lines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT),
				text, tags, this->horzFormatting, this->vertFormatting, gvec2(), true);
			if (lines.size() > 0)
			{
				atres::RenderLine line = lines.last();
				atres::RenderLine fullLine = fullLines[lines.size() - 1];
				position.y = fullLine.rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT);
				if (line.terminated)
				{
					position.y += fh;
					if (fullLines.size() > lines.size())
					{
						position.x = fullLines[lines.size()].rect.x;
					}
					else
					{
						position.x = base.x;
					}
				}
				else
				{
					position.x = fullLine.rect.x + line.rect.w;
				}
			}
		}
		if (text == "" || lines.size() == 0)
		{
			if (fullLines.size() > 0)
			{
				position.set(fullLines[0].rect.x, fullLines[0].rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT));
			}
			else
			{
				position = base;
			}
		}
		if (this->horzFormatting == atres::RIGHT || this->horzFormatting == atres::RIGHT_WRAPPED)
		{
			position.x -= 1;
		}
		position += gvec2((float)this->renderOffsetX, (float)this->renderOffsetY) * fh;
		position.y += fh * 0.5f;
		return position;
	}

	harray<grect> EditBox::_makeSelectedRects()
	{
		harray<grect> result;
		if (this->selectionCount == 0 || this->text == "")
		{
			return result;
		}
		grect rect;
		float fh = atres::renderer->getFont(this->font)->getLineHeight();
		float descender = atres::renderer->getFont(this->font)->getDescender();
		// full text
		harray<atres::FormatTag> tags = atres::renderer->prepareTags(this->font);
		harray<atres::RenderLine> fullLines = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT),
			this->text, tags, this->horzFormatting, this->vertFormatting);
		gvec2 base;
		float xhf = 0.0f; // x height factor
		this->_getBaseOffset(base, xhf);
		// vars
		hstr textStart = this->text.utf8_substr(0, hmin(this->caretIndex, this->caretIndex + this->selectionCount));
		hstr textEnd = this->text.utf8_substr(0, hmax(this->caretIndex, this->caretIndex + this->selectionCount));
		harray<atres::RenderLine> linesStart;
		harray<atres::RenderLine> linesEnd;
		gvec2 positionStart;
		gvec2 positionEnd;
		atres::RenderLine* fullLineStart = NULL;
		atres::RenderLine* fullLineEnd = NULL;
		int linesEndCount = 0;
		if (textStart != "")
		{
			tags = atres::renderer->prepareTags(this->font);
			linesStart = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT),
				textStart, tags, this->horzFormatting, this->vertFormatting, gvec2(), true);
			if (linesStart.size() > 0)
			{
				fullLineStart = &fullLines[linesStart.size() - 1];
				positionStart.set(fullLineStart->rect.x + linesStart.last().rect.w, fullLineStart->rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT));
			}
		}
		if (textStart == "" || linesStart.size() == 0)
		{
			if (fullLines.size() > 0)
			{
				fullLineStart = &fullLines[0];
				positionStart.set(fullLineStart->rect.x, fullLineStart->rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT));
			}
			else
			{
				positionStart += base;
			}
		}
		if (textEnd != this->text)
		{
			tags = atres::renderer->prepareTags(this->font);
			linesEnd = atres::renderer->createRenderLines(grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT),
				textEnd, tags, this->horzFormatting, this->vertFormatting, gvec2(), true);
			fullLineEnd = &fullLines[linesEnd.size() - 1];
			positionEnd.set(fullLineEnd->rect.x + linesEnd.last().rect.w, fullLineEnd->rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT));
			linesEndCount = linesEnd.size();
		}
		else if (fullLines.size() > 0)
		{
			fullLineEnd = &fullLines.last();
			positionEnd.set(fullLineEnd->rect.right(), fullLineEnd->rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT));
			linesEndCount = fullLines.size();
		}
		else
		{
			positionEnd += base;
		}
		gvec2 renderOffset(this->renderOffsetX * fh, this->renderOffsetY * fh);
		if (linesStart.size() == linesEndCount)
		{
			result += grect(positionStart + renderOffset, positionEnd.x - positionStart.x, fh);
		}
		else
		{
			if (fullLineStart != NULL)
			{
				result += grect(positionStart + renderOffset, fullLineStart->rect.right() - positionStart.x, fh);
			}
			if (linesEndCount - linesStart.size() > 1)
			{
				for_iter (i, linesStart.size(), linesEndCount)
				{
					result += grect(fullLines[i].rect.getPosition(), fullLines[i].rect.w, fh);
				}
			}
			if (fullLineEnd != NULL)
			{
				result += grect(fullLineEnd->rect.x + renderOffset.x, positionEnd.y + renderOffset.y, positionEnd.x - fullLineStart->rect.x, fh);
			}
		}
		return result;
	}

	void EditBox::notifyEvent(chstr type, EventArgs* args)
	{
		if (type == Event::LocalizationChanged)
		{
			if (this->emptyTextKey != "")
			{
				this->setEmptyTextKey(this->emptyTextKey);
			}
		}
		else if (type == Event::FocusGained)
		{
			if (!this->pushed) // some OSes will disable the keyboard if it is shown before a mouse-up event
			{
				april::window->beginKeyboardHandling();
			}
		}
		Label::notifyEvent(type, args);
	}
	
	bool EditBox::onMouseDown(april::Key keyCode)
	{
		if (Object::onMouseDown(keyCode))
		{
			this->selectionCount = 0;
			return true;
		}
		if (this->isCursorInside())
		{
			this->caretIndex = this->_makeCaretIndexAt(this->transformToLocalSpace(aprilui::getCursorPosition()));
			this->selectionCount = 0;
			this->setFocused(true);
			this->pushed = true;
			return true;
		}
		this->selectionCount = 0;
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
			// some OSes will disable the keyboard if it is shown before a mouse-up event
			april::window->beginKeyboardHandling();
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
				this->_ctrlMode ? this->_caretMoveLeftWord() : this->_caretMoveLeft();
				break;
			case april::AK_RIGHT:
				this->_ctrlMode ? this->_caretMoveRightWord() : this->_caretMoveRight();
				break;
			case april::AK_UP:
				this->multiLine ? this->_caretMoveUp() : this->setCaretIndex(0);
				break;
			case april::AK_DOWN:
				this->multiLine ? this->_caretMoveDown() : this->setCaretIndex(this->text.utf8_size());
				break;
#endif
			case april::AK_BACK:
				if (!this->_deleteSelected())
				{
					this->_ctrlMode ? this->_deleteLeftWord() : this->_deleteLeft();
				}
				break;
			case april::AK_DELETE:
				if (!this->_deleteSelected())
				{
					this->_ctrlMode ? this->_deleteRightWord() : this->_deleteRight();
				}
				break;
#ifndef _ANDROID // these keys aren't really available on Android
			case april::AK_HOME:
				this->setCaretIndex(0);
				break;
			case april::AK_END:
				this->setCaretIndex(this->text.utf8_size());
				break;
			case april::AK_CONTROL:
				this->_ctrlMode = true;
				break;
			case april::AK_SHIFT:
				this->_shiftMode = true;
				break;
#endif
			case april::AK_RETURN:
				if (this->multiLine)
				{
					this->_insertChar('\n');
				}
				this->triggerEvent(Event::SubmitEditText, april::AK_RETURN);
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
		case april::AK_SHIFT:
			this->_shiftMode = false;
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
			if (atres::renderer->getFont(this->font)->hasChar(charCode) && (this->filter.size() == 0 || this->filter.u_str().find_first_of(charCode) != std::string::npos))
			{
				this->_insertChar(charCode);
			}
		}
		return false;
	}

	void EditBox::mouseCancel()
	{
		this->pushed = false;
		Label::mouseCancel();
	}

	hstr EditBox::getProperty(chstr name)
	{
		if (name == "max_length")		return this->getMaxLength();
		if (name == "password_char")	return this->getPasswordChar();
		if (name == "filter")			return this->getFilter();
		if (name == "empty_text")		return this->getEmptyText();
		if (name == "empty_text_key")	return this->getEmptyTextKey();
		if (name == "caret_index")		return this->getCaretIndex();
		if (name == "cursor_index")
		{
			hlog::warn(aprilui::logTag, "'cursor_index' is deprecated. Use 'caret_index' instead."); // DEPRECATED
			return this->getCaretIndex();
		}
		if (name == "multi_line")		return this->isMultiLine();
		if (name == "selectable")		return this->isSelectable();
		if (name == "selection_color")	return this->getSelectionColor().hex();
		if (name == "space_hack")		return this->spaceHack;
		return Label::getProperty(name);
	}
	
	bool EditBox::setProperty(chstr name, chstr value)
	{
		if		(name == "max_length")		this->setMaxLength(value);
		else if	(name == "password_char")	this->setPasswordChar(value.c_str()[0]);
		else if	(name == "filter")			this->setFilter(value);
		else if	(name == "empty_text")		this->setEmptyText(value);
		else if	(name == "empty_text_key")	this->setEmptyTextKey(value);
		else if (name == "caret_index")		this->setCaretIndex(value);
		else if (name == "cursor_index")
		{
			hlog::warn(aprilui::logTag, "'cursor_index=' is deprecated. Use 'caret_index=' instead."); // DEPRECATED
			this->setCaretIndex(value);
		}
		else if (name == "multi_line")		this->setMultiLine(value);
		else if (name == "selectable")		this->setSelectable(value);
		else if (name == "selection_color")	this->setSelectionColor(value);
		else if (name == "space_hack")		this->spaceHack = (bool)value;
		else return Label::setProperty(name, value);
		return true;
	}
	
	void EditBox::_caretMoveLeft()
	{
		this->_shiftMode && this->selectable ? ++this->selectionCount : this->selectionCount = 0;
		this->setCaretIndex(this->caretIndex - 1);
	}
	
	void EditBox::_caretMoveRight()
	{
		this->_shiftMode && this->selectable ? --this->selectionCount : this->selectionCount = 0;
		this->setCaretIndex(this->caretIndex + 1);
	}
	
	void EditBox::_caretMoveUp()
	{
		int previousCaretIndex = this->caretIndex;
		if (this->caretIndex > 0)
		{
			gvec2 position = this->_makeCaretPosition(this->text.utf8_substr(0, this->caretIndex), this->text);
			position.y -= atres::renderer->getFont(this->font)->getLineHeight();
			this->setCaretIndexAt(position);
		}
		this->_shiftMode && this->selectable ? this->selectionCount += previousCaretIndex - this->caretIndex : this->selectionCount = 0;
	}
	
	void EditBox::_caretMoveDown()
	{
		int previousCaretIndex = this->caretIndex;
		if (this->caretIndex < this->text.utf8_size())
		{
			gvec2 position = this->_makeCaretPosition(this->text.utf8_substr(0, this->caretIndex), this->text);
			position.y += atres::renderer->getFont(this->font)->getLineHeight();
			this->setCaretIndexAt(position);
		}
		this->_shiftMode && this->selectable ? this->selectionCount += previousCaretIndex - this->caretIndex : this->selectionCount = 0;
	}
	
	void EditBox::_caretMoveLeftWord()
	{
		int previousCaretIndex = this->caretIndex;
		unsigned char c = 0;
		bool first = true;
		bool newLine = false;
		while (this->caretIndex > 0)
		{
			c = this->text.first_unicode_char(caretIndex - 1);
			if (c != UNICODE_CHAR_SPACE && c != UNICODE_CHAR_NEWLINE)
			{
				break;
			}
			if (c == UNICODE_CHAR_NEWLINE)
			{
				newLine = true;
				if (first)
				{
					--this->caretIndex;
				}
				break;
			}
			first = false;
			--this->caretIndex;
		}
		if (!newLine)
		{
			while (this->caretIndex > 0)
			{
				c = this->text.first_unicode_char(caretIndex - 1);
				if (c == UNICODE_CHAR_SPACE || c == UNICODE_CHAR_NEWLINE)
				{
					break;
				}
				--this->caretIndex;
			}
		}
		this->_shiftMode && this->selectable ? this->selectionCount += previousCaretIndex - this->caretIndex : this->selectionCount = 0;
		this->_blinkTimer = 0.0f;
	}
	
	void EditBox::_caretMoveRightWord()
	{
		int previousCaretIndex = this->caretIndex;
		int size = this->text.utf8_size();
		unsigned char c = 0;
		bool first = true;
		bool newLine = false;
		while (this->caretIndex < size)
		{
			c = this->text.first_unicode_char(caretIndex);
			if (c == UNICODE_CHAR_SPACE)
			{
				break;
			}
			if (c == UNICODE_CHAR_NEWLINE)
			{
				newLine = true;
				if (first)
				{
					++this->caretIndex;
				}
				break;
			}
			first = false;
			++this->caretIndex;
		}
		if (!newLine)
		{
			while (this->caretIndex < size)
			{
				c = this->text.first_unicode_char(caretIndex);
				if (c != UNICODE_CHAR_SPACE && c != UNICODE_CHAR_NEWLINE)
				{
					break;
				}
				++this->caretIndex;
			}
		}
		this->_shiftMode && this->selectable ? this->selectionCount += previousCaretIndex - this->caretIndex : this->selectionCount = 0;
		this->_blinkTimer = 0.0f;
	}
	
	void EditBox::_deleteLeft(int count)
	{
		if (this->caretIndex > 0)
		{
			count = hmin(count, this->caretIndex);
			hstr left;
			if (this->caretIndex > count)
			{
				left = this->text.utf8_substr(0, this->caretIndex - count);
			}
			hstr right;
			int size = this->text.utf8_size();
			if (this->caretIndex < size)
			{
				right = this->text.utf8_substr(this->caretIndex, size - this->caretIndex);
			}
			this->caretIndex -= count;
			this->text = left + right;
			this->_blinkTimer = 0.0f;
		}
	}
	
	void EditBox::_deleteRight(int count)
	{
		count = hmin(count, this->text.utf8_size() - this->caretIndex);
		this->caretIndex += count;
		this->_deleteLeft(count);
	}
	
	void EditBox::_deleteLeftWord()
	{
		int index = this->caretIndex;
		while (index > 0 && this->text.first_unicode_char(index - 1) == UNICODE_CHAR_SPACE)
		{
			--index;
		}
		while (index > 0 && this->text.first_unicode_char(index - 1) != UNICODE_CHAR_SPACE)
		{
			--index;
		}
		if (this->caretIndex > index)
		{
			this->_deleteLeft(this->caretIndex - index);
		}
	}
	
	void EditBox::_deleteRightWord()
	{
		int index = this->caretIndex;
		int size = this->text.utf8_size();
		while (index < size && this->text.first_unicode_char(index) != UNICODE_CHAR_SPACE)
		{
			++index;
		}
		while (index < size && this->text.first_unicode_char(index) == UNICODE_CHAR_SPACE)
		{
			++index;
		}
		if (index > this->caretIndex)
		{
			this->_deleteRight(index - this->caretIndex);
		}
	}
	
	bool EditBox::_deleteSelected()
	{
		bool result = false;
		if (this->selectionCount > 0)
		{
			this->_deleteRight(this->selectionCount);
			result = true;
		}
		else if (this->selectionCount < 0)
		{
			this->_deleteLeft(-this->selectionCount);
			result = true;
		}
		this->selectionCount = 0;
		return result;
	}

	void EditBox::_insertChar(unsigned int charCode)
	{
		this->_deleteSelected();
		int size = this->text.utf8_size();
		if (this->maxLength > 0 && size >= this->maxLength)
		{
			return;
		}
		hstr left;
		if (this->caretIndex > 0)
		{
			left = this->text.utf8_substr(0, this->caretIndex);
		}
		hstr right;
		if (this->caretIndex < size)
		{
			right = this->text.utf8_substr(this->caretIndex, size - this->caretIndex);
		}
		++this->caretIndex;
		this->text = (left + hstr::from_unicode(charCode)) + right;
		this->_blinkTimer = 0.0f;
	}
	

}
