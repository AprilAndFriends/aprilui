/// @file
/// @version 4.0
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
#define CHECK_RECT (grect(0.0f, 0.0f, this->rect.w, CHECK_RECT_HEIGHT))

#define MAKE_RENDER_LINES(text) \
	(this->textFormatting ? \
		atres::renderer->makeRenderLines(this->font, CHECK_RECT, text, this->horzFormatting, this->vertFormatting) : \
		atres::renderer->makeRenderLinesUnformatted(this->font, CHECK_RECT, text, this->horzFormatting, this->vertFormatting))

namespace aprilui
{
	harray<PropertyDescription> EditBox::_propertyDescriptions;

	EditBox::EditBox(chstr name) : Label(name), ButtonBase()
	{
		this->text = "";
		this->horzFormatting = atres::LEFT;
		this->textFormatting = false;
		this->backgroundColor = april::Color::Black;
		// this class' properties
		this->emptyText = "";
		this->emptyTextKey = "";
		this->emptyTextColor = april::Color::White;
		this->maxLength = 0;
		this->passwordChar = '\0';
		this->filter = "";
		this->caretIndex = 0;
		this->multiLine = false;
		this->selectable = false;
		this->selectionCount = 0;
		this->selectionColor = april::Color::Grey;
		this->disabledOffset = false;
		this->renderOffsetX = 0;
		this->renderOffsetY = 0;
		this->_ctrlMode = false;
		this->_shiftMode = false;
		this->_blinkTimer = 0.0f;
		this->_caretDirty = true; // calculates initial value
		this->_selectionDirty = true; // calculates initial value
		this->_caretPositionDirty = false; // this is used only for calculation of clicks, should not be calculated initially
		this->_sizeProblemReported = false;
	}

	EditBox::EditBox(const EditBox& other) : Label(other), ButtonBase(other)
	{
		this->emptyText = other.emptyText;
		this->emptyTextKey = other.emptyTextKey;
		this->emptyTextColor = other.emptyTextColor;
		this->maxLength = other.maxLength;
		this->passwordChar = other.passwordChar;
		this->filter = other.filter;
		this->caretIndex = other.caretIndex;
		this->caretOffset = other.caretOffset;
		this->multiLine = other.multiLine;
		this->selectable = other.selectable;
		this->selectionCount = other.selectionCount;
		this->selectionColor = other.selectionColor;
		this->disabledOffset = other.disabledOffset;
		this->renderOffsetX = 0;
		this->renderOffsetY = 0;
		this->_ctrlMode = false;
		this->_shiftMode = false;
		this->_blinkTimer = 0.0f;
		this->_caretDirty = true; // calculates initial value
		this->_selectionDirty = true; // calculates initial value
		this->_caretPositionDirty = false; // this is used only for calculation of clicks, should not be calculated initially
		this->_sizeProblemReported = false;
	}

	EditBox::~EditBox()
	{
	}

	Object* EditBox::createInstance(chstr name)
	{
		return new EditBox(name);
	}

	harray<PropertyDescription> EditBox::getPropertyDescriptions()
	{
		if (EditBox::_propertyDescriptions.size() == 0)
		{
			EditBox::_propertyDescriptions += PropertyDescription("empty_text", PropertyDescription::STRING);
			EditBox::_propertyDescriptions += PropertyDescription("empty_text_key", PropertyDescription::STRING);
			EditBox::_propertyDescriptions += PropertyDescription("empty_text_color", PropertyDescription::HEXCOLOR);
			EditBox::_propertyDescriptions += PropertyDescription("max_length", PropertyDescription::INT);
			EditBox::_propertyDescriptions += PropertyDescription("password_char", PropertyDescription::CHAR);
			EditBox::_propertyDescriptions += PropertyDescription("filter", PropertyDescription::STRING);
			EditBox::_propertyDescriptions += PropertyDescription("caret_index", PropertyDescription::INT);
			EditBox::_propertyDescriptions += PropertyDescription("caret_offset", PropertyDescription::GVEC2);
			EditBox::_propertyDescriptions += PropertyDescription("caret_offset_x", PropertyDescription::FLOAT);
			EditBox::_propertyDescriptions += PropertyDescription("caret_offset_y", PropertyDescription::FLOAT);
			EditBox::_propertyDescriptions += PropertyDescription("multi_line", PropertyDescription::BOOL);
			EditBox::_propertyDescriptions += PropertyDescription("selectable", PropertyDescription::BOOL);
			EditBox::_propertyDescriptions += PropertyDescription("selection_count", PropertyDescription::INT);
			EditBox::_propertyDescriptions += PropertyDescription("selection_color", PropertyDescription::HEXCOLOR);
			EditBox::_propertyDescriptions += PropertyDescription("disabled_offset", PropertyDescription::BOOL);
		}
		return (Label::getPropertyDescriptions() + EditBox::_propertyDescriptions);
	}

	hstr EditBox::getName()
	{
		return Label::getName();
	}

	int EditBox::getFocusIndex()
	{
		return Label::getFocusIndex();
	}

	Object* EditBox::getParent()
	{
		return Label::getParent();
	}

	Dataset* EditBox::getDataset()
	{
		return Label::getDataset();
	}

	bool EditBox::isCursorInside()
	{
		return Label::isCursorInside();
	}

	void EditBox::setCaretIndex(int value)
	{
		int newCaretIndex = hclamp(value, 0, this->text.utf8Size());
		if (this->caretIndex != newCaretIndex)
		{
			this->caretIndex = newCaretIndex;
			this->_caretDirty = true;
			this->_selectionDirty = true;
		}
		this->_blinkTimer = 0.0f;
	}

	void EditBox::setSelectionCount(int value)
	{
		int newSelectionCount = (this->selectable ? hclamp(value, -this->caretIndex, this->text.utf8Size() - this->caretIndex) : 0);
		if (this->selectionCount != newSelectionCount)
		{
			this->selectionCount = newSelectionCount;
			this->_selectionDirty = true;
		}
	}

	void EditBox::setMaxLength(int value)
	{
		this->maxLength = value;
		if (this->maxLength > 0 && this->text.utf8Size() > this->maxLength)
		{
			this->text = this->text.utf8SubString(0, this->maxLength);
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
			std::basic_string<unsigned int> ustr = newValue.uStr();
			harray<unsigned int> uText(ustr.c_str(), (int)ustr.size());
			ustr = this->filter.uStr();
			harray<unsigned int> uFilter(ustr.c_str(), (int)ustr.size());
			uFilter |= '\n'; // this is for multiline
			uText &= uFilter; // intersect, remove from first all that are not in second
			newValue = hstr::fromUnicode(uText);
		}
		if (this->maxLength > 0 && newValue.utf8Size() > this->maxLength)
		{
			newValue = newValue.utf8SubString(0, this->maxLength);
		}
		if (this->text != newValue) // a text change has to trigger a recalculation
		{
			this->_caretDirty = true;
			this->_selectionDirty = true;
		}
		Label::setText(newValue);
		this->setCaretIndex(this->caretIndex);
		this->setSelectionCount(0);
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
		return (this->selectionCount != 0 ? this->text.utf8SubString(hmin(this->caretIndex, this->caretIndex + this->selectionCount), habs(this->selectionCount)) : hstr(""));
	}

	void EditBox::setCaretIndexAt(gvec2 position)
	{
		if (!this->pushed || this->_caretCursorPosition != position)
		{
			this->_caretCursorPosition = position;
			this->_caretPositionDirty = true;
		}
	}

	void EditBox::_update(float time)
	{
		Label::_update(time);
		if (!this->pushed)
		{
			this->_blinkTimer += time * 2;
			this->_blinkTimer = (this->_blinkTimer - (int)this->_blinkTimer);
			this->_updateCaretRect();
			this->_updateCaret();
			this->_updateSelection();
			return;
		}
		int index = this->caretIndex;
		this->setCaretIndexAt(this->transformToLocalSpace(aprilui::getCursorPosition()));
		this->_updateCaretRect();
		this->_updateCaret();
		if (this->selectable)
		{
			this->setSelectionCount(this->selectionCount + index - this->caretIndex);
		}
		this->_updateSelection();
	}

	void EditBox::_updateCaretRect()
	{
		if (!this->_caretPositionDirty)
		{
			return;
		}
		this->_caretPositionDirty = false;
		if (this->text == "")
		{
			this->setCaretIndex(0);
			return;
		}
		// if caret already dirty, this calculation should be ignored
		if (this->_caretDirty)
		{
			return;
		}
		atres::Font* font = atres::renderer->getFont(this->font);
		if (font == NULL)
		{
			return;
		}
		float fh = font->getLineHeight();
		gvec2 position = this->_caretCursorPosition;
		// full text
		harray<atres::RenderLine> lines = MAKE_RENDER_LINES(this->text);
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
				for_iter (i, 0, lines.size())
				{
					if (i == lines.size() - 1)
					{
						descender = atres::renderer->getFont(this->font)->getDescender();
					}
					if (hbetweenIE(position.y, lines[i].rect.y, lines[i].rect.y + lines[i].rect.h + descender))
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
						foreach (atres::RenderWord, it, line->words)
						{
							if (hbetweenIE(position.x, (*it).rect.x, (*it).rect.x + (*it).rect.w))
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
								if (hbetweenIE(position.x, ow, ow + cw))
								{
									break;
								}
								++offsetIndex;
								ow += cw;
								if (hbetweenIE(position.x, ow, ow + cw))
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
		this->setCaretIndex(this->text(0, offsetIndex).utf8Size());
	}

	void EditBox::_updateCaret()
	{
		if (!this->_caretDirty)
		{
			return;
		}
		this->_caretDirty = false;
		hstr text = this->text;
		if (this->passwordChar != '\0' && this->text != "")
		{
			text = hstr(this->passwordChar, this->text.utf8Size());
		}
		hstr leftText = text.utf8SubString(0, this->caretIndex);
		atres::Font* font = atres::renderer->getFont(this->font);
		if (font == NULL)
		{
			return;
		}
		float fh = font->getLineHeight();
		float descender = font->getDescender();
		float lh = fh + descender;
		this->caretRect.set(0.0f, 0.0f, 1.0f, fh);
		// full text
		harray<atres::RenderLine> allLines = MAKE_RENDER_LINES(this->text);
		gvec2 base;
		float xhf = 0.0f; // x height factor
		this->_getBaseOffset(base, xhf);
		base.y -= descender * xhf;
		// caret position
		harray<atres::RenderLine> lines;
		if (leftText != "")
		{
			lines = MAKE_RENDER_LINES(leftText);
			if (lines.size() > 0)
			{
				atres::RenderLine line = lines.last();
				atres::RenderLine allLine = allLines[lines.size() - 1];
				this->caretRect.y = allLine.rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT);
				if (line.terminated)
				{
					this->caretRect.y += fh;
					if (allLines.size() > lines.size())
					{
						this->caretRect.x = allLines[lines.size()].rect.x;
					}
					else
					{
						this->caretRect.x = base.x;
					}
				}
				else
				{
					this->caretRect.x = allLine.rect.x + line.rect.w;
				}
			}
		}
		if (leftText == "" || lines.size() == 0)
		{
			if (allLines.size() > 0)
			{
				this->caretRect.setPosition(allLines[0].rect.x, allLines[0].rect.y + xhf * (this->rect.h - CHECK_RECT_HEIGHT));
			}
			else
			{
				this->caretRect.setPosition(base);
			}
		}
		if (this->horzFormatting == atres::RIGHT || this->horzFormatting == atres::RIGHT_WRAPPED)
		{
			this->caretRect.x -= 1;
		}
		this->caretRect += gvec2((float)this->renderOffsetX, (float)this->renderOffsetY) * fh;
		// calculate render offset
		int jumps = 0;
		if (!this->disabledOffset && this->horzFormatting != atres::LEFT_WRAPPED && this->horzFormatting != atres::CENTER_WRAPPED &&
			this->horzFormatting != atres::RIGHT_WRAPPED && this->horzFormatting != atres::JUSTIFIED)
		{
			if (atres::renderer->getTextWidth(this->font, this->text) > this->caretRect.w)
			{
				// left side
				if (this->caretRect.x < fh && (this->horzFormatting != atres::LEFT || this->renderOffsetX < 0))
				{
					jumps = hceil((fh - this->caretRect.x) / fh);
					if (this->horzFormatting == atres::LEFT)
					{
						jumps = hmin(jumps, -this->renderOffsetX);
					}
					if (jumps != 0)
					{
						this->caretRect.x += jumps * fh;
						this->renderOffsetX += jumps;
						if (this->pushed)
						{
							this->_caretPositionDirty = true;
						}
					}
				}
				// right side
				if (this->caretRect.x + fh > this->rect.w && (this->horzFormatting != atres::RIGHT || this->renderOffsetX > 0))
				{
					jumps = -hceil((this->caretRect.x + fh - this->rect.w) / fh);
					if (this->horzFormatting == atres::RIGHT)
					{
						jumps = hmax(jumps, -this->renderOffsetX);
					}
					if (jumps != 0)
					{
						this->caretRect.x += jumps * fh;
						this->renderOffsetX += jumps;
						if (this->pushed)
						{
							this->_caretPositionDirty = true;
						}
					}
				}
			}
			else
			{
				if (this->pushed && this->renderOffsetX != 0)
				{
					this->_caretPositionDirty = true;
				}
				this->renderOffsetX = 0;
			}
		}
		if (this->multiLine)
		{
			// top side
			if (this->caretRect.y < fh * 0.5f && (this->horzFormatting != atres::TOP || this->renderOffsetY < 0))
			{
				jumps = hceil((fh * 0.5f - this->caretRect.y) / fh);
				if (this->vertFormatting == atres::TOP)
				{
					jumps = hmin(jumps, -this->renderOffsetY);
				}
				if (jumps != 0)
				{
					this->caretRect.y += jumps * fh;
					this->renderOffsetY += jumps;
					if (this->pushed)
					{
						this->_caretPositionDirty = true;
					}
				}
			}
			// bottom side
			if (this->caretRect.y + (fh + lh) * 0.5f > this->rect.h && (this->horzFormatting != atres::BOTTOM || this->renderOffsetY > 0))
			{
				jumps = -hceil((this->caretRect.y + (fh + lh) * 0.5f - this->rect.h) / fh);
				if (this->vertFormatting == atres::BOTTOM)
				{
					jumps = hmax(jumps, -this->renderOffsetY);
				}
				if (jumps != 0)
				{
					this->caretRect.y += jumps * fh;
					this->renderOffsetY += jumps;
					if (this->pushed)
					{
						this->_caretPositionDirty = true;
					}
				}
			}
		}
		else
		{
			if (this->pushed && this->renderOffsetY != 0)
			{
				this->_caretPositionDirty = true;
			}
			this->renderOffsetY = 0;
		}
		this->textOffset.set(this->renderOffsetX * fh, this->renderOffsetY * fh);
	}

	void EditBox::_updateSelection()
	{
		if (!this->_selectionDirty)
		{
			return;
		}
		this->_selectionDirty = false;
		this->_selectionRects.clear();
		if (this->selectionCount == 0 || this->text == "")
		{
			return;
		}
		grect rect;
		float fh = atres::renderer->getFont(this->font)->getLineHeight();
		// full text
		harray<atres::RenderLine> allLines = MAKE_RENDER_LINES(this->text);
		gvec2 base;
		float hf = 0.0f; // x height factor
		this->_getBaseOffset(base, hf);
		float yOffset = hf * (this->rect.h - CHECK_RECT_HEIGHT);
		// vars
		hstr textStart = this->text.utf8SubString(0, hmin(this->caretIndex, this->caretIndex + this->selectionCount));
		hstr textEnd = this->text.utf8SubString(0, hmax(this->caretIndex, this->caretIndex + this->selectionCount));
		harray<atres::RenderLine> linesStart;
		harray<atres::RenderLine> linesEnd;
		gvec2 positionStart;
		gvec2 positionEnd;
		atres::RenderLine* allLineStart = NULL;
		atres::RenderLine* allLineEnd = NULL;
		int linesStartCount = 0;
		int linesEndCount = 0;
		if (textStart != "")
		{
			linesStart = MAKE_RENDER_LINES(textStart);
			if (linesStart.size() > 0)
			{
				allLineStart = &allLines[linesStart.size() - 1];
				positionStart.set(allLineStart->rect.x + linesStart.last().rect.w, allLineStart->rect.y + yOffset);
				linesStartCount = linesStart.size();
			}
		}
		if (textStart == "" || linesStart.size() == 0)
		{
			if (allLines.size() > 0)
			{
				allLineStart = &allLines[0];
				positionStart.set(allLineStart->rect.x, allLineStart->rect.y + yOffset);
				linesStartCount = 1;
			}
			else
			{
				positionStart += base;
			}
		}
		if (textEnd != this->text)
		{
			linesEnd = MAKE_RENDER_LINES(textEnd);
			allLineEnd = &allLines[linesEnd.size() - 1];
			positionEnd.set(allLineEnd->rect.x + linesEnd.last().rect.w, allLineEnd->rect.y + yOffset);
			linesEndCount = linesEnd.size();
		}
		else if (allLines.size() > 0)
		{
			allLineEnd = &allLines.last();
			positionEnd.set(allLineEnd->rect.right(), allLineEnd->rect.y + yOffset);
			linesEndCount = allLines.size();
		}
		else
		{
			positionEnd += base;
		}
		gvec2 renderOffset(this->renderOffsetX * fh, this->renderOffsetY * fh);
		if (linesStartCount == linesEndCount)
		{
			this->_selectionRects += grect(positionStart + renderOffset, positionEnd.x - positionStart.x, fh);
		}
		else
		{
			if (allLineStart != NULL)
			{
				this->_selectionRects += grect(positionStart + renderOffset, allLineStart->rect.right() - positionStart.x, fh);
			}
			if (linesEndCount - linesStartCount > 1)
			{
				for_iter (i, linesStartCount, linesEndCount - 1)
				{
					this->_selectionRects += grect(allLines[i].rect.x + renderOffset.x, allLines[i].rect.y + renderOffset.y + yOffset, allLines[i].rect.w, fh);
				}
			}
			if (allLineEnd != NULL)
			{
				this->_selectionRects += grect(allLineEnd->rect.x + renderOffset.x, positionEnd.y + renderOffset.y, positionEnd.x - allLineEnd->rect.x, fh);
			}
		}
	}

	void EditBox::_draw()
	{
		april::Color color = this->color;
		hstr text = this->text;
		if (this->passwordChar != '\0' && this->text != "")
		{
			this->text = hstr(this->passwordChar, this->text.utf8Size());
		}
		if (this->text == "" && this->dataset != NULL && this->dataset->getFocusedObject() != this)
		{
			this->text = this->emptyText;
			this->color = this->emptyTextColor;
		}
		unsigned char alpha = this->backgroundColor.a;
		if (this->pushed)
		{
			alpha = (unsigned char)(alpha * 0.75f);
		}
		atres::Font* font = atres::renderer->getFont(this->font);
		if (font != NULL)
		{
			float lh = font->getLineHeight() + font->getDescender();
			if (this->multiLine && !this->_sizeProblemReported && this->rect.h < lh)
			{
				hlog::warnf(logTag, "EditBox '%s' height (%d) is smaller than the minimum needed line height (%d) for the given font '%s' when using multi-line!", this->name.cStr(), (int)this->rect.h, (int)lh, this->font.cStr());
				this->_sizeProblemReported = true;
			}
		}
		// not using Label::_draw() directly, because the selection needs to be drawn inbetween the background and border
		Object::_draw();
		float disabledAlphaFactor = this->_getDisabledAlphaFactor();
		grect drawRect = this->_getDrawRect();
		april::Color drawColor = this->_getDrawColor();
		april::Color backgroundColor = this->backgroundColor;
		backgroundColor.a = (unsigned char)(backgroundColor.a * drawColor.a_f() * disabledAlphaFactor);
		drawColor.a = (unsigned char)(drawColor.a * disabledAlphaFactor);
		// background
		this->_drawLabelBackground(drawRect, drawColor, backgroundColor);
		// selection
		if (this->selectionCount != 0)
		{
			april::Color selectionColor = this->selectionColor;
			selectionColor.a = (unsigned char)(selectionColor.a * disabledAlphaFactor);
			harray<grect> selectionRects = this->_selectionRects;
			foreach (grect, it, selectionRects)
			{
				(*it) += drawRect.getPosition() + this->caretOffset;
				(*it).clip(drawRect);
				if ((*it).w > 0.0f && (*it).h > 0.0f)
				{
					april::rendersys->drawFilledRect((*it), selectionColor);
				}
			}
		}
		// border and normal label text
		this->_drawLabel(drawRect, drawColor);
		// caret render
		if (this->dataset != NULL && this->dataset->getFocusedObject() == this && this->_blinkTimer < 0.5f)
		{
			grect renderRect = this->caretRect - this->center + this->caretOffset;
			// make sure the carat is visible if the editbox is empty
			if (this->caretIndex == 0)
			{
				renderRect.x += 1.0f;
			}
			renderRect.clip(drawRect);
			if (renderRect.w > 0.0f && renderRect.h > 0.0f)
			{
				april::ColoredVertex v[2];
				v[0].set(renderRect.x, renderRect.y, 0);
				v[1].set(renderRect.x, renderRect.y + renderRect.h, 0);
				v[0].color = april::rendersys->getNativeColorUInt(this->textColor);
				v[1].color = april::rendersys->getNativeColorUInt(this->textColor);
				april::rendersys->render(april::RO_LINE_LIST, v, 2);
			}
		}
		this->text = text;
		this->color = color;
	}

	void EditBox::_getBaseOffset(gvec2& offset, float& hf)
	{
		offset.set(0.0f, 0.0f);
		hf = 0.0f; // x height factor
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
			hf = 0.5f;
		}
		else if (this->vertFormatting == atres::BOTTOM)
		{
			hf = 1.0f;
		}
		offset.y = (h2 * 2 - fh) * hf;
	}

	bool EditBox::triggerEvent(chstr type, april::Key keyCode)
	{
		return Label::triggerEvent(type, keyCode);
	}

	bool EditBox::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Label::triggerEvent(type, keyCode, string);
	}

	bool EditBox::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		return Label::triggerEvent(type, keyCode, position, string, userData);
	}

	bool EditBox::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return Label::triggerEvent(type, buttonCode, string, userData);
	}

	bool EditBox::triggerEvent(chstr type, chstr string, void* userData)
	{
		return Label::triggerEvent(type, string, userData);
	}

	bool EditBox::triggerEvent(chstr type, void* userData)
	{
		return Label::triggerEvent(type, userData);
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
	
	bool EditBox::_mouseDown(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseDown(keyCode);
		if (result)
		{
			this->setCaretIndexAt(this->transformToLocalSpace(aprilui::getCursorPosition()));
			this->setSelectionCount(0);
			this->setFocused(true);
			this->_updateCaretRect();
			this->_updateCaret();
			this->_updateSelection();
			this->triggerEvent(Event::MouseDown, keyCode);
		}
		else
		{
			this->setSelectionCount(0);
		}
		return (result || Label::_mouseDown(keyCode));
	}

	bool EditBox::_mouseUp(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseUp(keyCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::MouseUp, keyCode);
		}
		if (result)
		{
			// some OSes will disable the keyboard if it is shown before a mouse-up event
			april::window->beginKeyboardHandling();
			this->triggerEvent(Event::Click, keyCode);
		}
		return (result || up || Label::_mouseUp(keyCode));
	}

	void EditBox::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		Label::_mouseCancel(keyCode);
	}

	bool EditBox::_mouseMove()
	{
		return (ButtonBase::_mouseMove() || Label::_mouseMove());
	}

	bool EditBox::_keyDown(april::Key keyCode)
	{
		if (this->dataset == NULL || this->dataset->getFocusedObject() == this)
		{
			switch (keyCode)
			{
#if !defined(_ANDROID) && !defined(_IOS) && !defined(_WINP8) // these keys aren't really available on Android, iOS and WinP8
			case april::AK_LEFT:
				this->_ctrlMode ? this->_caretMoveLeftWord() : this->_caretMoveLeft();
				break;
			case april::AK_RIGHT:
				this->_ctrlMode ? this->_caretMoveRightWord() : this->_caretMoveRight();
				break;
			case april::AK_UP:
				this->multiLine ? this->_caretMoveUp() : this->_caretMoveStart();
				break;
			case april::AK_DOWN:
				this->multiLine ? this->_caretMoveDown() : this->_caretMoveEnd();
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
#if !defined(_ANDROID) && !defined(_IOS) && !defined(_WINP8) // these keys aren't really available on Android, iOS and WinP8
			case april::AK_HOME:
				this->_caretMoveStart();
				break;
			case april::AK_END:
				this->_caretMoveEnd();
				break;
			case april::AK_CONTROL:
#ifdef _MAC
			case april::AK_LCOMMAND:
			case april::AK_RCOMMAND:
#endif
				this->_ctrlMode = true;
				break;
			case april::AK_SHIFT:
				this->_shiftMode = true;
				break;
			case april::AK_A:
				if (this->_ctrlMode)
				{
					this->_caretMoveEnd();
					this->setSelectionCount(-this->text.utf8Size());
				}
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
		return Label::_keyDown(keyCode);
	}
	
	bool EditBox::_keyUp(april::Key keyCode)
	{
		switch (keyCode)
		{
		case april::AK_CONTROL:
		case april::AK_MENU:
#ifdef _MAC
		case april::AK_LCOMMAND:
		case april::AK_RCOMMAND:
#endif
			this->_ctrlMode = false;
			break;
		case april::AK_SHIFT:
			this->_shiftMode = false;
			break;
		default:
			break;
		}
		return Label::_keyUp(keyCode);
	}

	bool EditBox::_char(unsigned int charCode)
	{
		if (this->dataset == NULL || this->dataset->getFocusedObject() == this)
		{
			atres::Font* font = atres::renderer->getFont(this->font);
			if (font != NULL && font->hasChar(charCode) && (this->filter.size() == 0 || this->filter.uStr().find_first_of(charCode) != std::string::npos))
			{
				this->_insertChar(charCode);
			}
		}
		return Label::_char(charCode);
	}

	bool EditBox::_buttonDown(april::Button buttonCode)
	{
		bool result = ButtonBase::_buttonDown(buttonCode);
		if (result)
		{
			this->triggerEvent(Event::ButtonDown, buttonCode);
		}
		return (result || Label::_buttonDown(buttonCode));
	}

	bool EditBox::_buttonUp(april::Button buttonCode)
	{
		if (Label::_buttonUp(buttonCode))
		{
			return true;
		}
		bool result = ButtonBase::_buttonUp(buttonCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::ButtonUp, buttonCode);
		}
		if (result)
		{
			this->triggerEvent(Event::ButtonTrigger, buttonCode);
		}
		return (result || up || Label::_buttonUp(buttonCode));
	}

	hstr EditBox::getProperty(chstr name)
	{
		if (name == "empty_text")		return this->getEmptyText();
		if (name == "empty_text_key")	return this->getEmptyTextKey();
		if (name == "empty_text_color")	return this->getEmptyTextColor().hex();
		if (name == "max_length")		return this->getMaxLength();
		if (name == "password_char")	return this->getPasswordChar();
		if (name == "filter")			return this->getFilter();
		if (name == "caret_index")		return this->getCaretIndex();
		if (name == "cursor_index")
		{
			hlog::warn(logTag, "'cursor_index' is deprecated. Use 'caret_index' instead."); // DEPRECATED
			return this->getCaretIndex();
		}
		if (name == "caret_offset")		return april::gvec2ToHstr(this->getCaretOffset());
		if (name == "caret_offset_x")	return this->getCaretOffsetX();
		if (name == "caret_offset_y")	return this->getCaretOffsetY();
		if (name == "multi_line")		return this->isMultiLine();
		if (name == "selectable")		return this->isSelectable();
		if (name == "selection_count")	return this->getSelectionCount();
		if (name == "selection_color")	return this->getSelectionColor().hex();
		if (name == "disabled_offset")	return this->isDisabledOffset();
		return Label::getProperty(name);
	}
	
	bool EditBox::setProperty(chstr name, chstr value)
	{
		if		(name == "empty_text")			this->setEmptyText(value);
		else if	(name == "empty_text_key")		this->setEmptyTextKey(value);
		else if (name == "empty_text_color")	this->setEmptyTextColor(value);
		else if (name == "max_length")			this->setMaxLength(value);
		else if (name == "password_char")		this->setPasswordChar(value.cStr()[0]);
		else if (name == "filter")				this->setFilter(value);
		else if (name == "caret_index")			this->setCaretIndex(value);
		else if (name == "cursor_index")
		{
			hlog::warn(logTag, "'cursor_index=' is deprecated. Use 'caret_index=' instead."); // DEPRECATED
			this->setCaretIndex(value);
		}
		else if (name == "caret_offset")		this->setCaretOffset(april::hstrToGvec2(value));
		else if (name == "caret_offset_x")		this->setCaretOffsetX(value);
		else if (name == "caret_offset_y")		this->setCaretOffsetY(value);
		else if (name == "multi_line")			this->setMultiLine(value);
		else if (name == "selectable")			this->setSelectable(value);
		else if (name == "selection_count")		this->setSelectionCount(value);
		else if (name == "selection_color")		this->setSelectionColor(value);
		else if (name == "disabled_offset")		this->setDisabledOffset(value);
		else return Label::setProperty(name, value);
		return true;
	}

	void EditBox::_updateSelectionCount(int previousCaretIndex)
	{
		this->setSelectionCount(this->_shiftMode && this->selectable ? this->selectionCount + previousCaretIndex - this->caretIndex : 0);
	}
	
	void EditBox::_caretMoveStart()
	{
		int index = this->caretIndex;
		this->setCaretIndex(0);
		this->_updateSelectionCount(index);
	}

	void EditBox::_caretMoveEnd()
	{
		int index = this->caretIndex;
		this->setCaretIndex(this->text.utf8Size());
		this->_updateSelectionCount(index);
	}

	void EditBox::_caretMoveLeft()
	{
		int index = this->caretIndex;
		this->setCaretIndex(this->caretIndex - 1);
		this->_updateSelectionCount(index);
	}

	void EditBox::_caretMoveRight()
	{
		int index = this->caretIndex;
		this->setCaretIndex(this->caretIndex + 1);
		this->_updateSelectionCount(index);
	}

	void EditBox::_caretMoveUp()
	{
		int index = this->caretIndex;
		if (this->caretIndex > 0)
		{
			this->_updateCaretRect();
			this->setCaretIndexAt(gvec2(this->caretRect.x, this->caretRect.y - atres::renderer->getFont(this->font)->getLineHeight() * 0.5f));
			this->_updateCaretRect();
		}
		this->_updateSelectionCount(index);
	}
	
	void EditBox::_caretMoveDown()
	{
		int index = this->caretIndex;
		if (this->caretIndex < this->text.utf8Size())
		{
			this->_updateCaretRect();
			this->setCaretIndexAt(gvec2(this->caretRect.x, this->caretRect.y + atres::renderer->getFont(this->font)->getLineHeight() * 1.5f));
			this->_updateCaretRect();
		}
		this->_updateSelectionCount(index);
	}
	
	void EditBox::_caretMoveLeftWord()
	{
		int newCaretIndex = this->caretIndex;
		unsigned char c = 0;
		bool first = true;
		bool newLine = false;
		while (newCaretIndex > 0)
		{
			c = this->text.firstUnicodeChar(newCaretIndex - 1);
			if (c != UNICODE_CHAR_SPACE && c != UNICODE_CHAR_NEWLINE)
			{
				break;
			}
			if (c == UNICODE_CHAR_NEWLINE)
			{
				newLine = true;
				if (first)
				{
					--newCaretIndex;
				}
				break;
			}
			first = false;
			--newCaretIndex;
		}
		if (!newLine)
		{
			while (newCaretIndex > 0)
			{
				c = this->text.firstUnicodeChar(newCaretIndex - 1);
				if (c == UNICODE_CHAR_SPACE || c == UNICODE_CHAR_NEWLINE)
				{
					break;
				}
				--newCaretIndex;
			}
		}
		int index = this->caretIndex;
		this->setCaretIndex(newCaretIndex);
		this->_updateSelectionCount(index);
	}
	
	void EditBox::_caretMoveRightWord()
	{
		int newCaretIndex = this->caretIndex;
		int size = this->text.utf8Size();
		unsigned char c = 0;
		bool first = true;
		bool newLine = false;
		while (newCaretIndex < size)
		{
			c = this->text.firstUnicodeChar(newCaretIndex);
			if (c == UNICODE_CHAR_SPACE)
			{
				break;
			}
			if (c == UNICODE_CHAR_NEWLINE)
			{
				newLine = true;
				if (first)
				{
					++newCaretIndex;
				}
				break;
			}
			first = false;
			++newCaretIndex;
		}
		if (!newLine)
		{
			while (newCaretIndex < size)
			{
				c = this->text.firstUnicodeChar(newCaretIndex);
				if (c != UNICODE_CHAR_SPACE && c != UNICODE_CHAR_NEWLINE)
				{
					break;
				}
				++newCaretIndex;
			}
		}
		int index = this->caretIndex;
		this->setCaretIndex(newCaretIndex);
		this->_updateSelectionCount(index);
	}
	
	void EditBox::_deleteLeft(int count)
	{
		if (this->caretIndex > 0)
		{
			int newCaretIndex = this->caretIndex;
			count = hmin(count, newCaretIndex);
			hstr left;
			if (newCaretIndex > count)
			{
				left = this->text.utf8SubString(0, newCaretIndex - count);
			}
			hstr right;
			int size = this->text.utf8Size();
			if (newCaretIndex < size)
			{
				right = this->text.utf8SubString(newCaretIndex, size - newCaretIndex);
			}
			newCaretIndex -= count;
			this->text = left + right;
			this->_blinkTimer = 0.0f;
			this->setCaretIndex(newCaretIndex);
		}
		this->setSelectionCount(0);
	}
	
	void EditBox::_deleteRight(int count)
	{
		int newCaretIndex = this->caretIndex;
		count = hmin(count, this->text.utf8Size() - newCaretIndex);
		newCaretIndex += count;
		this->setCaretIndex(newCaretIndex);
		this->setSelectionCount(0);
		this->_deleteLeft(count);
	}
	
	void EditBox::_deleteLeftWord()
	{
		int index = this->caretIndex;
		while (index > 0 && this->text.firstUnicodeChar(index - 1) == UNICODE_CHAR_SPACE)
		{
			--index;
		}
		while (index > 0 && this->text.firstUnicodeChar(index - 1) != UNICODE_CHAR_SPACE)
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
		int size = this->text.utf8Size();
		while (index < size && this->text.firstUnicodeChar(index) != UNICODE_CHAR_SPACE)
		{
			++index;
		}
		while (index < size && this->text.firstUnicodeChar(index) == UNICODE_CHAR_SPACE)
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
		this->setSelectionCount(0);
		return result;
	}

	void EditBox::_insertChar(unsigned int charCode)
	{
		if (this->_ctrlMode && (charCode == 'A' || charCode == 'a'))
		{
			return;
		}
		this->_deleteSelected();
		int size = this->text.utf8Size();
		if (this->maxLength > 0 && size >= this->maxLength)
		{
			return;
		}
		hstr left;
		if (this->caretIndex > 0)
		{
			left = this->text.utf8SubString(0, this->caretIndex);
		}
		hstr right;
		if (this->caretIndex < size)
		{
			right = this->text.utf8SubString(this->caretIndex, size - this->caretIndex);
		}
		this->text = (left + hstr::fromUnicode(charCode)) + right;
		this->setCaretIndex(this->caretIndex + 1);
	}
	

}
