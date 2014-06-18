/// @file
/// @version 3.2
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
		HL_DEFINE_GET(int, cursorIndex, CursorIndex);
		void setCursorIndex(int value);
		HL_DEFINE_GET(int, maxLength, MaxLength);
		void setMaxLength(int value);
		HL_DEFINE_GETSET(char, passwordChar, PasswordChar);
		HL_DEFINE_ISSET(multiLine, MultiLine);
		HL_DEFINE_GET(hstr, filter, Filter);
		void setFilter(chstr value);
		HL_DEFINE_GET(gvec2, caretPosition, CaretPosition);
		void setText(chstr value);
		void setFocused(bool value);

		harray<PropertyDescription> getPropertyDescriptions();
		
		void setCursorIndexAt(float x, float y);

		void update(float time);

		void notifyEvent(chstr name, void* params);
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
		bool onMouseDown(april::Key keyCode);
		bool onMouseUp(april::Key keyCode);
		bool onKeyDown(april::Key keyCode);
		bool onKeyUp(april::Key keyCode);
		bool onChar(unsigned int charCode);
		void mouseCancel();
		
	protected:
		hstr emptyText;
		hstr emptyTextKey;
		bool pushed;
		int cursorIndex;
		int maxLength;
		char passwordChar;
		bool multiLine;
		bool spaceHack; // TODO - remove
		hstr filter;
		gvec2 caretPosition;
		
		void OnDraw();
		
		void _cursorMoveLeft();
		void _cursorMoveRight();
		void _cursorMoveUp();
		void _cursorMoveDown();
		void _cursorMoveLeftWord();
		void _cursorMoveRightWord();
		void _deleteLeft(int count = 1);
		void _deleteRight(int count = 1);
		void _deleteLeftWord();
		void _deleteRightWord();
		void _insertChar(unsigned int charCode);
		gvec2 _makeCaretPosition(chstr text, chstr originalText);

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		bool _ctrlMode;
		float _blinkTimer;
		bool _sizeProblemReported;

	};

}

#endif
