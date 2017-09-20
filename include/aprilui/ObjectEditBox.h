/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a text editbox.

#ifndef APRILUI_EDIT_BOX_H
#define APRILUI_EDIT_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectButtonBase.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class apriluiExport EditBox : public Label, public ButtonBase
	{
		APRILUI_CLONEABLE(EditBox);
	public:
		EditBox(chstr name);
		~EditBox();
		inline hstr getClassName() const { return "EditBox"; }
		static Object* createInstance(chstr name);
		
		harray<PropertyDescription> getPropertyDescriptions() const;

		hstr getName() const;
		int getFocusIndex() const;
		Object* getParent() const;
		Dataset* getDataset() const;
		bool isCursorInside() const;

		HL_DEFINE_GET(hstr, emptyText, EmptyText);
		void setEmptyText(chstr value);
		HL_DEFINE_GET(hstr, emptyTextKey, EmptyTextKey);
		void setEmptyTextKey(chstr value);
		HL_DEFINE_GETSET(april::Color, emptyTextColor, EmptyTextColor);
		void setEmptyTextSymbolicColor(chstr value);
		HL_DEFINE_GET(int, caretIndex, CaretIndex);
		void setCaretIndex(int value);
		HL_DEFINE_GETSET(gvec2, caretOffset, CaretOffset);
		HL_DEFINE_GETSET(float, caretOffset.x, CaretOffsetX);
		HL_DEFINE_GETSET(float, caretOffset.y, CaretOffsetY);
		HL_DEFINE_GETSET(april::Color, caretColor, CaretColor);
		void setCaretSymbolicColor(chstr value);
		HL_DEFINE_ISSET(multiLine, MultiLine);
		HL_DEFINE_ISSET(selectable, Selectable);
		HL_DEFINE_GET(int, selectionCount, SelectionCount);
		void setSelectionCount(int value);
		HL_DEFINE_GETSET(april::Color, selectionColor, SelectionColor);
		void setSelectionSymbolicColor(chstr value);
		HL_DEFINE_ISSET(clipboardEnabled, ClipboardEnabled);
		HL_DEFINE_GET(int, maxLength, MaxLength);
		void setMaxLength(int value);
		HL_DEFINE_GETSET(char, passwordChar, PasswordChar);
		HL_DEFINE_ISSET(disabledOffset, DisabledOffset);
		HL_DEFINE_GET(hstr, filter, Filter);
		void setFilter(chstr value);
		HL_DEFINE_GET(grect, caretRect, CaretRect);
		void setText(chstr value);
		void setFocused(bool value);
		hstr getSelectedText() const;
		hstr getDisplayedText() const;
		void setMinAutoScale(float value);

		void setCaretIndexAt(cgvec2 position);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2 position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

		static hstr defaultFilterUnsignedNumeric;
		static hstr defaultFilterUnsignedDecimal;
		static hstr defaultFilterNumeric;
		static hstr defaultFilterDecimal;
		static hstr defaultFilterAlphaUpperCase;
		static hstr defaultFilterAlphaLowerCase;
		static hstr defaultFilterAlpha;
		static hstr defaultFilterAlphaNumericUpperCase;
		static hstr defaultFilterAlphaNumericLowerCase;
		static hstr defaultFilterAlphaNumeric;
		static hstr defaultFilterAsciiNoSpace;
		static hstr defaultFilterAscii;
		static hstr defaultFilterEmail;
		static hstr defaultFilterEmailComplete;

		DEPRECATED_ATTRIBUTE inline int getCursorIndex() const { return getCaretIndex(); }
		DEPRECATED_ATTRIBUTE inline void setCursorIndex(int value) { this->setCaretIndex(value); }
		DEPRECATED_ATTRIBUTE inline void setCursorIndexAt(float x, float y) { this->setCaretIndexAt(gvec2(x, y)); }
		DEPRECATED_ATTRIBUTE inline gvec2 getCaretPosition() const { return gvec2(this->caretRect.x, this->caretRect.y + this->caretRect.h * 0.5f); }
		
	protected:
		hstr emptyText;
		hstr emptyTextKey;
		april::Color emptyTextColor;
		int maxLength;
		char passwordChar;
		hstr filter;
		int caretIndex;
		gvec2 caretOffset;
		april::Color caretColor;
		bool multiLine;
		bool selectable;
		int selectionCount;
		april::Color selectionColor;
		bool clipboardEnabled;
		bool disabledOffset;

		grect caretRect;
		int renderOffsetX;
		int renderOffsetY;

		void _updateCaretPosition();
		void _updateCaret();
		void _updateSelection();
		gvec2 _makeCaretPosition(const harray<atres::RenderLine>& lines, int index, cgvec2 base, float fontHeight, float heightOffset, int* lineIndex = NULL);
		void _makeBaseOffset(gvec2& offset, float& heightOffset, float* heightFactor = NULL) const;

		void _update(float timeDelta);
		void _draw();

		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();
		bool _keyDown(april::Key keyCode);
		bool _keyUp(april::Key keyCode);
		bool _char(unsigned int charCode);
		bool _buttonDown(april::Button buttonCode);
		bool _buttonUp(april::Button buttonCode);

		april::Color _makeSelectionDrawColor(const april::Color& drawColor) const;

		void _updateSelectionCount(int previousCaretIndex);
		void _caretMoveStart();
		void _caretMoveEnd();
		void _caretMoveLeft();
		void _caretMoveRight();
		void _caretMoveUp();
		void _caretMoveDown();
		void _caretMoveLeftWord();
		void _caretMoveRightWord();
		void _deleteLeft(int count = 1);
		void _deleteRight(int count = 1);
		void _deleteLeftWord();
		void _deleteRightWord();
		bool _deleteSelected();
		void _insertChar(unsigned int charCode);
		void _cutText();
		void _copyText();
		void _pasteText();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		bool _consumeKey;
		bool _ctrlMode;
		bool _altMode;
		bool _shiftMode;
		float _blinkTimer;
		bool _caretDirty;
		bool _selectionDirty;
		bool _caretPositionDirty;
		bool _sizeProblemReported;
		gvec2 _caretCursorPosition;
		harray<grect> _selectionRects;

	};

}

#endif
