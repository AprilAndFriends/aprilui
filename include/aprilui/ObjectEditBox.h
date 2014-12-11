/// @file
/// @version 4.0
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
#include "ObjectLabel.h"

namespace aprilui
{
	class apriluiExport EditBox : public Label
	{
		APRILUI_CLONEABLE(EditBox);
	public:
		EditBox(chstr name);
		~EditBox();
		inline hstr getClassName() const { return "EditBox"; }

		static Object* createInstance(chstr name);
		
		HL_DEFINE_GET(hstr, emptyText, EmptyText);
		void setEmptyText(chstr value);
		HL_DEFINE_GET(hstr, emptyTextKey, EmptyTextKey);
		void setEmptyTextKey(chstr value);
		HL_DEFINE_GETSET(april::Color, emptyTextColor, EmptyTextColor);
		HL_DEFINE_GET(int, caretIndex, CaretIndex);
		void setCaretIndex(int value);
		HL_DEFINE_GETSET(gvec2, caretOffset, CaretOffset);
		HL_DEFINE_GETSET(float, caretOffset.x, CaretOffsetX);
		HL_DEFINE_GETSET(float, caretOffset.y, CaretOffsetY);
		HL_DEFINE_ISSET(multiLine, MultiLine);
		HL_DEFINE_ISSET(selectable, Selectable);
		HL_DEFINE_GET(int, selectionCount, SelectionCount);
		void setSelectionCount(int value);
		HL_DEFINE_GET(int, maxLength, MaxLength);
		void setMaxLength(int value);
		HL_DEFINE_GETSET(char, passwordChar, PasswordChar);
		HL_DEFINE_ISSET(disabledOffset, DisabledOffset);
		HL_DEFINE_GET(hstr, filter, Filter);
		void setFilter(chstr value);
		HL_DEFINE_GETSET(april::Color, selectionColor, SelectionColor);
		HL_DEFINE_GET(grect, caretRect, CaretRect);
		void setText(chstr value);
		void setFocused(bool value);
		hstr getSelectedText();

		harray<PropertyDescription> getPropertyDescriptions();
		
		void setCaretIndexAt(gvec2 position);

		void update(float time);

		void notifyEvent(chstr type, EventArgs* args);
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
		DEPRECATED_ATTRIBUTE inline int getCursorIndex() { return getCaretIndex(); }
		DEPRECATED_ATTRIBUTE inline void setCursorIndex(int value) { this->setCaretIndex(value); }
		DEPRECATED_ATTRIBUTE inline void setCursorIndexAt(float x, float y) { this->setCaretIndexAt(gvec2(x, y)); }
		DEPRECATED_ATTRIBUTE inline gvec2 getCaretPosition() { return gvec2(this->caretRect.x, this->caretRect.y + this->caretRect.h * 0.5f); }
		
	protected:
		hstr emptyText;
		hstr emptyTextKey;
		april::Color emptyTextColor;
		int maxLength;
		char passwordChar;
		hstr filter;
		int caretIndex;
		gvec2 caretOffset;
		bool multiLine;
		bool selectable;
		int selectionCount;
		april::Color selectionColor;
		bool disabledOffset;

		grect caretRect;
		bool pushed;
		int renderOffsetX;
		int renderOffsetY;

		void _updateCaretRect();
		void _updateCaret();
		void _updateSelection();
		
		void _draw();

		void _getBaseOffset(gvec2& offset, float& hf);

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

		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _keyDown(april::Key keyCode);
		bool _keyUp(april::Key keyCode);
		bool _char(unsigned int charCode);

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		bool _ctrlMode;
		bool _shiftMode;
		float _blinkTimer;
		bool _caretPositionDirty;
		bool _caretDirty;
		bool _selectionDirty;
		bool _sizeProblemReported;
		gvec2 _caretCursorPosition;
		harray<grect> _selectionRects;

	};

}

#endif
