/// @file
/// @version 3.34
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
	public:
		EditBox(chstr name, grect rect);
		~EditBox();
		hstr getClassName() const { return "EditBox"; }

		static Object* createInstance(chstr name, grect rect);
		
		HL_DEFINE_GET(hstr, emptyText, EmptyText);
		void setEmptyText(chstr value);
		HL_DEFINE_GET(hstr, emptyTextKey, EmptyTextKey);
		void setEmptyTextKey(chstr value);
		HL_DEFINE_GET(int, caretIndex, CaretIndex);
		void setCaretIndex(int value);
		HL_DEFINE_GET(int, selectionCount, SelectionCount);
		void setSelectionCount(int value);
		HL_DEFINE_GET(int, maxLength, MaxLength);
		void setMaxLength(int value);
		HL_DEFINE_GETSET(char, passwordChar, PasswordChar);
		HL_DEFINE_ISSET(multiLine, MultiLine);
		HL_DEFINE_ISSET(selectable, Selectable);
		HL_DEFINE_GET(hstr, filter, Filter);
		void setFilter(chstr value);
		HL_DEFINE_GETSET(april::Color, selectionColor, SelectionColor);
		HL_DEFINE_GET(gvec2, caretPosition, CaretPosition);
		void setText(chstr value);
		void setFocused(bool value);
		hstr getSelectedText();

		harray<PropertyDescription> getPropertyDescriptions();
		
		void setCaretIndexAt(gvec2 position);

		void update(float time);

		void notifyEvent(chstr type, EventArgs* args);
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onKeyDown(april::Key keyCode);
		bool onKeyUp(april::Key keyCode);
		bool onChar(unsigned int charCode);
		void mouseCancel();

		DEPRECATED_ATTRIBUTE int getCursorIndex() { return getCaretIndex(); }
		DEPRECATED_ATTRIBUTE void setCursorIndex(int value) { this->setCaretIndex(value); }
		DEPRECATED_ATTRIBUTE void setCursorIndexAt(float x, float y) { this->setCaretIndexAt(gvec2(x, y)); }
		
	protected:
		hstr emptyText;
		hstr emptyTextKey;
		bool pushed;
		int caretIndex;
		int selectionCount;
		int maxLength;
		char passwordChar;
		bool multiLine;
		bool selectable;
		bool spaceHack; // TODO - remove
		hstr filter;
		april::Color selectionColor;
		gvec2 caretPosition;
		int renderOffsetX;
		int renderOffsetY;

		void _getBaseOffset(gvec2& offset, float& xhf);
		
		void OnDraw();

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
		gvec2 _makeCaretPosition(chstr text, chstr originalText);
		int _makeCaretIndexAt(gvec2 position);
		harray<grect> _makeSelectedRects();

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		bool _ctrlMode;
		bool _shiftMode;
		float _blinkTimer;
		bool _sizeProblemReported;

	};

}

#endif
